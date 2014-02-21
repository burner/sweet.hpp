// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
// based on DeathHandler by Markovtsev Vadim

/**
 *
 *	int main() {
 *		sweet::DeathHandler dh;
 *		int* p = NULL;
 *		*p = 0;
 *		return 0;
 *	}
 */

#pragma once
#include <stddef.h>
#include <assert.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <malloc.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

#pragma GCC poison malloc realloc free backtrace_symbols \
	printf fprintf sprintf snprintf scanf sscanf	// NOLINT(runtime/printf)

#define checked(x) do { if ((x) <= 0) _Exit(EXIT_FAILURE); } while (false)

// Comment this out on systems without quick_exit()
#define QUICK_EXIT

namespace sweet {
/// @brief This namespace contains some basic supplements
/// of the needed libc functions which potentially use heap.
namespace Safe {
	/// @brief Converts an integer to a preallocated string.
	inline char *itoa(int val, char* memory, int base = 10) {
		char* res = memory;
		const int res_max_length = 32;
		int i;
		bool negative = val < 0;
		res[res_max_length - 1] = 0;
		for (i = res_max_length - 2; val != 0 && i != 0; i--, val /= base) {
			res[i] = "0123456789ABCDEF"[val % base];
		}
		if (negative) {
			res[i--] = '-';
		}
		return &res[i + 1];
	}

	/// @brief Converts an unsigned integer to a preallocated string.
	inline char *utoa(uint64_t val, char* memory, int base = 10) {
		char* res = memory;
		const int res_max_length = 32;
		int i;
		res[res_max_length - 1] = 0;
		for (i = res_max_length - 2; val != 0 && i != 0; i--, val /= base) {
			res[i] = "0123456789ABCDEF"[val % base];
		}
		return &res[i + 1];
	}

	/// @brief Converts a pointer to a preallocated string.
	inline char *ptoa(const void *val, char* memory) {
		char* buf = utoa(reinterpret_cast<uint64_t>(val), memory + 32, 16);
		char* result = memory;	// 32
		strcpy(result + 2, buf);	// NOLINT(runtime/printf
		result[0] = '0';
		result[1] = 'x';
		return result;
	}

	/// @brief Reentrant printing to stderr.
	inline void print2stderr(const char *msg, size_t len = 0) {
		if (len > 0) {
			checked(write(STDERR_FILENO, msg, len));
		} else {
			checked(write(STDERR_FILENO, msg, strlen(msg)));
		}
	}
}

inline static void safe_abort() {
	struct sigaction sa;
	sigaction(SIGABRT, NULL, &sa);
	sa.sa_handler = SIG_DFL;
	kill(getppid(), SIGCONT);
	sigaction(SIGABRT, &sa, NULL);
	abort();
}

/// @brief Invokes addr2line utility to determine the function name
/// and the line information from an address in the code segment.
inline static char *addr2line(const char *image, void *addr, bool color_output,
		char** memory) {
	int pipefd[2];
	if (pipe(pipefd) != 0) {
		safe_abort();
	}
	pid_t pid = fork();
	if (pid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		if (execlp("addr2line", "addr2line",
							 Safe::ptoa(addr, *memory), "-f", "-C", "-e", image,
							 reinterpret_cast<void*>(NULL)) == -1) {
			safe_abort();
		}
	}

	close(pipefd[1]);
	const int line_max_length = 4096;
	char* line = *memory; 
	*memory += line_max_length;
	ssize_t len = read(pipefd[0], line, line_max_length);	
	close(pipefd[0]);
	if (len == 0) {
		safe_abort();
	}
	line[len] = 0;

	if (waitpid(pid, NULL, 0) != pid) {
		safe_abort();
	}
	if (line[0] == '?') {
		char* straddr = Safe::ptoa(addr, *memory);
		if (color_output) {
			strcpy(line, "\033[32;1m");	// NOLINT(runtime/printf)
		}
		strcat(line, straddr);	// NOLINT(runtime/printf)
		if (color_output) {
			strcat(line, "\033[0m");	// NOLINT(runtime/printf)
		}
		strcat(line, " at ");	// NOLINT(runtime/printf)
		strcat(line, image);	// NOLINT(runtime/printf)
		strcat(line, " ");	// NOLINT(runtime/printf)
	} else {
		if (*(strstr(line, "\n") + 1) == '?') {
			char* straddr = Safe::ptoa(addr, *memory);
			strcpy(strstr(line, "\n") + 1, image);	// NOLINT(runtime/printf)
			strcat(line, ":");	// NOLINT(runtime/printf)
			strcat(line, straddr);	// NOLINT(runtime/printf)
			strcat(line, "\n");	// NOLINT(runtime/printf)
		}
	}
	return line;
}

/// @brief This class installs a SEGFAULT signal handler to print
/// a nice stack trace and (if requested) generate a core dump.
/// @details In DeathHandler's constructor, a SEGFAULT signal handler
/// is installed via sigaction(). If your program encounters a segmentation
/// fault, the call stack is unwinded with backtrace(), converted into
/// function names with line numbers via addr2line (fork() + execlp()).
/// Addresses from shared libraries are also converted thanks to dladdr().
/// All C++ symbols are demangled. Printed stack trace includes the faulty
/// thread id obtained with pthread_self() and each line contains the process
/// id to distinguish several stack traces printed by different processes at
/// the same time.
class DeathHandler {
private:
	/// @brief The size of the preallocated memory to use in the signal handler.
	static size_t& kNeededMemoryImpl() {
		static size_t kNeededMemory_ = 12288;
		return kNeededMemory_;
	}

