// http://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads/19659736#19659736

#include <mutex>
#include <condition_variable>

namespace sweet {
	class Semaphore{
  	  private:
		std::mutex mtx;
		std::condition_variable cv;
	    int count;
	
	  public:
	    inline Semaphore(int count_ = 0):count(count_){;}

	    inline void notify() {
			std::unique_lock<std::mutex> lck(mtx);
	        ++count;
	        cv.notify_one();
	    }

	    inline void wait() {
			std::unique_lock<std::mutex> lck(mtx);

			cv.wait(lck, [this]() {
				return count > 0; 
			});
	
	        count--;
	    }
	};
}
