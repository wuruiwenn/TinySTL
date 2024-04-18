
/*
    SGI STL的第一种分配器：std::allocator
    2个函数：allocate、deallocate
    分别是对operator new、operator delete的包装

    和原始STL标准规定的分配器接口函数，几乎就是一样的。
    当然，是我自己的理解，看侯捷的书，也是这样的。

    这里有个问题就是，我在看<侯捷的书>的时候，实际对于std::allocator的实现
    只给出了：allocate，deallocate，2个内存处理的函数，底层分别调用
    operator new / operator delete

    但是对于对象的处理，好像没有给出函数哇。
    后来我参考了Github其他小伙伴对于TinySTL的实现，发现对于std::allocator
    的实现，是调用了 construct.h，因为construct.h在侯捷书里面其实是std::alloc
    分配器中的内容，所以我就觉得很奇怪。
    所以，实际应该确实如此，std::allocator的实现也是基于construct.h的
    即关于<对象的构造>的内容。
*/

#ifndef _SGI_STD_ALLOCATOR_
#define _SGI_STD_ALLOCATOR_

#include <new>  // for placement new
#include <cstddef>// for ptrdiff_t
#include <cstdlib> // for exit
#include <climits> // for UNIX_MAX
#include <iostream> // for cerr

#include"../std_alloc/object_handler/construct.h"

namespace wrwSTL
{
    template<class T>
    class allocator
    {
    public:
        static T* allocate(size_t n, T*);
        static void deallocate(T* ptr);
        static void construct(T* ptr, const T& val);
        static void destroy(T* ptr);
        static void destroy(T* first, T* last);
    };
}


// namespace wrwSTL
// {
//     template<class T>
//     inline T* allocate(size_t n, T*) {
//         void* p = ::operator new(n * sizeof(T));
//         if (p == 0) {
//             cerr << "out of memory.\n";
//             exit(0);
//         }
//         return p;
//     }
//     template<class T>
//     inline void deallocate(T* ptr) {
//         ::operator delete(ptr);
//     }
//     template<class T, class S>
//     inline void construct(T* ptr, const S& val) {
//         new (ptr) T(val);
//     }
//     template<class T>
//     inline void destroy(T* ptr) {
//         ptr->~T();
//     }

//     template<class T, class S>
//     class sgi_std_allocator
//     {
//     public:
//         void construct(T* ptr, const S& val) {
//             _construct(ptr, val);
//         }
//         void destroy(T* ptr) {
//             _destroy(ptr);
//         }
//         void deallocate(T* ptr) {
//             _deallocate(ptr);
//         }
//         T* allocate(size_t n, const void* hint = 0) {
//             _allocate(n, (T*)0);
//         }
//     };
// }
#endif