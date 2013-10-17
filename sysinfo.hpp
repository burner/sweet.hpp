#include <cxxabi.h>
#include <execinfo.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <ucontext.h>
#include <unistd.h>

// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0
/*inline void process_mem_usage(double& vm_usage, double& resident_set) {
	using std::ios_base;
	using std::ifstream;
	using std::string;

	vm_usage = 0.0;
	resident_set = 0.0;

	// 'file' stat seems to give the most reliable results
	ifstream stat_stream("/proc/self/stat",ios_base::in);

	// dummy vars for leading entries in stat that we don't care about
	string pid, comm, state, ppid, pgrp, session, tty_nr;
	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	string utime, stime, cutime, cstime, priority, nice;
	string O, itrealvalue, starttime;

	// the two fields we want
	unsigned long vsize;
	long rss;

	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
					>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
					>> utime >> stime >> cutime >> cstime >> priority >> nice
					>> O >> itrealvalue >> starttime >> vsize >> rss; 
	// don't care about the rest

	stat_stream.close();
	
	// in case x86-64 is configured to use 2MB pages
	long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; 
	vm_usage = vsize / 1024.0;
	resident_set = rss * page_size_kb;
}*/

inline void process_mem_usage(double& vm_usage, double& resident_set) {
	FILE* file = fopen("/proc/self/statm", "r");
	int a, b, c, d, e, f, g;
	fscanf(file, "%d %d %d %d %d %d %d", &a, &b, &c, &d, &e, &f, &g);
	fclose(file);
	vm_usage = a;
	resident_set = b;
}

#ifdef __X86_64__ 
typedef struct _sig_ucontext {
	unsigned long     uc_flags;
	struct ucontext   *uc_link;
	stack_t           uc_stack;
	struct sigcontext uc_mcontext;
	sigset_t          uc_sigmask;
} sig_ucontext_t;

inline void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext) {
	sig_ucontext_t * uc = (sig_ucontext_t *)ucontext;

    void * caller_address = (void *) uc->uc_mcontext.rip; // x86 specific

    std::cerr << "signal " << sig_num 
              << " (" << strsignal(sig_num) << "), address is " 
              << info->si_addr << " from " << caller_address 
              << std::endl << std::endl;

    void * array[50];
    int size = backtrace(array, 50);

    array[1] = caller_address;

    char ** messages = backtrace_symbols(array, size);    

    // skip first stack frame (points here)
    for(int i = 1; i < size && messages != NULL; ++i) {
        char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

        // find parantheses and +address offset surrounding mangled name
        for(char *p = messages[i]; *p; ++p) {
            if(*p == '(') {
                mangled_name = p; 
            } else if(*p == '+') {
                offset_begin = p;
            } else if(*p == ')') {
                offset_end = p;
                break;
            }
        }

        // if the line could be processed, attempt to demangle the symbol
        if(mangled_name && offset_begin && offset_end && 
				mangled_name < offset_begin) {
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            int status;
            char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

            // if demangling is successful, output the demangled function name
            if(status == 0) {    
                std::cerr << "[bt]: (" << i << ") " << messages[i] << " : " 
                          << real_name << "+" << offset_begin << offset_end 
                          << std::endl;

            } else { // otherwise, output the mangled function name
                std::cerr << "[bt]: (" << i << ") " << messages[i] << " : " 
                          << mangled_name << "+" << offset_begin << offset_end 
                          << std::endl;
            }
            free(real_name);
        } else { // otherwise, print the whole line
            std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
        }
    }
    std::cerr<<std::endl;

    free(messages);

    exit(EXIT_FAILURE);
}

#else

inline void handler(int) {
	/*
	void *array[10];
	size_t size;
	
	// get void*'s for all entries on the stack
	size = backtrace(array, 10);
	
	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, 2);
	exit(1);
	*/
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    int child_pid = fork();
    if(!child_pid) {           
        dup2(2,1); // redirect output to stderr
        fprintf(stdout,"stack trace for %s pid=%s\n",name_buf,pid_buf);
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", 
			name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
		exit(EXIT_FAILURE);
    } else {
        waitpid(child_pid,NULL,0);
		exit(EXIT_FAILURE);
    }
	exit(EXIT_FAILURE);
}

#endif

inline void installHandler() {
#ifdef __X86_64__ 
	struct sigaction sigact;
	
	sigact.sa_sigaction = crit_err_hdlr;
	sigact.sa_flags = SA_RESTART | SA_SIGINFO;
	
	if(sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0) {
		fprintf(stderr, "error setting signal handler for %d (%s)\n",
			SIGSEGV, strsignal(SIGSEGV));
		exit(EXIT_FAILURE);
	}
#else
	signal(SIGSEGV, handler); 
#endif
}

inline size_t numberOfCpus() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}
