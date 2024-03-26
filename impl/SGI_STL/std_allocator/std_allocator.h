
/*
    SGI STL的第一种分配器：std::allocator
    2个函数：allocate、deallocate
    分别是对operator new、operator delete的包装

    和原始STL标准规定的分配器接口函数是一样的。
    当然，是我自己的理解，看侯捷的书，也是这样的，不过这不重要。
*/

#ifndef STD_ALLOCATOR
#define STD_ALLOCATOR

#include <new>  // for placement new
#include <cstddef>// for ptrdiff_t
#include <cstdlib> // for exit
#include <climits> // for UNIX_MAX
#include <iostream> // for cerr

namespace mystl
{
    template<class T>
    inline T* allocate(size_t n)
    {
        void* p = ::operator new(sizeof(T));
        if (p == 0)
        {
            cerr << "out of memory.\n";
            exit(0);
        }
        return p;
    }
    template<class T>
    inline void deallocate(T* ptr)
    {
        ::operator delete(ptr);
    }
    template<class T1, class T2>
    inline void construct(T1* ptr, const T2& val)
    {
        new (ptr) T1(val);
    }
    template<class T>
    inline void destroy(T* ptr)
    {
        ptr->~T();
    }

    template<class T>
    class allocator
    {
        typedef T* pointer;
    public:
        void construct(pointer ptr, const T& val)
        {
            _construct(ptr, val);
        }
        void destroy(pointer ptr)
        {
            _destroy(ptr);
        }
        void deallocate(pointer ptr)
        {
            _deallocate(ptr);
        }
        pointer allocate(size_t n, const void* hint = 0)
        {
            _allocate(n, (pointer)0);
        }
    };
}

#endif