
/*
    SGI STL的第一种分配器：std::allocator
    2个函数：allocate、deallocate
    分别是对operator new、operator delete的包装

    和原始STL标准规定的分配器接口函数，几乎就是一样的。
    当然，是我自己的理解，看侯捷的书，也是这样的。

    测试：略了。
*/

#ifndef _SGI_STD_ALLOCATOR_
#define _SGI_STD_ALLOCATOR_

#include <new>  // for placement new
#include <cstddef>// for ptrdiff_t
#include <cstdlib> // for exit
#include <climits> // for UNIX_MAX
#include <iostream> // for cerr

namespace wrwSTL
{
    template<class T>
    inline T* allocate(size_t n, T*) {
        void* p = ::operator new(n * sizeof(T));
        if (p == 0) {
            cerr << "out of memory.\n";
            exit(0);
        }
        return p;
    }
    template<class T>
    inline void deallocate(T* ptr) {
        ::operator delete(ptr);
    }
    template<class T, class S>
    inline void construct(T* ptr, const S& val) {
        new (ptr) T(val);
    }
    template<class T>
    inline void destroy(T* ptr) {
        ptr->~T();
    }

    template<class T, class S>
    class sgi_std_allocator
    {
    public:
        void construct(T* ptr, const S& val) {
            _construct(ptr, val);
        }
        void destroy(T* ptr) {
            _destroy(ptr);
        }
        void deallocate(T* ptr) {
            _deallocate(ptr);
        }
        T* allocate(size_t n, const void* hint = 0) {
            _allocate(n, (T*)0);
        }
    };
}
#endif