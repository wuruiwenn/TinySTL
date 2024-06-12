
/*
    实现迭代器，迭代器最顶层的基类
    容器自身的迭代器，都是基于底层基类的迭代器实现的，继承
*/

namespace wrwSTL
{
    //顶层的5种迭代器
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    //5类迭代器源码
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

}