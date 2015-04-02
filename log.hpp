// LGPL3  Robert burner Schadek rburners@gmail.com
#pragma once

#include <limits>
#include <chrono>
#include <ctime>
#include <mutex>
#include <deque>
#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <sstream>

#include <format.hpp>
#include <semaphore.hpp>

namespace sweet {

#define LOG(LogLevel) LoggerSource(LogLevel, __LINE__, __FILE__, __PRETTY_FUNCTION__)

	inline std::string shortenString(const std::string& str) {
		size_t idx = str.rfind('/');
		if(idx == std::string::npos) {
			return str;
		} else {
			return str.substr(idx+1);
		}
	}

	struct LoggerPayload {
		const unsigned logLevel;
		const int line;
		const char* filename;
		const char* prettyFunc;
		std::chrono::time_point<std::chrono::system_clock> time;
		std::stringstream str;

		unsigned destructorCount;

		inline LoggerPayload(const unsigned logLevel_, const int line_, 
				const char* filename_, const char* prettyFunc_) : logLevel(logLevel_),
				line(line_), filename(filename_), prettyFunc(prettyFunc_),
				time(std::chrono::system_clock::now())
		{
		}
	};

	class LoggerDrain {
	  public:
		unsigned logLevel;
		std::function<void(std::ostream&,LoggerPayload&)> func;
		bool alwaysFlush;

		inline LoggerDrain() : logLevel(1), alwaysFlush(false), 
			output(&std::cerr), terminate(false) 
		{
			func = [&](std::ostream& out, const LoggerPayload& input) {
				auto tt = std::chrono::system_clock::to_time_t(input.time);
				char timeBuf[100];
				std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d_%H:%M:%S", 
					localtime(&tt));
				long milli = std::chrono::duration_cast<std::chrono::milliseconds>(
					input.time.time_since_epoch()).count();
				milli = milli % 10000;
				format(out, "%s.%04u %s:%d | %s\n", const_cast<const char*>(timeBuf), milli,
					shortenString(input.filename), input.line, input.str.str());
			};

			writerThread = std::thread([this] { this->writeElement(); });
		}

		inline ~LoggerDrain() {
			endLogger();
			writerThread.join();
		}

		inline void setOutput(std::unique_ptr<std::ostream> out) {
			outputPtr = std::move(out);
			output = outputPtr.get();
		}


		inline void endLogger() {
			terminate = true;
			consumerSem.notify();
		}

		inline void log(std::unique_ptr<LoggerPayload> payload) {
			std::lock_guard<std::mutex> insert(dequeMutex);
			payloads.emplace_back(std::move(payload));
			consumerSem.notify();
		}

		inline void writeElement() {
			while(true) {
				consumerSem.wait();
				dequeMutex.lock();
				if(terminate && payloads.size() == 0u) {
					break;
				}

				std::unique_ptr<LoggerPayload> p(std::move(payloads.front()));
				payloads.pop_front();

				if(terminate && payloads.size() == 0u) {
					dequeMutex.unlock();
					if(p->logLevel >= this->logLevel) {
						func(*output, *p);
						if(alwaysFlush) {
							output->flush();
						}
					}
					break;
				}
				dequeMutex.unlock();

				if(p->logLevel >= this->logLevel) {
					func(*output, *p);
					if(alwaysFlush) {
						output->flush();
					}
				}
			}
		}


	  private:
		std::ostream* output;
		bool terminate;
		std::mutex dequeMutex;
		sweet::Semaphore consumerSem;
		std::deque<std::unique_ptr<LoggerPayload>> payloads;
		std::thread writerThread;	
		std::unique_ptr<std::ostream> outputPtr;
	};

	inline LoggerDrain& getLoggerDrain() {
		static LoggerDrain logger;
		return logger;
	}

	class LoggerSource {
	public:
		inline LoggerSource(const unsigned logLevel, const int line,  const char* filename, 
			const char* prettyFunc) 
		{
			this->payload = std::make_unique<LoggerPayload>(logLevel, line, filename, 
					prettyFunc);
		}

		inline ~LoggerSource() {
			getLoggerDrain().log(std::move(payload));
		}

		template<typename... Args>
		LoggerSource& operator()(const char* form, Args... a) {
			format(payload->str, form, a...);
			return *this;
		}

		LoggerSource& operator()() {
			return *this;
		}

		template<typename T>
		LoggerSource& operator<<(const T& arg) {
			payload->str<<arg;
			return *this;
		}

		inline LoggerSource& operator<<(std::ostream&(*f)(std::ostream&) ) {
			if(f == static_cast<
				std::basic_ostream<char>& (*)(std::basic_ostream<char>&)>(std::endl))
			{
				payload->str<<std::endl;
			} else {
				payload->str<<f;
			}
			payload->str<<std::endl;
			return *this;
		}

	private:
		std::unique_ptr<LoggerPayload> payload;

	};
}
