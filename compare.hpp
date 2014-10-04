#pragma once

#include <type_traits>
#include <int128.hpp>

namespace sweet {
	template<typename T, typename S, typename E = void>
	struct Comperator;

	template<typename T, typename S>
	struct ComperatorSameSign
	{
		typedef typename std::conditional<sizeof(T) >= sizeof(S), T, S>::type Bigger;

		static bool less(T t, S s) {
			return static_cast<Bigger>(t) < static_cast<Bigger>(s);
		}

		static bool lessEqual(T t, S s) {
			return static_cast<Bigger>(t) <= static_cast<Bigger>(s);
		}

		static bool equal(T t, S s) {
			return static_cast<Bigger>(t) == static_cast<Bigger>(s);
		}

		static bool notEqual(T t, S s) {
			return static_cast<Bigger>(t) != static_cast<Bigger>(s);
		}

		static bool greater(T t, S s) {
			return static_cast<Bigger>(t) > static_cast<Bigger>(s);
		}

		static bool greaterEqual(T t, S s) {
			return static_cast<Bigger>(t) >= static_cast<Bigger>(s);
		}
	};

	template<typename T, typename S>
	struct Comperator<T,S,
		typename std::enable_if<
			(std::is_unsigned<T>::value && std::is_unsigned<S>::value)
			|| (std::is_signed<T>::value && std::is_signed<S>::value)>
		::type> : ComperatorSameSign<T,S>
	{};

	template<typename T, typename S>
	struct Comperator<T,S,
		typename std::enable_if<
			is_sweet_int128<T>::value || is_sweet_int128<S>::value>
		::type>
	{
		static bool less(T t, S s) {
			return int128(t) < int128(s);
		}

		static bool lessEqual(T t, S s) {
			return int128(t) <= int128(s);
		}

		static bool equal(T t, S s) {
			return int128(t) == int128(s);
		}

		static bool notEqual(T t, S s) {
			return int128(t) != int128(s);
		}

		static bool greater(T t, S s) {
			return int128(t) > int128(s);
		}

		static bool greaterEqual(T t, S s) {
			return int128(t) >= int128(s);
		}
	};

	template<typename T, typename S>
	struct Comperator<T,S,
		typename std::enable_if<
			std::is_signed<T>::value && std::is_unsigned<S>::value>
		::type>
	{
		static bool less(T t, S s) {
			return (t < 0) ? true : static_cast<int64_t>(t) < static_cast<int64_t>(s);
		}

		static bool lessEqual(T t, S s) {
			return (t < 0) ? true : static_cast<int64_t>(t) <= static_cast<int64_t>(s);
		}

		static bool equal(T t, S s) {
			return (t < 0) ? false : static_cast<int64_t>(t) == static_cast<int64_t>(s);
		}

		static bool notEqual(T t, S s) {
			return (t < 0) ? true : static_cast<int64_t>(t) != static_cast<int64_t>(s);
		}

		static bool greater(T t, S s) {
			return (t < 0) ? false : static_cast<int64_t>(t) > static_cast<int64_t>(s);
		}

		static bool greaterEqual(T t, S s) {
			return (t < 0) ? false : static_cast<int64_t>(t) >= static_cast<int64_t>(s);
		}
	};

	template<typename T, typename S>
	struct Comperator<T,S,
		typename std::enable_if<
			std::is_unsigned<T>::value && std::is_signed<S>::value>
		::type>
	{
		static bool less(T t, S s) {
			return (s < 0) ? false : static_cast<int64_t>(t) < static_cast<int64_t>(s);
		}

		static bool lessEqual(T t, S s) {
			return (s < 0) ? false : static_cast<int64_t>(t) <= static_cast<int64_t>(s);
		}

		static bool equal(T t, S s) {
			return (s < 0) ? false : static_cast<int64_t>(t) == static_cast<int64_t>(s);
		}

		static bool notEqual(T t, S s) {
			return (s < 0) ? true : static_cast<int64_t>(t) != static_cast<int64_t>(s);
		}

		static bool greater(T t, S s) {
			return (s < 0) ? true : static_cast<int64_t>(t) > static_cast<int64_t>(s);
		}

		static bool greaterEqual(T t, S s) {
			return (s < 0) ? true : static_cast<int64_t>(t) >= static_cast<int64_t>(s);
		}
	};

	template<typename T, typename S>
	bool less(T t, S s) {
		return Comperator<T,S>::less(t,s);
	}

	template<typename T, typename S>
	bool lessEqual(T t, S s) {
		return Comperator<T,S>::lessEqual(t,s);
	}

	template<typename T, typename S>
	bool equal(T t, S s) {
		return Comperator<T,S>::equal(t,s);
	}

	template<typename T, typename S>
	bool notEqual(T t, S s) {
		return Comperator<T,S>::notEqual(t,s);
	}

	template<typename T, typename S>
	bool greater(T t, S s) {
		return Comperator<T,S>::greater(t,s);
	}

	template<typename T, typename S>
	bool greaterEqual(T t, S s) {
		return Comperator<T,S>::greaterEqual(t,s);
	}
}