	static bool& generate_core_dumpImpl() {
		static bool generate_core_dump_ = true;
		return generate_core_dump_;
	}
	static bool& cleanupImpl() {
		static bool cleanup_ = true;
		return cleanup_;
	}
#ifdef QUICK_EXIT
	static bool& quick_exitImpl() {
		static bool quick_exit_ = false;
		return quick_exit_;
	}
#endif
	static int& frames_countImpl() {
		static int frames_count_ = 16;
		return frames_count_;
	}
	static bool& cut_common_path_rootImpl() {
		static bool cut_common_path_root_ = true;
		return cut_common_path_root_;
	}
	static bool& cut_relative_pathsImpl() {
		static bool cut_relative_paths_ = true;
		return cut_relative_paths_;
	}
	static bool& append_pidImpl() {
		static bool append_pid_ = false;
		return append_pid_;
	}
	static bool& color_outputImpl() {
		static bool color_output_ = true;
		return color_output_;
	}
	static bool& thread_safeImpl() {
		static bool thread_safe_ = true;
		return thread_safe_;
	}
	static char*& memoryImpl() {
		static char* memory_ = NULL;
		return memory_;
	}

public:
	/// @brief Installs the SIGSEGV/SIGABRT signal handler.
	inline DeathHandler() {
		if (memoryImpl() == NULL) {
			memoryImpl() = new char[kNeededMemoryImpl()];
		}
		struct sigaction sa;
		sa.sa_handler = (__sighandler_t)SignalHandler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART | SA_SIGINFO;
		sigaction(SIGSEGV, &sa, NULL);
		sigaction(SIGABRT, &sa, NULL);	
	}
	/// @brief This is called on normal program termination. Previously installed
	/// SIGSEGV and SIGABRT signal handlers are removed.
	inline ~DeathHandler() {
		struct sigaction sa;
		sigaction(SIGSEGV, NULL, &sa);
		sa.sa_handler = SIG_DFL;
		sigaction(SIGSEGV, &sa, NULL);
		sigaction(SIGABRT, NULL, &sa);
		sa.sa_handler = SIG_DFL;
		sigaction(SIGABRT, &sa, NULL);
		delete[] memoryImpl();
	}

