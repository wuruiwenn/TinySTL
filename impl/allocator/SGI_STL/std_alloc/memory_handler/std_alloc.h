
/*
    SGI STL 分配器：std::alloc，的，关于 <内存的处理>
    SGI STL 源码中， <内存的处理> 存于文件 <stl_alloc.h>

    关于 <内存的处理>，采用-双层配置器：
        整体使用宏定义 USE_MALLOC 来配置全局使用哪一层配置器
        （默认使用第二层配置器，更高效）

    第一层配置器：malloc_alloc_template：
        4个函数，allocate，reallocate，deallocate，set_new_handler
    第二层配置器：default_alloc_template：
        整体上2个函数：allocate(包括refill)，deallocate
                        union，free_list，内存池
*/

#pragma once

#include<new>
#include<cstddef> // for size_t
#include<cstdlib> // for free()、exit()
#include<iostream> // for cout、cerr

namespace wrwSTL
{
    /*第一层配置器
        4个函数：
        allocate、deallocate、reallocate、set_new_handler
    */
    class malloc_alloc_template
    {
    private:
        //处理内存不足情况的几个函数
        //源码中，没太懂它为什么这里要声明，而不是直接实现，显然是可以直接类内实现的，除了这个函数指针的初始化
        static void* oom_alloc(size_t n);
        static void* oom_realloc(void* loc, size_t n);
        //声明一个函数指针，该指针指向一个 返回值为void，无任何形参类型的函数
        //指针名字：malloc_alloc_oom_handler_ptr
        //类似C++  operator new函数：set_new_handler内存处理机制
        static void (*malloc_oom_handler_ptr)();
    public:
        //内存分配
        //本质调用底层：malloc()
        static void* allocate(size_t n) {
            void* ret = malloc(n);
            if (ret == 0) {//C++11之前，使用0或NULL来标识空指针
                ret = oom_alloc(n);
            }
            return ret;
        }
        //内存释放
        //本质调用底层：free()
        static void deallocate(void* p) {
            free(p);
        }
        //内存重分配
        //本质调用底层：realloc()
        //一般用于：在指定地址上扩展内存
        static void* reallocate(void* loc, size_t new_n) {
            void* ret = realloc(loc, new_n);
            if (ret == NULL) {
                ret = oom_realloc(loc, new_n);
            }
            return ret;
        }
    };

    //第一层配置器所需的oom_xxx()函数-类外部-具体实现
    // static void* oom_alloc(size_t n);
    // static void* oom_realloc(void* loc, size_t n);
    // static void (*malloc_alloc_oom_handler_ptr)();

    // 在类外部，初始化类内的这个函数指针
    // 类内普通成员函数的声明，就是函数指针，
    // 所以，在类外部初始化函数指针，和初始化成员函数是一样的
    // void (*malloc_alloc_template::malloc_alloc_oom_handler_ptr)() = NULL;
    // 初始化为空指针，即没有指向任何实体函数
    //客户端应该是可以在此指定具体的外部的oom处理函数
    inline void (*malloc_alloc_template::malloc_oom_handler_ptr)() = 0;

    //在类外部，初始化类内声明的成员函数oom_alloc
    inline void* malloc_alloc_template::oom_alloc(size_t n) {
        //再定义一个函数指针，来接收类内的malloc_alloc_oom_handler_ptr函数指针
        void(*my_malloc_handler)();
        void* ret;
        for (;;) {
            my_malloc_handler = malloc_oom_handler_ptr;
            if (my_malloc_handler == 0) {//如果外部没有指定oom处理程序，直接抛异常
                // throw std::bad_alloc;
                // __THROW_BAD_ALLOC;
                std::cout << "bad alloc Error.\n";
                exit(1);
            }
            my_malloc_handler();//通过函数指针 执行实际函数
            ret = malloc(n);//重新执行内存分配
            if (ret) {
                return ret;
            }
        }
    }

    //在类外部，初始化类内声明的成员函数oom_alloc
    // static void* oom_realloc(void* loc, size_t n);
    void* malloc_alloc_template::oom_realloc(void* loc, size_t n) {
        void(*my_malloc_handler)();
        void* ret;
        for (;;) {
            my_malloc_handler = malloc_oom_handler_ptr;
            if (my_malloc_handler == 0) {//如果外部没有指定oom处理程序，直接抛异常
                std::cout << "bad realloc Error.\n";
                exit(1);
            }
            my_malloc_handler();//执行oom处理程序
            ret = realloc(loc, n);//重新分配内存
            if (ret) {
                return ret;
            }
        }
    }



    /*
        第二层配置器
    */
    class default_alloc_template
    {
    private://需要的一些全局量
        enum { ALIGN = 8 };	//小型区块的上调边界
        enum { MAX_BYTES = 128 }; //小型区块的上限
        enum { NFREELISTS = MAX_BYTES / ALIGN }; //N个，即free-list个数
    private:
        //内存块节点的基本结构
        //每个内存块节点，都是一个union
        union obj {
            obj* next;
            char client_data[1];
        };
        // 整体是16个链表，每个链表用一个头结点obj来表达
        // 这里数组存的是每个链表的头结点的指针
        static obj* free_list[NFREELISTS];
    private:
        //将输入字节数bytes上调至8的倍数
        static size_t round_up(size_t b) {
            return (((b)+ALIGN - 1) & ~(ALIGN - 1));// 等价于：return ((b + ALIGN - 1) / ALIGN) * ALIGN;
        }
        //根据输入字节数bytes，返回应该使用16个free_list中的哪一个free_list
        // free_list[16] = [8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128]
        // 下标从0开始
        static size_t free_list_indx(size_t b) {
            return (round_up(b) / ALIGN) - 1;
        }
    public:
        static void* allocate(size_t n);//分配内存
        static void* refill(size_t n);
        static void deallocate(void* p, size_t n);//释放内存
        // static void* reallocate(void* p, size_t old_sz, size_t new_sz);//内存重新分配
    };

    //第二层配置器内部static函数，类外实现
    inline void* default_alloc_template::allocate(size_t n) {
        if (n > (size_t)MAX_BYTES) {
            return malloc_alloc_template::allocate(n);
        }
        //从free_list[]上取内存块
        size_t i = free_list_indx(n);
        obj* objHead = free_list[i];//free_list[]本身存的就是obj*，这样相当于把地址赋给左边
        if (objHead == 0) {//若free_list[i]上的链表头部没有可用内存块
            return refill(round_up(n));
        }
        //调整头结点
        //这里的objHead要么用二级指针，要么只能用free_list[i]来更新头结点
        //因为objHead是一个拷贝，更新objHead并不能实地更新free_list，你更新的是一个拷贝的变量
        //但是如果这个拷贝的变量void** objHead是一个二级指针，则可以用*objHead来更新头结点
        //原始的SGI STL这里的代码就是用二级指针
        free_list[i] = objHead->next;//free_list[i]就是此时的链表上的头结点(的地址)
        return objHead;
    }
    inline void default_alloc_template::deallocate(void* p, size_t n) {
        if (n > (size_t)MAX_BYTES) {
            malloc_alloc_template::deallocate(p);
            return;
        }
        //把p指向的内存块，接到free_list上适当位置，即回收内存
        //把节点插入链表的头部
        size_t i = free_list_indx(n);
        obj* pnew = (obj*)p;
        pnew->next = free_list[i];//free_list[i]就是当前的头结点(的指针)(obj*)
        free_list[i] = pnew;
    }



}



