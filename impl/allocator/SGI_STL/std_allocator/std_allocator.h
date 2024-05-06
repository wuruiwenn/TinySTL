
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
    后来我参考了 Github 其他小伙伴对于类似的 TinySTL 的实现，发现对于 std::allocator
    的实现，是调用了 construct.h，因为 construct.h 在侯捷书里面其实是 std::alloc
    分配器中的，关于对象的处理，的内容，所以我就觉得很奇怪。
    所以，实际应该确实如此，std::allocator 的实现也是基于construct.h的
    即关于<对象的构造>的内容。

    以下是以header-only形式实现std::allocator的，即没有使用 .h和.cpp文件配对的方法
    本质上来说，这样会产生重定义问题，因为这里类方法是在类外用类作用域符号::实现的
    所以我加了 inline，避免重定义问题
    （实际也不会产生重定义问题，因为只会在test中include当前文件，为了规范，还是加了。）

    已测试。
*/

#ifndef _SGI_STD_ALLOCATOR_
#define _SGI_STD_ALLOCATOR_

#include <new>  // for placement new
#include <cstddef>// for ptrdiff_t
#include <cstdlib> // for exit
#include <climits> // for UNIX_MAX
#include <iostream> // for cerr

// 要引入 construct.h，用来构造/析构 对象
#include"../std_alloc/object_handler/construct.h"

namespace wrwSTL
{
    //类 std::allocator 的声明
    template<class T>
    class allocator
    {
    public:
        static T* allocate();//分胚一个目标对象的内存
        static T* allocate(size_t n);//分配指定个数个目标对象的内存
        static void deallocate(T* ptr);

        static void construct(T* ptr, const T& val);
        static void destroy(T* ptr);
        static void destroy(T* first, T* last);
    };

    //类 std::allocator 的定义(实现)
    template<class T>
    inline T* allocator<T>::allocate() {
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<class T>
    inline T* allocator<T>::allocate(size_t n) {
        if (n == 0) {
            return nullptr;
        }
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<class T>
    inline void allocator<T>::deallocate(T* ptr) {
        ::operator delete(ptr);
    }

    template<class T>
    inline void allocator<T>::construct(T* ptr, const T& val) {
        // new(ptr) T(val);
        // wrwSTL::construct<T, T>(ptr, val);
        wrwSTL::construct(ptr, val);
        //经过测试，这里加不加模板都没问题，应该是会默认给加上的
    }

    template<class T>
    inline void allocator<T>::destroy(T* ptr) {
        wrwSTL::destroy<T>(ptr);
        // wrwSTL::destroy(ptr);
    }

    template<class T>
    inline void allocator<T>::destroy(T* first, T* last) {
        wrwSTL::destroy(first, last);
    }
}
#endif
