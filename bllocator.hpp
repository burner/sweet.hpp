#pragma once

#include <stdio.h>
#include <stddef.h>
#include <limits>
#include <algorithm>
#include <deque>
#include <memory>

namespace sweet {
	class BaseAllocator {
	  public:
		virtual void* allocate(size_t) = 0;
		virtual void deallocate(void*, size_t=0) = 0;
		virtual size_t max() const = 0;
		virtual bool allocated(void*) const = 0;
	};

	class Mallocator : public BaseAllocator {
	  public:
		inline void* allocate(size_t s) override {
			return malloc(s);
		}

		inline void deallocate(void* ptr, size_t=0) override {
			free(ptr);
		}

		inline size_t max() const override {
			return std::numeric_limits<size_t>::max();
		}

		inline bool allocated(void*) const override {
			return true;
		}
	};

	class FailAllocator : public BaseAllocator {
	  public:
		inline void* allocate(size_t) override {
			return nullptr;
		}

		inline void deallocate(void*, size_t) override {
		}

		inline size_t max() const override {
			return 0u;
		}

		inline bool allocated(void*) const override {
			return false;
		}
	};

	template<typename A1, typename A2>
	class FallbackAllocator : public BaseAllocator {
	  private:
		A1 a1;
		A2 a2;

	  public:
		inline void* allocate(size_t s) override {
			void* ptr = a1.allocate(s);
			if(ptr == nullptr) {
				ptr = a2.allocate(s);
			}

			if(ptr == nullptr) {
				throw std::bad_alloc();
			} else {
				return ptr;
			}
		}

		inline void deallocate(void* ptr, size_t s) override {
			if(a1.allocated(ptr)) {
				a1.deallocate(ptr, s);
			} else if(a2.allocated(ptr)) {
				a2.deallocate(ptr, s);
			} else {
				throw std::logic_error("Either of the two Allocated allocated "
					"the given ptr.");
			}
		}

		inline size_t max() const {
			return std::max(a1.max(), a2.max());
		}

		inline bool allocated(void* ptr) const override {
			if(a1.allocated(ptr)) {
				return true;
			} else if(a2.allocated(ptr)) {
				return true;
			} else {
				return false;
			}
		}
	};

	template<typename A>
	class FreeDequeAllocator : public BaseAllocator {
	  private:
		struct StoreNode {
			void* ptr;
			size_t size;

			inline StoreNode(void* p, size_t s) : ptr(p), size(s) {}
		};
		std::deque<StoreNode> store;

		A allocator;

	  public:
		inline ~FreeDequeAllocator() {
			std::for_each(this->store.begin(), this->store.end(), [&](const StoreNode& n) {
				allocator.deallocate(n.ptr, n.size);
			});
		}

		inline void* allocate(size_t size) override {
			auto it = std::find_if(store.begin(), store.end(), 
					[&](const typename std::deque<StoreNode>::value_type& s) {
				return s.size == size;
			});

			if(it != this->store.end()) {
				void* ptr = it->ptr;
				this->store.erase(it);
				return ptr;
			} else {
				return this->allocator.allocate(size);
			}
		}

		inline void deallocate(void* p, size_t s) override {
			this->store.push_back(StoreNode(p,s));
		}

		inline size_t max() const override {
			return this->allocator.max();
		}

		inline bool allocated(void* p) const override {
			return this->allocator.allocated(p);
		}
	};

	template<size_t Size = 4096>
	class StackAllocator : public BaseAllocator {
	  private:
		char stack[Size];
		size_t idx;

	  public:
		inline StackAllocator() : idx(0u) {}

		inline void* allocate(size_t size) override {
			size_t nIdx = this->idx + size;
			if(nIdx > Size) {
				return nullptr;
			} else {
				void* ret = reinterpret_cast<void*>(&this->stack[this->idx]);
				this->idx = nIdx;
				return ret;
			}
		}

		inline void deallocate(void* p, size_t s) override {
			char* a = static_cast<char*>(p) + s;
			char* b = &(this->stack[this->idx]);
			if(a == b) {
				this->idx -= s;
			}
		}

		inline size_t max() const override {
			return Size;
		}

		inline bool allocated(void* p) const override {
			return p >= &this->stack && p <= &this->stack[this->idx];
		}
	};

	template<size_t Size = 4096>
	class PoolAllocator : public BaseAllocator {
	  private:
		char* stack;
		size_t idx;

	  public:
		inline PoolAllocator() : stack(static_cast<char*>(malloc(Size))), idx(0u) {
			if(this->stack == nullptr) {
				throw std::bad_alloc();
			}
		}

		inline ~PoolAllocator() {
			free(this->stack);
		}

		inline void* allocate(size_t size) override {
			size_t nIdx = this->idx + size;
			if(nIdx > Size) {
				return nullptr;
			} else {
				void* ret = reinterpret_cast<void*>(&this->stack[this->idx]);
				this->idx = nIdx;
				return ret;
			}
		}

		inline void deallocate(void* p, size_t s) override {
			char* a = static_cast<char*>(p) + s;
			char* b = &(this->stack[this->idx]);
			if(a == b) {
				this->idx -= s;
			}
		}

		inline size_t max() const override {
			return Size;
		}

		inline bool allocated(void* p) const override {
			return p >= this->stack && p <= &this->stack[this->idx];
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

		inline STLAllo() {}

		template<typename U>
		struct rebind {typedef STLAllo<U,Allo> other;};

		inline pointer address(reference r) const noexcept {
			return &r;
		}

		inline const_pointer address(const_reference r) const noexcept {
			return &r;
		}

		inline pointer allocate(size_type n, const void* =0) {
			auto ptr = allo.allocate(sizeof(T) * n);
			if(ptr == nullptr) {
				throw std::bad_alloc();
			}

			return reinterpret_cast<pointer>(ptr);
		}

		inline void deallocate(pointer ptr, size_type s) {
			this->allo.deallocate(ptr, sizeof(T) * s);
		}

		inline size_type max_size() const {
			return this->allo.max();
		}

		template<typename... Args>
		void construct(pointer p, Args&&... args) {
			::new((void*)p) T(std::forward<Args>(args)...);
		}

		void destroy(pointer p) {
			p->~T();
		}

	};

	template <class T1, class T2, class A1, class A2>
	bool operator==(const STLAllo<T1,A1>&, const STLAllo<T2,A2>&) {
		return std::is_same<A1,A2>::value;
	}
 
	template <class T1, class T2, class A1, class A2>
	bool operator!=(const STLAllo<T1,A1>&, const STLAllo<T2,A2>&) {
		return !std::is_same<A1,A2>::value;
	}
}
