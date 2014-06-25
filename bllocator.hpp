#pragma once

#include <stdio.h>
#include <stddef.h>
#include <limits>
#include <memory>

namespace sweet {
	class BaseAllocator {
	  public:
		template<typename U, typename... Args>
		void construct(U* p, Args&&... args) {
			::new((void*)p) U(std::forward<Args>(args)...);
		}

		template<typename U>
		void destroy(U* p) {
			p->~T();
		}

		virtual void* allocate(size_t) = 0;
		virtual void deallocate(void*, size_t=0) = 0;
	};

	class Mallocator : public BaseAllocator {
	  public:
		void* allocate(size_t s) override {
			return malloc(s);
		}

		void deallocate(void* ptr, size_t=0) override {
			free(ptr);
		}
	};

	template<typename T, typename Allo>
	class STLAllo {
		Allo allo;

	  public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;

		template<typename U>
		class rebind {typedef STLAllo<U,Allo> other;};

		inline pointer address(reference r) const noexcept {
			return &r;
		}

		inline const_pointer address(const_reference r) const noexcept {
			return &r;
		}

		inline pointer allocate(size_type n, const void* =0) {
			auto ptr = allo.allocate(n);
			if(ptr == nullptr) {
				throw std::bad_alloc();
			}

			return reinterpret_cast<pointer>(ptr);
		}

		inline void deallocate(pointer ptr, size_type s) {
			this->allo.deallocate(ptr, s);
		}

		inline size_type max_size() const {
			return this->allo.max();
		}
	};
}
