#pragma once

// std::alloc对外暴露的接口
// 就是std::alloc最终被外部应该如何使用呢？
// 这里就相当于定义一个供外部应用层使用的 应用接口
// 比如vector<>就是应用层：
// 实际C++ std::vector的实现文件内部肯定include了类似的暴露接口文件的

//对于每一个实现的"组件"，如：分配器，容器等，
//应该都各自定义一个唯一的 对外接口文件，供外部应用层进行使用

// STL中是在原始std::alloc外层包裹了一层class simple_alloc的方式，把真正的std::alloc实现源码隐藏起来了
// 这个simple_alloc就相当于一个对外暴露的接口，这个接口，才是真正被其他，比如vector容器内部，所使用的
// 因而，在class vector{}内部实现中，首先要 #include "alloc_external_expose_interface.h"

//引入全局的组态设置
#include "./global_macro_control.h" 

//引入实际的分配器实现
#include "./memory_handler/std_alloc.h" 

//引入实际的对象处理construct
#include "./construct.h"

#ifdef _USE_MALLOC_
typedef wrwSTL::malloc_alloc_template alloc;
#else
typedef wrwSTL::default_alloc_template alloc;
#endif

// 定义别名，也可以用using：
    // using alloc = wrwSTL::default_alloc_template;
    // using alloc = wrwSTL::malloc_alloc_template;

namespace wrwSTL
{
    //simple_alloc是如何接收外部实现的alloc的呢？

    // 如果simple_alloc被决定实现为一个 函数，
    // 则可以通过函数形参的方式传入alloc【这种最简单，在一般的C++编程中也用的多】

    // 如果simple_alloc被决定实现为一个 class，则只能使用模板T的方式传入alloc
    //【这种更加规范，利用template参数】
    //【template传入参数T，是一种类型】
    //【因为 函数是通过类型T调用，
    // 所以，外部的alloc的函数的实现只能是static成员、嵌套类型 这2种】
    //【这样的话，才能通过 T:: 来调用】

    // 若要通过实例化对象调用，可通过T t;实例化一个对象，然后 t.allocate()这样调用
    // 但内部函数都实现为static，显然更方便

    //【所以这里若要用class simple_alloc来接收外部的分配器的话，】
    //【则对应的双层配置器class的内部函数实现要定义为static函数才行】
    //【因为std::alloc的双层配置器都是class，
    // 所以实际SGI STL std::alloc中对外接口simple_alloc也只能用class template实现】
    //【从而通过template T把std::alloc传进来进行一层封装】

    template<class T, class Alloc>
    class simple_alloc
    {
    public:
        static T* allocate(size_t n) {
            return (n == 0) ? (0 : (T*)Alloc::allocate(n * sizeof(T)));
        }
        static T* allocate(void) {
            return (T*)Alloc::allocate(sizeof(T));
        }
        static void deallocate(T* ptr) {
            return Alloc::deallocate(ptr, sizeof(T));
        }
        static void deallocate(T* ptr, size_t n) {
            if (n != 0) {
                return Alloc::deallocate(ptr, n * sizeof(T));
            }
        }
    };

    /*
        定义一个默认的分配器，默认采用alloc作为内存分配器的分配器
        作为对外的一个默认分配器的接口
    */
    template<class T>
    class default_allocator {
    public:
        //内存处理
        static T* allocate(size_t n) {
            return (n == 0) ? (0 : (T*)alloc::allocate(n * sizeof(T)));
        }
        static T* allocate(void) {
            return (T*)alloc::allocate(sizeof(T));
        }
        static void deallocate(T* ptr) {
            return alloc::deallocate(ptr, sizeof(T));
        }
        static void deallocate(T* ptr, size_t n) {
            if (n != 0) {
                return alloc::deallocate(ptr, n * sizeof(T));
            }
        }
        // 对象的构建
        template<class ...ArgsType>
        static void construct(T* ptr, ArgsType&& ...args) {
            return wrwSTL::construct(ptr, args);
        }
        // 这个地方可能有问题，外部default_allocator需要传入2个参数，一个是关于内存分配的T
        //     一个是 对象构建的参数S，参数S可能还必须是一个 Args...可变参数
        //     比如是构建User对象，那需要传入 名字，年龄，地址等
    }
}