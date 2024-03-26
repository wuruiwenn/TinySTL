
/*
    SGI STL 分配器：std::alloc关于 <对象的处理>
    SGI STL 源码中， <对象的处理> 存于文件 <stl_construct.h>
    主要就2个函数：construct、destroy，负责目标对象的构建和析构
    construct：运用placement new机制
    destroy：判断析构trivial destructor，逐级调用
*/
#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include<new>
#include"type_traits.h"
// ==================construct：构造对象==========================
template<class T1, class T2>
inline void construct(T1* ptr, const T2& val)
{
    new (ptr) T1(val);
}
// ==================construct==========================


// ==================destroy：将对象析构==========================

//类型-1：仅析构一个指针指向的对象
template<class T>
inline void destroy(T* ptr)//总体函数
{
    destroy_one(ptr, std::is_trivially_destructible<T>{});
    //当然这里可以用if来处理，只不过不够优雅
}

template<class T>
inline void destroy_one(T* ptr, std::true_type)//可选被调函数-1
{
}

template<class T>
inline void destroy_one(T* ptr, std::false_type)//可选被调函数-2
{
    if (ptr != nullptr)
    {
        ptr->~T();
    }
}


//类型-2：析构指定迭代器区间内的对象
template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)//总体函数
{
    destroy_lst(first, last, value_type(first));
}


template<class ForwardIterator>//可选被调函数-1
inline void destroy_lst(ForwardIterator first, ForwardIterator last, std::true_type)
{
}
template<class ForwardIterator>//可选被调函数-2
inline void destroy_lst(ForwardIterator first, ForwardIterator last, std::false_type)
{
    for (;first < last;++first)
    {
        //first是迭代器，迭代器指向对象就是容器内的元素，
        //destroy()需接收地址，所以要继续取地址
        destroy_one(&*first);
    }
}








// ==================destroy==========================

#endif