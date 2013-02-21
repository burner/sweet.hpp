#ifndef SWEET_PARALLEL
#define SWEET_PARALLEL

#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

namespace css {

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
		size_t numThreads = 2) {
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
		UnaryFunction f, size_t numThreads = 2) {
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

} // namespace css

#endif