	/// @brief Sets the value of cleanup property.
	/// @details If cleanup is set to true, program attempts to run all static
	/// destructors and atexit() callbacks before terminating. If
	/// generate_core_dump is set to true, this property is ignored.
	/// @note Default value of this property is true.
	inline bool cleanup() {
		return cleanupImpl();
	}

	/// @brief Returns the value of cleanup property.
	/// @details If cleanup is set to true, program attempts to run all static
	/// destructors and atexit() callbacks before terminating. If
	/// generate_core_dump is set to true, this property is ignored.
	/// @note Default value of this property is true.
	inline void set_cleanup(bool value) {
		cleanupImpl() = value;
	}

	/// @brief Returns the current value of generate_core_dump property.
	/// @details If generate_core_dump is set to true, a core dump will
	/// be generated when the program terminates. This behavior
	/// is implemented using abort(). cleanup property is ignored.
	/// @note You need to set ulimit to a value different from the default 0
	/// (for example, "ulimit -c unlimited") to enable core dumps generation
	/// on your system.
	/// @note Default value of this property is true.
	inline bool generate_core_dump() {
		return generate_core_dumpImpl();
	}

	/// @brief Sets the value of generate_core_dump property.
	/// @details If generate_core_dump is set to true, a core dump will
	/// be generated when the program terminates. This behavior
	/// is implemented using abort(). cleanup property is ignored.
	/// @note You need to set ulimit to a value different from the default 0
	/// (for example, "ulimit -c unlimited") to enable core dumps generation
	/// on your system.
	/// @note Default value of this property is true.
	inline void set_generate_core_dump(bool value) {
		generate_core_dumpImpl() = value;
	}

#ifdef QUICK_EXIT
	/// @brief Returns the value of quick_exit property.
	/// @details If quick_exit is set to true, program will be terminated with
	/// quick_exit() call. generate_core_dump and cleanup properties are
	/// ignored.
	/// @note Default value is false.
	inline bool quick_exit() {
		return quick_exitImpl();
	}

	/// @brief Sets the value of quick_exit property.
	/// @details If quick_exit is set to true, program will be terminated with
	/// quick_exit() call. generate_core_dump and cleanup properties are
	/// ignored.
	/// @note Default value is false.
	inline void set_quick_exit(bool value) {
		quick_exitImpl() = value;
	}

	/// @brief Returns the depth of the stack trace.
	/// @note Default value is 16.
	inline int frames_count() {
		return frames_countImpl();
	}

	/// @brief Sets the depth of the stack trace. Accepted range is 1..100.
	/// @note Default value is 16.
	inline void set_frames_count(int value) {
		assert(value > 0 && value <= 100);
 	   frames_countImpl() = value;
	}	
#endif

	/// @brief Returns the value indicating whether to shorten stack trace paths
	/// by cutting off the common root between each path and the current working
	/// directory.
	/// @note Default value is true.
	inline bool cut_common_path_root() {
		return cut_common_path_rootImpl();
	}

	/// @brief Sets the value indicating whether to shorten stack trace paths
	/// by cutting off the common root between each path and the current working
	/// directory.
	/// @note Default value is true.
	inline void set_cut_common_path_root(bool value) {
		cut_common_path_rootImpl() = value;
	}

	/// @brief Returns the value indicating whether to shorten stack trace paths
	/// by cutting off the relative part (e.g., "../../..").
	/// @note Default value is true.
	inline bool cut_relative_paths() {
		return cut_relative_pathsImpl();
	}

	/// @brief Sets the value indicating whether to shorten stack trace paths
	/// by cutting off the relative part (e.g., "../../..").
	/// @note Default value is true.
	inline void set_cut_relative_paths(bool value) {
		cut_relative_pathsImpl() = value;
	}

	/// @brief Returns the value indicating whether to append the process id
	/// to each stack trace line.
	/// @note Default value is false.
	inline bool append_pid() {
		return append_pidImpl();
	}

