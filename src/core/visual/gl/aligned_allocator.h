

#ifndef __ALIGNED_ALLOCATOR_H__
#define __ALIGNED_ALLOCATOR_H__

#if defined(ANDROID) || defined(LINUX) || defined(__APPLE__)

#else
#include <intrin.h>
#endif
#if !defined(__APPLE__)
#include <malloc.h>		// _aligned_malloc and _aligned_free
#else
//#include <stdlib.h> //for macos aligned_alloc
#endif
#include <memory>		// std::allocator

#if (defined( _M_IX86 ) || defined( _M_X64 )) && !defined(__MINGW32__)
// STL allocator
template< class T, int TAlign = 16 >
struct aligned_allocator : public std::allocator<T>
{
	static const int ALIGN_SIZE = TAlign;
	template <class U> struct rebind    { typedef aligned_allocator<U, TAlign> other; };
	aligned_allocator() throw() {}
	aligned_allocator(const aligned_allocator&) throw () {}
	template <class U> aligned_allocator(const aligned_allocator<U, TAlign>&) throw() {}
	template <class U> aligned_allocator& operator=(const aligned_allocator<U, TAlign>&) throw()  {}
	// allocate
	pointer allocate(size_type c, const void* hint = 0) {
		return static_cast<pointer>(_mm_malloc(sizeof(T)*c, TAlign));
	}
	// deallocate
	void deallocate(pointer p, size_type n) {
		_mm_free(p);
	}
};


#elif defined(ANDROID) || defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
//https://github.com/MegEngine/MegEngine/blob/b938b1cf3cd50cf46a746d416824bd0d3e70060a/dnn/src/common/cv/aligned_allocator.h

// STL allocator
template< class T, int TAlign = 16 >
struct aligned_allocator : public std::allocator<T>
{
	typedef T* pointer;
	typedef size_t size_type;

	static const int ALIGN_SIZE = TAlign;
	template <class U> struct rebind    { typedef aligned_allocator<U, TAlign> other; };
	aligned_allocator() throw() {}
	aligned_allocator(const aligned_allocator&) throw () {}
	template <class U> aligned_allocator(const aligned_allocator<U, TAlign>&) throw() {}
	template <class U> aligned_allocator& operator=(const aligned_allocator<U, TAlign>&) throw()  {}
	// allocate
	pointer allocate(size_type c, const void* hint = 0) {
#if defined(__APPLE__)
        //return static_cast<pointer>(aligned_alloc(TAlign, sizeof(T)*c));
        void *ptr = 0;
        if (0==posix_memalign(&ptr, TAlign, sizeof(T)*c))
            return static_cast<pointer>(ptr);
        else
            return static_cast<pointer>(0);
#elif defined(__MINGW32__)
        return static_cast<pointer>(_aligned_malloc(TAlign, sizeof(T)*c));
#else
		return static_cast<pointer>(memalign(TAlign, sizeof(T)*c));
#endif
	}
	// deallocate
	void deallocate(pointer p, size_type n) {
		free(p);
	}
};

#endif


#endif // __ALIGNED_ALLOCATOR_H__
