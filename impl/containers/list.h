
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
        typedef node<T>* node_ptr;
        // using node_ptr = node<T>*;
    };

    template<class T>
    struct base_T_traits {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
    };

    /*
        List的节点结构：Node
    */
    template<class T>
    struct node {
    private:
        using node_ptr = node_traits<T>::node_ptr;
        using value_type = base_T_traits<T>::value_type;
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

    /*
        List
    */
    template<class T>
    class list
    {
    private://list的成员
        node_ptr head;
        node_ptr tail;
        size_type size;

    private: //traits
        typedef typename node_traits<T>::node_ptr       node_ptr;
        typedef typename base_T_traits<T>::value_type   value_type;
        // typedef int size_type;
    private:
        typedef list_iterator<T>                iterator;//list_iterator
        typedef default_allocator<T>            allocator_type;
        typedef default_allocator<node<T>>      node_allocator; //用于list中节点的空间分配器

    private://构建list对象的辅助函数
        /*
            创建一个节点
            根据传入的值，创建一个新节点，返回指向该节点的指针
            显然要调用 分配器STL alloc 进行内存分配+节点对象构建
        */
        template<class... ArgsType>
        node_ptr create_node(ArgsType&&... args);

        // 用 n 个元素初始化list容器
        void fill_init(int n, const value_type& v);

        //在某节点后创建k个节点，节点内容为v
        void link_after_node(node_ptr node, int k, const value_type& v);




    public://构造函数
        list() {}

        // 用 n 个元素初始化容器
        list(int n, const value_type& v) //list<int> list(5,9);
        {
            fill_init(n, v);
        }
    public:
        node_ptr push_back(const value_type& val) {
            //create_node
            //push
        }
    };

    /*************************list的一些辅助函数***************************** */

    //创建节点
    //意味着需要调用分配器进行内存分配、对象构建
    template<class T>
    template<class ...ArgsType>
    typename list<T>::node_ptr list<T>::create_node(ArgsType&&... args)
    {
        auto p = default_allocator::allocate(1);//内存分配，1个节点对象
        //下次从这里开始，在地址p上用construct构建对象
        default_allocator::construct(p, args);//对象构建
    }



    // 用 n 个元素初始化list容器
    // 显然要调用 分配器alloc进行内存分配和node节点对象构建
    template<class T>
    void list<T>::fill_init(int n, const value_type& v)
    {
        head = default_allocator::allocate(1);//用分配器分配内存，而不是new Node()
        link_after_node(head, n - 1, v);
        size = n;
    }

    //在某节点后创建k个节点
    template<class T>
    void list<T>::link_after_node(node_ptr node, int k, const value_type& v)
    {
        // auto p = create_node(v);

        //更新tail指向最后一个节点
    }


}




