
/*
    SGI STL 分配器：std::alloc，的，关于 <对象的处理>
    SGI STL 源码中， <对象的处理> 存于文件 <stl_construct.h>
    主要就2个函数：construct、destroy，负责目标对象的构建和析构
    construct：运用placement new机制
    destroy：2种类型，析构一个对象，析构迭代器指定区间内多个对象：
                判断析构trivial destructor，逐级调用
*/
#ifndef _CONSTRUCT_H
#define _CONSTRUCT_H

#include<new>
#include"type_traits.h"

namespace wrwSTL
{
    // ==================construct：构造对象==========================
    // placement new 机制
    template<class T, class S>
    inline void construct(T* ptr, const S& val) {
        new (ptr) T(val);
    }

    //construct的一些重载
    template<class T, class... ArgsType>
    inline void construct(T* ptr, ArgsType... args) {
        wrwSTL::construct(ptr, wrwSTL::forward<ArgsType>(args)...);
    }




    // ==================destroy：将对象析构==========================

    //类型-1：仅析构一个指针指向的对象
    template<class T>
    inline void destroy(T* ptr)
    {
        using has_trivial_destructor = typename _type_traits<T>::has_trivial_destructor;
        destroy_one(ptr, has_trivial_destructor());//判断是不是平凡析构，是的话，不用任何处理
    }

    template<class T>
    inline void destroy_one(T* ptr, _true_type) {}

    template<class T>
    inline void destroy_one(T* ptr, _false_type)
    {
        if (ptr != nullptr) {
            ptr->~T();
        }
    }

    //类型-2：析构迭代器指定区间内的多个对象
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last)
    {
        typedef typename _type_traits<ForwardIterator>::has_trivial_destructor has_trivial_destructor;
        // using is_POD_type = typename _type_traits<ForwardIterator>::is_POD_type;
        destroy_lst(first, last, has_trivial_destructor());
    }

    template<class ForwardIterator>
    inline void destroy_lst(ForwardIterator first, ForwardIterator last, _true_type) {
        //如果是目标对象是平凡析构，则啥也不需要做
    }
    template<class ForwardIterator>
    inline void destroy_lst(ForwardIterator first, ForwardIterator last, _false_type) {
        for (;first != last;++first) {
            destroy_one(&*first, _false_type());//非平凡析构，则一一执行析构函数
        }
    }
}

#endif