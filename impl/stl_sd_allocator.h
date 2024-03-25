#pragma once

/*
    原始STL所规定的一个标准的分配器所应该具备的所有接口，
    4个核心函数，即4个接口：
    allocate、deallocate、construct、destroy
*/

#include <new>  // for placement new
#include <cstddef>// for ptrdiff_t
#include <cstdlib> // for exit
#include <climits> // for UNIX_MAX
#include <iostream> // for cerr


namespace my_sd_alloc
{
    template<class T1, class T2>
    inline void _construct(T1* ptr, const T2& val)
    {
        new (ptr) T1(val);//placement new
    }
    template<class T>
    inline void _destroy(T* ptr) // User* pr;
    {
        ptr->~T();
    }
    template<class T>
    T* _allocate(size_t n, T* hint)//书里面参数是(ptrdiff_t n,T*)，这里T*应该是忽略了参数的名字，C++允许这样的，因为这里我只需要类型计算sizeof即可，所以不需要指定名字
    {
        T* p = (T*)::operator new(size_t(n * sizeof(T)));//我觉得这里用 operator new还是malloc无所谓，operator new本质就是调用malloc，只是一层包裹
        if (p == 0)
        {
            std::cerr << "out of memory.\n";
            exit(1);
        }
        return p;
    }
    template<class T>
    inline void _deallocate(T* p)
    {
        ::operator delete(p);
    }

    //包裹一层，封装为一个class
    template<class T>
    class stl_sd_allocator
    {
        typedef T* pointer;

    public:
        void construct(pointer ptr, const T& val)
        {
            _construct(ptr, val);
        }
        void destroy(pointer ptr)
        {
            _destroy(ptr);
        }
        void deallocate(pointer ptr)
        {
            _deallocate(ptr);
        }
        pointer allocate(size_t n, const void* hint = 0)
        {
            _allocate(n, (pointer)0);
        }
    };
}