	/// @brief Sets the value indicating whether to append the process id
	/// to each stack trace line.
	/// @note Default value is false.
	inline void set_append_pid(bool value) {
		append_pidImpl() = value;
	}

	/// @brief Returns the value indicating whether to color the output
	/// with ANSI escape sequences.
	/// @note Default value is true.
	inline bool color_output() {
		return color_outputImpl();
	}

	/// @brief Sets the value indicating whether to color the output
	/// with ANSI escape sequences.
	/// @note Default value is true.
	inline void set_color_output(bool value) {
		color_outputImpl() = value;
	}

	/// @brief Returns the value indicating whether to do a thread-safe
	/// stack trace printing, stopping all running threads by forking.
	/// @note Default value is true.
	inline bool thread_safe() {
		return thread_safeImpl();
	}

	/// @brief Sets the value indicating whether to do a thread-safe stack trace
	/// printing, stopping all running threads by forking.
	/// @note Default value is true.
	void set_thread_safe(bool value) {
		thread_safeImpl() = value;
	}

 private:

	inline static void SignalHandler(int sig, void* info, void* secret) {
		// Stop all other running threads by forking
		pid_t forkedPid = fork();
		if (forkedPid != 0) {
			int status;
			if (thread_safeImpl()) {
				// Freeze the original process, until it's child prints the stack trace
				kill(getpid(), SIGSTOP);
				// Wait for the child without blocking and exit as soon as possible,
				// so that no zombies are left.
				waitpid(forkedPid, &status, WNOHANG);
			} else {
				// Wait for the child, blocking only the current thread.
				// All other threads will continue to run, potentially crashing the parent.
				waitpid(forkedPid, &status, 0);
			}
#ifdef QUICK_EXIT
			if (quick_exitImpl()) {
				::quick_exit(EXIT_FAILURE);
			}
#endif
			if (generate_core_dumpImpl()) {
				struct sigaction sa;
				sigaction(SIGABRT, NULL, &sa);
				sa.sa_handler = SIG_DFL;
				sigaction(SIGABRT, &sa, NULL);
				abort();
			} else {
				if (cleanupImpl()) {
					exit(EXIT_FAILURE);
				} else {
					_Exit(EXIT_FAILURE);
				}
			}
		}
	
		ucontext_t *uc = reinterpret_cast<ucontext_t *>(secret);
	
		if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1) {	// redirect stdout to stderr
			Safe::print2stderr("Failed to redirect stdout to stderr\n");
		}
		char* memory = memoryImpl();
		{
			char* msg = memory;
			const int msg_max_length = 128;
			if (color_outputImpl()) {
				// \033[31;1mSegmentation fault\033[0m \033[33;1m(%i)\033[0m\n
				strcpy(msg, "\033[31;1m");	// NOLINT(runtime/printf)
			} else {
				msg[0] = '\0';
			}
			switch (sig) {
				case SIGSEGV:
					strcat(msg, "Segmentation fault");	// NOLINT(runtime/printf)
					break;
				case SIGABRT:
					strcat(msg, "Aborted");	// NOLINT(runtime/printf)
					break;
				default:
					strcat(msg, "Caught signal ");	// NOLINT(runtime/printf)
					strcat(msg, Safe::itoa(sig, msg + msg_max_length));	// NOLINT(*)
					break;
			}
			if (color_outputImpl()) {
				strcat(msg, "\033[0m");	// NOLINT(runtime/printf)
			}
			strcat(msg, " (thread ");	// NOLINT(runtime/printf)
			if (color_outputImpl()) {
				strcat(msg, "\033[33;1m");	// NOLINT(runtime/printf)
			}
			strcat(msg, Safe::utoa(pthread_self(), msg + msg_max_length));	// NOLINT(*)
			if (color_outputImpl()) {
				strcat(msg, "\033[0m");	// NOLINT(runtime/printf)
			}
			strcat(msg, ", pid ");	// NOLINT(runtime/printf)
			if (color_outputImpl()) {
				strcat(msg, "\033[33;1m");	// NOLINT(runtime/printf)
			}
			strcat(msg, Safe::itoa(getppid(), msg + msg_max_length));	// NOLINT(*)
			if (color_outputImpl()) {
				strcat(msg, "\033[0m");	// NOLINT(runtime/printf)
			}
			strcat(msg, ")");	// NOLINT(runtime/printf)		
			Safe::print2stderr(msg);
		}
	
