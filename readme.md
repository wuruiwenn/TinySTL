# C++实现SGI STL
## 1、SGI STL 空间分配器
1. STL标准规定的一个分配器所应该具备的接口函数：
`allocate`，`deallocate`，`construct`，`destroy`，因而可以自己实现一个简单的STL标准的分配器，代码`/impl/stl_sd_allocator.h`。但是这个并不好测试，因为测试是否编译通过，取决于编译器环境，比如在gcc编译器环境下编译，不能通过，因为gcc编译器采用了`SGI STL 分配器实现标准`，而SGI STL关于分配器，在原始STL标准分配器的接口上进行了部分扩展，所以自己写的分配器如果没有完全按照SGI STL所实现的接口来实现，则不能通过编译。
2. SGI STL 关于分配器，有2个实现，`std::allocator`和`std::alloc`

    - `std::allocator`：不常用，核心是2个函数，`allocate`，`deallocate`，是仅仅对`operator new`，`operator delete`的调用，没有内存分配方面的优化
    - `std::alloc`：SGI STL分配器的核心，也是SGI STL各容器默认使用的分配器。
      - 其核心包括：对象和内存处理分工合作(对象的处理：placement new、trivial destructor判断。内存的处理：双层配置器)
      ---
      - 对象的处理：construct(placement new机制)、destroy(逐级调用，trivial destructor判断)
      - 内存的处理：采用，双层配置器
        - 第一级配置器：malloc_alloc_template：4个函数：allocate(包括 oom_xx)、reallocate(包括 oom_xx)、deallocate(直接调用free)，set_new_handler机制；
        - 第二级配置器：default_alloc_template：union，free list(解决小额内存块分配形造成的碎片问题)，内存池
---
3. `std::alloc`分配器中的一些全局编程架构的方案
- `SGI STL`中对于`std::alloc`的双层配置器是使用 `USE_MALLO`宏来控制对外暴露哪一个的
  - 所以，我在文件 `global_control.h`中进行了这些全局宏的设定，相当于是一个用于接口暴露的控制
- 此外，在实现`std::alloc`后，我需要单独写一个文件`alloc_external_expose_interface.h`来实现一个 应用程序的接口，就是对实现的 `std::alloc`进行整合，给外部进行实际应用，在实际的`SGI STL`中的 `类simple_alloc`就是对应该文件的内容
  - 所以，`alloc_external_expose_interface.h`文件做2件事：
    - 根据全局控制`global_control.h` 中 `USE_MALLOC`宏，使用`typedef`，来配置全局唯一对外的 `alloc`是用的哪一级配置器
    - 使用 `class template`实现`simple_alloc`，使用模板参数的方式传入外部的实际`alloc`，对该`alloc`的内部方法，进一步封装一下
    - 那么，在实际应用时，比如 `class vector<class T,class Alloc>`的实现，就可以在文件内 `#include "alloc_external_expose_interface.h"`来对自定义实现的分配器的对外接口进行调用了
---