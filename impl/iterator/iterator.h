
/*
    实现迭代器，迭代器最顶层的基类
    容器自身的迭代器，都是基于底层基类的迭代器实现的，继承
*/

#include<stddef.h>

namespace wrwSTL
{
    /*
        ================================================================
            顶层的5种迭代器
        ================================================================
    */

    //仅仅是空壳，用于进一步实现具体的5种迭代器
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


    // iterator 基本模板
    template<class Category,
        class T,
        class Distance = ptrdiff_t,
        class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category	iterator_category;
        typedef T			value_type;
        typedef Distance	difference_type;
        typedef Pointer		pointer;
        typedef Reference	reference;
    };

    /*
        ================================================================
            迭代器 traits，相当于一种工具
        ================================================================
     */

     //对上述5种类型的迭代器，实现 类型萃取 


    //iterator_traits：是为了萃取出 迭代器 的特性
    //type_traits：是为了萃取出 类型 的特性
    template <class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;	//迭代器类型
        typedef typename Iterator::value_type        value_type;			// 迭代器所指对象的类型
        typedef typename Iterator::difference_type   difference_type;		// 两个迭代器之间的距离
        typedef typename Iterator::pointer           pointer;				// 迭代器所指对象的类型指针
        typedef typename Iterator::reference         reference;			// 迭代器所指对象的类型引用
    };

    /*
        上面的traits结构体并没有对原生指针做处理,
        所以还要为特化、偏特化版本 (即原生指针)做统一
    */
    // 针对原生指针 T* 生成的 traits 偏特化
    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef T* pointer;
        typedef T& reference;
    };
    // 针对原生指针 const T* 生成的 traits 偏特化
    template <class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef const T* pointer;
        typedef const T& reference;
    };

    /*****************************************************************************************/


    // 5类 迭代器 具体实现
    template <class T, class Distance>
    struct input_iterator
    {
        typedef input_iterator_tag	iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    struct output_iterator
    {
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;
    };

    template <class T, class Distance>
    struct forward_iterator
    {
        typedef forward_iterator_tag	iterator_category;
        typedef T						value_type;
        typedef Distance				difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    template <class T, class Distance>
    struct bidirectional_iterator
    {
        typedef bidirectional_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef Distance					difference_type;
        typedef T* pointer;
        typedef T& reference;
    };
    template <class T, class Distance>
    struct random_access_iterator
    {
        typedef random_access_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef Distance					difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    /*****************************************************************************************/
    /*
        ================================================================
            迭代器 重要操作：
                advance、distance
        ================================================================
    */

    /*
    advance：用于让迭代器前进 n 个距离
        自然，5种迭代器，各自都有个性化的 advance
        advance函数的实现，非常好地体现了 iterator traits的作用和意义，
        就如同type_traits一样
    */

    //最顶层的advance函数接口
    //也是对外开放的接口：advance(it,8)这样调用的，it是某个迭代器实例
    //它作为调用实际多个重载的advance函数的外部唯一接口，具体调用哪个advance，取决于 iterator traits结果
    template<class WhatIterator, class Distance>
    void advance(WhatIterator& iter, Distance n) {
        advance_dispatch(iter, n, iterator_category(WhatIterator));//参数3就是把迭代器的类型 萃取出来
    }

    //以下是按需被调用的几种advance
    //就是几个advance的重载

    //input_iterator_tag 的 advance
    template <class InputIterator, class Distance>
    void advance_dispatch(InputIterator& iter, Distance n, input_iterator_tag) {
        while (n--) {
            ++iter;
        }
    }

    //  bidirectional_iterator_tag 的 advance
    template<class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& iter, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            while (n--)
                ++iter;
        }
        else {
            while (n++)
                --iter;
        }
    }

    //  random_access_iterator_tag 的 advance
    template<class RandomIterator, class Distance>
    void advance_dispatch(RandomIterator& iter, Distance n) {
        iter += n;
    }

    /*
        distance：用于计算迭代器间的距离
    */

    //首先是最外层的接口函数:distance()
    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
    {
        return distance_dispatch(first, last, iterator_category(first));
    }

    //以下是按需被调用的distance

    // distance 的 input_iterator_tag 的版本
    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
        distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    // distance 的 random_access_iterator_tag 的版本
    template <class RandomIter>
    typename iterator_traits<RandomIter>::difference_type
        distance_dispatch(RandomIter first, RandomIter last,
            random_access_iterator_tag)
    {
        return last - first;
    }

    /*****************************************************************************************/

    /*
        萃取迭代器的特性：value_type，distance
    */

    //萃取某个迭代器的 value_type
    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator& iter)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // 萃取某个迭代器的 distance_type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator& iter)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 萃取某个迭代器的 category
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator& iter)
    {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }
}