		Safe::print2stderr("\nStack trace:\n");
		void **trace = reinterpret_cast<void**>(memory);
		memory += (frames_countImpl() + 2) * sizeof(void*);
		// Workaround malloc() inside backtrace()
		void* (*oldMallocHook)(size_t, const void*) = __malloc_hook;
		void (*oldFreeHook)(void *, const void *) = __free_hook;
		__malloc_hook = MallocHook;
		__free_hook = NULL;
		int trace_size = backtrace(trace, frames_countImpl() + 2);
		__malloc_hook = oldMallocHook;
		__free_hook = oldFreeHook;
		if (trace_size <= 2) {
			safe_abort();
		}
	
		// Overwrite sigaction with caller's address
#if defined(__arm__)
		trace[1] = reinterpret_cast<void *>(uc->uc_mcontext.arm_pc);
#else
#if !defined(__i386__) && !defined(__x86_64__)
#error Only ARM, x86 and x86-64 are supported
#endif
#if defined(__x86_64__)
		trace[1] = reinterpret_cast<void *>(uc->uc_mcontext.gregs[REG_RIP]);
#else
		trace[1] = reinterpret_cast<void *>(uc->uc_mcontext.gregs[REG_EIP]);
#endif
#endif
	
		const int path_max_length = 2048;
		char* name_buf = memory;
		ssize_t name_buf_length = readlink("/proc/self/exe", name_buf, path_max_length - 1);
		if (name_buf_length < 1) {
			safe_abort();
		}
		name_buf[name_buf_length] = 0;
		memory += name_buf_length + 1;
		char* cwd = memory;	
		if (getcwd(cwd, path_max_length) == NULL) {
			safe_abort();
		}
		strcat(cwd, "/");	// NOLINT(runtime/printf)
		memory += strlen(cwd) + 1;
		char* prev_memory = memory;
	
