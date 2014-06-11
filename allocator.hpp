#pragma once

#include <stdio.h>
#include <stddef.h>
#include <limits>
#include <memory>
#include <stdexcept>

#define BASE_ALLOCATOR_TYPES(T) \
typedef size_t size_type; \
typedef ptrdiff_t difference_type; \
typedef T* pointer; \
typedef const T* const_pointer; \
typedef T& reference; \
typedef const T& const_reference; \
typedef T value_type \

namespace sweet {
	template<typename T>
	class BaseAllocator {
	  public:
		BASE_ALLOCATOR_TYPES(T);

		template<typename U, typename... Args>
		void construct(U* p, Args&&... args) {
			::new((void*)p) U(std::forward<Args>(args)...);
		}

		inline void destroy(pointer p) {
			p->~T();
		}
	};

	template<typename T>
	class Mallocator : public BaseAllocator<T> {
	  public:
		BASE_ALLOCATOR_TYPES(T);

		inline pointer address(reference r) const noexcept {
			return &r;
		}

		inline const_pointer address(const_reference r) const noexcept {
			return &r;
		}

		inline pointer allocate(size_type n, const void* =0) {
			auto ptr = malloc(n * sizeof(T));
			if(ptr == nullptr) {
				throw std::bad_alloc();
			}

			return reinterpret_cast<pointer>(ptr);
		}

		inline void deallocate(pointer ptr, size_type) {
			free(ptr);
		}

		inline size_type max_size() const {
			return std::numeric_limits<size_type>::max();
		}
	};

	template<typename T>
	class FailAllocator : public BaseAllocator<T> {
	  public:
		BASE_ALLOCATOR_TYPES(T);

		inline pointer address(reference) const noexcept {
			return nullptr;
		}

		inline const_pointer address(const_reference) const noexcept {
			return nullptr;
		}

		inline pointer allocate(size_type, const void* =0) {
			return nullptr;
		}

		inline void deallocate(pointer, size_type) {
		}

		inline size_type max_size() const {
			return 0u;
		}
	};

	template<typename T, typename Primary, typename Secondary>
	class FallbackAllocator : BaseAllocator<T> {
	  private:
		  Primary primary;
		  Secondary secondary;

	  public:
		BASE_ALLOCATOR_TYPES(T);

		inline pointer address(reference r) const noexcept {
			pointer p = primary.address(r);
			return p != nullptr ? p : secondary.address(r);
		}

		inline const_pointer address(const_reference r) const noexcept {
			pointer p = primary.address(r);
			return p != nullptr ? p : secondary.address(r);
		}

		inline pointer allocate(size_type n, const void* hint=0) {
			pointer ptr;
			try {
				ptr = primary.allocate(n,hint);
			} catch(...) {
				ptr = nullptr;
			}

			if(ptr == nullptr) {
				ptr = secondary.allocate(n,hint);
			}

			if(ptr == nullptr) {
				throw std::bad_alloc();
			}

			return reinterpret_cast<pointer>(ptr);
		}

		inline void deallocate(pointer ptr, size_type size) {
			if(primary.address(*ptr) != nullptr) {
				primary.deallocate(ptr, size);
			} else {
				secondary.deallocate(ptr, size);
			}
		}

		inline size_type max_size() const {
			return std::max(primary.max_size(), secondary.max_size());
		}
	};

	template<typename T, size_t BumpSize>
	class BumpAllocator : public BaseAllocator<T> {
	  private:
		  T bump[BumpSize];
		  size_t idx;
	  public:
		BASE_ALLOCATOR_TYPES(T);

		inline BumpAllocator() : idx(0) {}

		inline pointer address(reference r) const noexcept {
			return &r;
		}

		inline const_pointer address(const_reference r) const noexcept {
			return &r;
		}

		inline pointer allocate(size_type s, const void* =0) {
			if(this->idx + s >= BumpSize) {
				throw std::bad_alloc();
			}

		}

		inline void deallocate(pointer, size_type) {
		}

		inline size_type max_size() const {
			return 0u;
		}
	};
}
