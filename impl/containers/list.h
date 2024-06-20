
/*
    实现 STL 容器：list
    双向链表，一种序列式容器
*/

#include"../iterator/iterator.h"
#include"../exception/exceptdef.h"

namespace wrwSTL
{
    //一些前置操作，主要是traits

    //声明
    template<class T>
    struct list_node_base;

    template<class T>
    struct list_node;

    //node_traits
    //相当于一个工具类、中间件
    //专门用于节点类的类型萃取，而节点类是list的基础
    //例如，可以这样：typedef Node<T>* NodePtrType;
    //那么在class list中就可以进一步萃取：typedef typename node_traits<T>::NodePtrType _NodePtrType;
    template<class T>
    struct node_traits {
        typedef list_node_base<T>* base_ptr;
        typedef list_node<T>* node_ptr;
        //或者用 using，一样的，目的就是 定义 类型别名，实现traits
    };

    /*
        List的节点结构
    */
    template<class T>
    struct list_node_base
    {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        list_node_base* pre;
        list_node_base* next;

        list_node_base() = default;

        base_ptr self() {
            return static_cast<base_ptr>(&*this);
        }

        node_ptr as_node() {
            return static_cast<node_ptr>(self());
        }
    };

    template<class T>
    struct list_node :list_node_base<T> {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value;//节点的内容

        list_node(T&& v)
            :value(std::move(v))
        {}

        node_ptr self() {
            return static_cast<node_ptr>(&*this);
        }
        base_ptr as_base() {
            return static_cast<base_ptr>(&*this);
        }
    };

    /*
        list 的迭代器设计
        是一个可支持双向操作的迭代器
    */
    template<class T>
    struct list_iterator : public wrwSTL::iterator<wrwSTL::bidirectional_iterator_tag, T>
    {
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator<T> self;

        //构造函数
        list_iterator() = default;
        list_iterator(base_ptr nodePtr)
            :obj(nodePtr) {}

        //迭代器和目标对象关联的指针
        base_ptr obj;


        //重要操作符重载
        base_ptr operator->() {
            return obj;
        }

        reference operator*() {
            return obj->as_node()->value;
        }

        self& operator++() {
            wrwSTL_DEBUG(obj != nullptr);
            obj = obj->next;
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            ++(*this);
            return tmp;
        }

        self& operator--() {
            wrwSTL_DEBUG(obj != nullptr);
            obj = obj->pre;
            return *this;
        }

        self operator--(int) {
            selft tmp = *this;
            --(*this);
            return tmp;
        }

        //迭代器  == ，!=  比较操作符
        bool operator==(const self& other) {
            return obj == other.obj;
        }
        bool operator!=(const self& other) {
            return obj != other.obj;
        }
    };


    template<class T>
    class list
    {

    }
}