		int stackOffset = trace[2] == trace[1]? 2 : 1;
		for (int i = stackOffset; i < trace_size; i++) {
			memory = prev_memory;
			char *line;
			Dl_info dlinf;
			if (dladdr(trace[i], &dlinf) == 0 || dlinf.dli_fname[0] != '/' ||
					!strcmp(name_buf, dlinf.dli_fname)) {
				line = addr2line(name_buf, trace[i], color_outputImpl(), &memory);
			} else {
				line = addr2line(dlinf.dli_fname, reinterpret_cast<void *>(
						reinterpret_cast<char *>(trace[i]) -
						reinterpret_cast<char *>(dlinf.dli_fbase)),
						color_outputImpl(), &memory);
			}
	
			char *function_name_end = strstr(line, "\n");
			if (function_name_end != NULL) {
				*function_name_end = 0;
				{
					// "\033[34;1m[%s]\033[0m \033[33;1m(%i)\033[0m\n
					char* msg = memory;
					const int msg_max_length = 512;				
					if (color_outputImpl()) {
						strcpy(msg, "\033[34;1m");	// NOLINT(runtime/printf)
					} else {
						msg[0] = 0;
					}
					strcat(msg, "[");	// NOLINT(runtime/printf)
					strcat(msg, line);	// NOLINT(runtime/printf)
					strcat(msg, "]");	// NOLINT(runtime/printf)
					if (append_pidImpl()) {
						if (color_outputImpl()) {
							strcat(msg, "\033[0m\033[33;1m");	// NOLINT(runtime/printf)
						}
						strcat(msg, " (");	// NOLINT(runtime/printf)
						strcat(msg, Safe::itoa(getppid(), msg + msg_max_length));	// NOLINT(*)
						strcat(msg, ")");	// NOLINT(runtime/printf)
						if (color_outputImpl()) {
							strcat(msg, "\033[0m");	// NOLINT(runtime/printf)
						}
						strcat(msg, "\n");	// NOLINT(runtime/printf)
					} else {
						if (color_outputImpl()) {
							strcat(msg, "\033[0m");	// NOLINT(runtime/printf)
						}
						strcat(msg, "\n");	// NOLINT(runtime/printf)
					}
					Safe::print2stderr(msg);
				}
				line = function_name_end + 1;
	
				// Remove the common path root
				if (cut_common_path_rootImpl()) {
					int cpi;
					for (cpi = 0; cwd[cpi] == line[cpi]; cpi++) {};
					if (line[cpi - 1] != '/') {
						for (; line[cpi - 1] != '/'; cpi--) {};
					}
					if (cpi > 1) {
						line = line + cpi;
					}
				}
	
				// Remove relative path root
				if (cut_relative_pathsImpl()) {
					char *path_cut_pos = strstr(line, "../");
					if (path_cut_pos != NULL) {
						path_cut_pos += 3;
						while (!strncmp(path_cut_pos, "../", 3)) {
							path_cut_pos += 3;
						}
						line = path_cut_pos;
					}
				}
	
				// Mark line number
				if (color_outputImpl()) {
					char* number_pos = strstr(line, ":");
					if (number_pos != NULL) {
						char* line_number = memory;	// 128
						strcpy(line_number, number_pos);	// NOLINT(runtime/printf)
						// Overwrite the new line char
						line_number[strlen(line_number) - 1] = 0;
						// \033[32;1m%s\033[0m\n
						strcpy(number_pos, "\033[32;1m");	// NOLINT(runtime/printf)
						strcat(line, line_number);	// NOLINT(runtime/printf)
						strcat(line, "\033[0m\n");	// NOLINT(runtime/printf)
					}
				}
			}
	
			// Overwrite the new line char
			line[strlen(line) - 1] = 0;
	
			// Append pid
			if (append_pidImpl()) {
				// %s\033[33;1m(%i)\033[0m\n
				strcat(line, " ");	// NOLINT(runtime/printf)
				if (color_outputImpl()) {
					strcat(line, "\033[33;1m");	// NOLINT(runtime/printf)
				}
				strcat(line, "(");	// NOLINT(runtime/printf)
				strcat(line, Safe::itoa(getppid(), memory));	// NOLINT(runtime/printf)
				strcat(line, ")");	// NOLINT(runtime/printf)
				if (color_outputImpl()) {
					strcat(line, "\033[0m");	// NOLINT(runtime/printf)
				}
			}
	
			strcat(line, "\n");	// NOLINT(runtime/printf)
			Safe::print2stderr(line);
		}
	
		// Write '\0' to indicate the end of the output
		char end = '\0';
		write(STDERR_FILENO, &end, 1);
		
		if (thread_safeImpl()) {
			// Resume the parent process
			kill(getppid(), SIGCONT);
		}
	
		// This is called in the child process
		_Exit(EXIT_SUCCESS);
	}

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#pragma GCC diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif

	inline static void* MallocHook(size_t size, const void* /* caller */) {
		char* malloc_buffer = memoryImpl() + kNeededMemoryImpl() - 512;
		if (size > 512U) {
			const char* msg = "malloc() replacement function should not return "
					"a memory block larger than 512 bytes\n";
			Safe::print2stderr(msg, strlen(msg) + 1);
			_Exit(EXIT_FAILURE);
		}
		return malloc_buffer;
	}

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

};

}
