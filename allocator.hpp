// LGPL 3 or higher Robert Burner Schadek rburners@gmail.com
#pragma once

#include <stdio.h>
#include <stddef.h>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <deque>
#include <memory>
#include <stdexcept>

#include <conv.hpp>

#define BASE_ALLOCATOR_TYPES(NAME,T) \
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
		BASE_ALLOCATOR_TYPES(BaseAllocator,T);

		inline BaseAllocator() {}

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
		BASE_ALLOCATOR_TYPES(Mallocator,T);

		inline Mallocator() {}

		template<typename U>
		struct rebind {typedef Mallocator<U> other;};

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
		BASE_ALLOCATOR_TYPES(FailAllocator,T);

		inline FailAllocator() {}

		template<typename U>
		struct rebind {typedef FailAllocator<U> other;};

		template<class Z>
		using DefaultType = FailAllocator<Z>;

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

	template<typename T, template<typename> class Primary, template<typename> class Secondary>
	class FallbackAllocator : public Primary<T>, public Secondary<T> {
	  public:
		BASE_ALLOCATOR_TYPES(FallbackAllocator,T);
	  private:
		std::unordered_map<pointer,size_type> secMemory;
	  public:

		inline FallbackAllocator() {}

		template<typename U>
		struct rebind {
			typedef FallbackAllocator<U,Primary,Secondary> other;
		};

		inline pointer address(reference r) const noexcept {
			pointer p = Primary<T>::address(r);
			return p != nullptr ? p : Secondary<T>::address(r);
		}

		inline const_pointer address(const_reference r) const noexcept {
			pointer p = Primary<T>::address(r);
			return p != nullptr ? p : Secondary<T>::address(r);
		}

		inline pointer allocate(size_type n, const void* hint=0) {
			pointer ptr;
			ptr = Primary<T>::allocate(n,hint);

			if(ptr == nullptr) {
				ptr = Secondary<T>::allocate(n,hint);
				secMemory.insert(std::make_pair(ptr,n));
			}

			if(ptr == nullptr) {
				throw std::bad_alloc();
			}

			return reinterpret_cast<pointer>(ptr);
		}

		inline void deallocate(pointer ptr, size_type size) {
			auto it = this->secMemory.find(ptr);
			if(it != this->secMemory.end()) {
				Secondary<T>::deallocate(it->first, it->second);
				this->secMemory.erase(it);
			} else {
				Primary<T>::deallocate(ptr, size);
			}
		}

		inline size_type max_size() const {
			return std::max(Primary<T>::max_size(), Secondary<T>::max_size());
		}
	};


	template<typename T, size_t BumpSize = 4096>
	class BumpAllocator : public BaseAllocator<T> {
	  private:
		  T bump[BumpSize];
		  size_t idx;
	  public:
		BASE_ALLOCATOR_TYPES(BumpAllocator,T);

		template<class Z>
		using DefaultType = BumpAllocator<Z>;

		inline BumpAllocator() : idx(0) {}

		template<typename U>
		struct rebind {typedef BumpAllocator<U,BumpSize> other;};

		inline pointer address(reference r) const noexcept {
			return &r;
		}

		inline const_pointer address(const_reference r) const noexcept {
			return &r;
		}

		inline pointer allocate(size_type s, const void* =0) {
			if(this->idx + s >= BumpSize) {
				return nullptr;
			}

			auto ret = &(this->bump[this->idx]);
			this->idx += s;

			return ret;
		}

		inline void deallocate(pointer p, size_type s) {
			pointer a = p + s;
			pointer b = &(this->bump[this->idx]);
			if(a == b) {
				this->idx -= s;
			}
		}

		inline size_type max_size() const {
			return BumpSize * sizeof(T);
		}
	};

	template<typename T, typename A, size_t BumpSize = 4096>
	class SingleBlockAllocator : public BaseAllocator<T> {
	  private:
		  size_t idx;
		  T* base;
		  A allocator;
	  public:
		BASE_ALLOCATOR_TYPES(BumpAllocator,T);

		inline SingleBlockAllocator() : idx(0), base(nullptr) {}
		inline ~SingleBlockAllocator() {
			this->allocator.deallocate(base, BumpSize);
		}

		/*template<typename U>
		struct rebind {typedef SingleBlockAllocator<U,A,BumpSize> other;};
		*/

		inline pointer address(reference r) const noexcept {
			return &r;
		}

		inline const_pointer address(const_reference r) const noexcept {
			return &r;
		}

		inline pointer allocate(size_type s, const void* =0) {
			if(this->base == nullptr) {
				this->base = allocator.allocate(BumpSize);
			}
			if(this->idx + s >= BumpSize) {
				return nullptr;
			}

			auto ret = &(this->base[this->idx]);
			this->idx += s;

			return ret;
		}

		inline void deallocate(pointer p, size_type s) {
			pointer a = p + s;
			pointer b = &(this->base[this->idx]);
			if(a == b) {
				this->idx -= s;
			}
		}

		inline size_type max_size() const {
			return BumpSize * sizeof(T);
		}
	};

	template<typename T, typename Allocator>
	class FreeVectorAllocator : public BaseAllocator<T> {
	  public:
		BASE_ALLOCATOR_TYPES(BumpAllocator,T);

	  private:
		template<typename U>
		struct StoreNode {
			U* ptr;
			size_t size;

			inline StoreNode(U* p, size_type s) : ptr(p), size(s) {}
		};
		std::deque<StoreNode<T>> store;
		Allocator allocator;

	  public:
		inline FreeVectorAllocator() {}

		inline ~FreeVectorAllocator() {
			std::for_each(this->store.begin(), this->store.end(), [&](const StoreNode<T>& n) {
				allocator.deallocate(n.ptr, n.size);
			});
		}

		template<typename U>
		struct rebind {typedef FreeVectorAllocator<U, Allocator> other;};

		inline pointer address(reference r) const noexcept {
			return allocator.address(r);
		}

		inline const_pointer address(const_reference r) const noexcept {
			return allocator.address(r);
		}

		inline pointer allocate(size_type size, const void* h=0) {
			auto it = std::find_if(store.begin(), store.end(), 
					[&](const typename std::deque<StoreNode<T>>::value_type& s) {
				return s.size == size;
			});

			if(it != this->store.end()) {
				pointer ptr = it->ptr;
				this->store.erase(it);
				return ptr;
			} else {
				return this->allocator.allocate(size, h);
			}
		}

		inline void deallocate(pointer p, size_type s) {
			this->store.push_back(StoreNode<T>(p,s));
		}

		inline size_type max_size() const {
			return this->allocator.max_size();
		}
	};
}
