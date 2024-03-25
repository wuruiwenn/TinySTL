# C++实现SGI STL

## 1、SGI STL 空间分配器
1. STL标准规定的一个分配器所应该具备的接口函数：
`allocate`，`deallocate`，`construct`，`destroy`，因而可以自己实现一个简单的STL标准的分配器，代码`/impl/stl_sd_allocator.h`。但是这个并不好测试，因为测试是否编译通过，取决于编译器环境，比如在gcc编译器环境下编译，不能通过，因为gcc编译器采用了`SGI STL 分配器实现标准`，而SGI STL关于分配器，在原始STL标准分配器的接口上进行了部分扩展，所以自己写的分配器如果没有完全按照SGI STL所实现的接口来实现，则不能通过编译。
1. SGI STL 关于分配器，有2个实现，`std::allocator`和`std::alloc`

    - `std::allocator`：不常用，核心是2个函数，`allocate`，`deallocate`，是仅仅对`operator new`，`operator delete`的调用，没有内存分配方面的优化
    - `std::alloc`：SGI STL分配器的核心，也是SGI STL各容器默认使用的分配器。
      - 其核心包括：对象和内存处理分工合作(对象的处理：placement new、trivial destructor判断。内存的处理：双层配置器)
      ---
      - 对象的处理：construct(placement new机制)、destroy(逐级调用，trivial destructor判断)
      - 内存的处理：采用，双层配置器
        - 第一级配置器：malloc_alloc_template：4个函数：allocate(包括 oom_xx)、reallocate(包括 oom_xx)、deallocate(直接调用free)，set_new_handler机制；
        - 第二级配置器：default_alloc_template：union，free list，内存池
