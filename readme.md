# C++实现SGI STL

## 1、SGI STL 空间分配器
1. STL标准规定的一个分配器所应该具备的接口函数：
`allocate`，`deallocate`，`construct`，`destroy`，因而可以自己实现一个简单的STL标准的分配器，代码`/impl/stl_sd_allocator.h`
1. SGI STL 关于分配器，有2个实现，`std::allocator`和`std::alloc`

    - `std::allocator`不常用，
