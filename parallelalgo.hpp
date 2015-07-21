// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <algorithm>
#include <vector>
#include <thread>
#include <iterator>
#include <mutex>
#include <atomic>
#include <unistd.h>

namespace sweet {

inline unsigned getNumberOfCores() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}

template<typename Iterator, typename UnaryFunction>
UnaryFunction for_each_impl(Iterator first, Iterator second,
		UnaryFunction f, std::random_access_iterator_tag, size_t numThreads) {

	std::vector<std::thread> threads;
	const size_t dist = std::distance(first, second);
	const size_t stepWidth = dist/numThreads;

	for(size_t i = 0; i < numThreads; ++i) {
		int fillUp = (i+1 == numThreads) && (dist % 2 != 0) ? 1 : 0;
		threads.push_back(
			std::thread(
				std::for_each<Iterator, UnaryFunction>, 
				first+(i*stepWidth), first+((i+1)*stepWidth) + fillUp, f));
	}

	for(size_t i = 0; i < numThreads; ++i) {
		threads[i].join();
	}

	return f;
}

template<typename Iterator, typename UnaryFunction>
UnaryFunction for_each(Iterator first, Iterator second, UnaryFunction f,
		size_t numThreads = getNumberOfCores()) {
	typedef typename std::iterator_traits<Iterator>::iterator_category category;
	return for_each_impl(std::forward<Iterator>(first), 
		std::forward<Iterator>(second), std::forward<UnaryFunction>(f),
		category(), numThreads);
}

template<typename Iterator, typename OIterator, typename UnaryFunction>
OIterator transform_impl(Iterator first, Iterator second, OIterator out, 
		UnaryFunction f, size_t numThreads, std::random_access_iterator_tag, 
		std::random_access_iterator_tag) {
	std::vector<std::thread> threads;
	const size_t dist = std::distance(first, second);
	const size_t stepWidth = dist/2u;

	for(size_t i = 0; i < numThreads; ++i) {
		int fillUp = (i+1 == numThreads) && (dist % 2 != 0) ? 1 : 0;
		threads.push_back(
			std::thread(
				std::transform<Iterator, OIterator, UnaryFunction>,
				first+(i*stepWidth), first+((i+1)*stepWidth) + fillUp,
				out+(i*stepWidth), f
			)
		);
	}

	for(size_t i = 0; i < numThreads; ++i) {
		threads[i].join();
	}

	return out;
}

template<typename Iterator, typename OIterator, typename UnaryFunction>
OIterator transform(Iterator first, Iterator second, OIterator out, 
		UnaryFunction f, size_t numThreads = getNumberOfCores()) {
	typedef typename std::iterator_traits<Iterator>::iterator_category iType;
	typedef typename std::iterator_traits<OIterator>::iterator_category oType;

	return transform_impl(std::forward<Iterator>(first),
			std::forward<Iterator>(second), std::forward<OIterator>(out),
			std::forward<UnaryFunction>(f), numThreads, iType(), oType());
}

template<typename Iterator, typename OIterator, typename UnaryFunction>
UnaryFunction transform(Iterator first, Iterator second, OIterator oit,
		UnaryFunction f, size_t numThreads = 2) {
	typedef typename std::iterator_traits<Iterator>::iterator_category category;

	return transform_impl(std::forward<Iterator>(first), 
		std::forward<Iterator>(second), std::forward<OIterator>(oit),
		std::forward<UnaryFunction>(f), category(), numThreads);
}

template<typename Iterator, typename OIterator, typename Tmp, typename UnaryFunction>
OIterator mapReduce(Iterator first, Iterator second, OIterator out, 
		UnaryFunction f, size_t numThreads = getNumberOfCores()) {
	struct Job {
		Iterator b;
	    Iterator e;
		OIterator outIter;
		Tmp tmpStore;
		UnaryFunction func;
		std::mutex& jmutex;

		inline Job(Iterator nb, Iterator ne, OIterator nOutIter, UnaryFunction nFunc,
				std::mutex& nJmutex) : b(nb), e(ne), outIter(nOutIter),
				func(nFunc), jmutex(nJmutex) {
		}

		inline void operator()() {
			/*std::insert_iterator<Tmp> tmpOut = std::inserter(tmpStore,
					tmpStore.end());*/
			auto tmpOut = std::inserter(this->tmpStore, this->tmpStore.end());
			for(; this->b != this->e; ++this->b) {
				this->func(*this->b, tmpOut);	
			}

			this->jmutex.lock();
			std::copy(this->tmpStore.begin(), this->tmpStore.end(), this->outIter);
			this->jmutex.unlock();
		}
	};

	std::mutex joinMutex;

	size_t curSize = std::distance(first, second);
	size_t advanceSize = curSize/numThreads;
	if(advanceSize == 0) {
		++advanceSize;
	}

	Iterator cur = first;

	std::vector<std::thread> jobs;
	jobs.reserve(numThreads);

	for(size_t i = 0; i < numThreads && cur != second; ++i) {
		Iterator next = cur;
		std::advance(next, advanceSize);

		jobs.push_back(std::thread(Job(cur, next, out, f, joinMutex)));
		cur = next;
	}

	if(cur != second) {
		jobs.push_back(std::thread(Job(cur, second, out, f, joinMutex)));
	}

	for(auto& it : jobs) {
		it.join();
	}

	return out;
}

template<typename T>
struct FindEqual {
	const T& value;
	inline FindEqual(const T& v) : value(v) {}
	inline bool operator()(const T& other) {
		return this->value == other;
	}
};

template<typename Iterator, typename Unary>
Iterator find_if(Iterator begin, Iterator end, Unary pred, 
		size_t numThreads = getNumberOfCores()) 
{
	size_t curSize = std::distance(begin, end);
	size_t advanceSize = curSize/numThreads;
	if(advanceSize == 0) {
		++advanceSize;
	}

	std::atomic_bool die(false);
	std::vector<std::thread> jobs;
	jobs.reserve(numThreads);

	Iterator cur = begin;
	Iterator found = end;

	for(size_t i = 0; i < numThreads && cur != end; ++i) {
		Iterator next = cur;
		std::advance(next, advanceSize);

		jobs.push_back(std::thread([&](Iterator be, Iterator en) {
			for(; be != en && !die; ++be) {
				if(pred(*be)) {
					found = be;
					die = true;
					break;
				}
			}
		}, cur, next));
		cur = next;
	}

	for(auto& it : jobs) {
		it.join();
	}

	return found;
}

template<typename Iterator, typename T>
Iterator find(Iterator begin, Iterator end, const T& value, 
		size_t numThreads = getNumberOfCores()) 
{
	return find_if(begin, end, FindEqual<T>(value), numThreads);
}

} // namespace sweet
