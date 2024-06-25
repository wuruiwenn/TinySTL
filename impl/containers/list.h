
/*
    实现 STL 容器：list
    双向链表，一种序列式容器
*/

#include"../iterator/iterator.h"
#include"../exception/exceptdef.h"
#include"../allocator/SGI_STL/std_allocator/std_allocator.h" //第一种分配器
#include"../allocator/SGI_STL/std_alloc/alloc_external_expose_interface.h" //第二种分配器

namespace wrwSTL
{
    //一些前置操作，主要是traits

    //声明
    // template<class T>
    // struct list_node_base;

    // template<class T>
    // struct list_node;

    //我最终用于实现list的是这个node
    //按自己想法实现的
    template<class T>
    struct node;

    //node_traits
    //相当于一个工具类、中间件
    //专门用于节点类的类型萃取，而节点类是list的基础
    //例如，可以这样：typedef Node<T>* NodePtrType;
    //那么在class list中就可以进一步萃取：typedef typename node_traits<T>::NodePtrType _NodePtrType;
    template<class T>
    struct node_traits {
        // typedef list_node_base<T>* base_ptr;
        // typedef list_node<T>* node_ptr;
        //或者用 using，一样的，目的就是 定义 类型别名，实现traits
        // using value_type = T;
        // using node_ptr = node<T>*;
        typedef node<T>* node_ptr;
    };

    template<class T>
    struct base_t_traits {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
    };

    /*
        List的节点结构
    */
    // template<class T>
    // struct list_node_base
    // {
    //     typedef typename node_traits<T>::base_ptr base_ptr;
    //     typedef typename node_traits<T>::node_ptr node_ptr;

    //     list_node_base* pre;
    //     list_node_base* next;

    //     list_node_base() = default;

    //     base_ptr self() {
    //         return static_cast<base_ptr>(&*this);
    //     }

    //     node_ptr as_node() {
    //         return static_cast<node_ptr>(self());
    //     }
    // };

    // template<class T>
    // struct list_node :list_node_base<T> {
    //     typedef typename node_traits<T>::base_ptr base_ptr;
    //     typedef typename node_traits<T>::node_ptr node_ptr;

    //     T value;//节点的内容

    //     list_node(T&& v)
    //         :value(std::move(v))
    //     {}

    //     node_ptr self() {
    //         return static_cast<node_ptr>(&*this);
    //     }
    //     base_ptr as_base() {
    //         return static_cast<base_ptr>(&*this);
    //     }
    // };

    template<class T>
    struct node {
    private:
        using node_ptr = node_traits<T>::node_ptr;
        using value_type = base_t_traits<T>::value_type;
    public:
        value_type value;
        node_ptr prev;
        node_ptr next;
    };

    /*
        list 的迭代器设计
        是一个可支持双向操作的迭代器
    */
    template<class T>
    struct list_iterator : public wrwSTL::iterator<wrwSTL::bidirectional_iterator_tag, T>
    {
    private:
        typedef typename base_t_traits<T>::value_type value_type;
        typedef typename base_t_traits<T>::pointer pointer;
        typedef typename base_t_traits<T>::reference reference;

        typedef typename node_traits<T>::node_ptr node_ptr;

        typedef list_iterator<T> self;

    private:
        node_ptr obj; //迭代器和目标对象关联的指针
    public:
        //构造函数
        list_iterator() = default;
        list_iterator(node_ptr nodePtr)
            :obj(nodePtr) {}
    public:

        //重要操作符重载
        node_ptr operator->() {
            return obj;
        }
        reference operator*() {
            return obj->value;
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
            obj = obj->prev;
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


    // template<class T, class Alloc = alloc>
    template<class T>
    class list
    {
    private:
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef typename base_t_traits<T>::value_type value_type;
        typedef int size_type;
    private:
        typedef list_iterator<T> iterator;//list_iterator
        typedef default_allocator<T> allocator_type;
        typedef default_allocator<list_node<T>> node_allocator; //用于list中节点的空间分配器

    private://构建list对象的辅助函数
        // 用 n 个元素初始化list容器
        void fill_init(size_type n, const value_type& v);

        //在某节点后创建k个节点，节点内容为v
        void link_after_node(node_ptr node, size_type k, const value_type& v);

        //创建一个节点
        //显然要调用 分配器 进行内存分配+节点对象构建
        template<class ...ArgsType>
        void create_node(ArgsType&& ...args);

    private://list的成员
        node_ptr head;
        node_ptr tail;
        size_type size;
    public://构造函数
        list()
        {

        }
        // 用 n 个元素初始化容器
        list(size_type n, const value_type& v) //list<int> list(5,9);
        {
            fill_init(n, v);
        }
    public:
        node_ptr push_back(const value_type& val) {
            // if (head == nullptr) {
            //     //
            // }
            // node_ptr link_node = create_node(val);
            // ++size;
        }
    };

    /*************************list的一些辅助函数***************************** */

    // 用 n 个元素初始化list容器
    // 显然要调用 分配器alloc进行内存分配和node节点对象构建
    template<class T>
    void list<T>::fill_init(size_type n, const value_type& v)
    {
        head = default_allocator::allocate(1);//用分配器分配内存，而不是new Node()
        link_after_node(head, n - 1, v);
        size = n;
    }

    //在某节点后创建k个节点
    template<class T>
    void list<T>::link_after_node(node_ptr node, size_type k, const value_type& v)
    {
        auto p = create_node(v);

        //更新tail指向最后一个节点
    }

    //创建节点
    //意味着需要调用分配器进行内存分配、对象构建
    template<class T>
    template<class ...ArgsType>
    void list<T>::create_node(ArgsType&& ...args)
    {
        auto p = default_allocator::allocate(1);//内存分配
        default_allocator::construct(p, args);//对象构建
    }

}




