#ifndef SWEET_PARALLEL
#define SWEET_PARALLEL

#include <algorithm>
#include <vector>
#include <thread>

namespace css {

template<class Iterator, class UnaryFunction>
UnaryFunction for_each_impl(Iterator first, Iterator second,
		UnaryFunction f, std::random_access_iterator_tag, size_t numThreads) {

	std::vector<std::thread> threads;
	const size_t dist = std::distance(first, second);
	const size_t stepWidth = dist/2u;

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

template<class Iterator, class UnaryFunction>
UnaryFunction for_each(Iterator first, Iterator second, UnaryFunction f,
		size_t numThreads = 2) {
	typedef typename std::iterator_traits<Iterator>::iterator_category category;
	return for_each_impl(std::forward<Iterator>(first), 
		std::forward<Iterator>(second), std::forward<UnaryFunction>(f),
		category(), numThreads);
}

}

#endif
