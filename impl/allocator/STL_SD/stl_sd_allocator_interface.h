
/*
    原始STL所规定的一个标准的分配器所应该具备的所有接口，
    这里只实现4个核心函数，即4个接口：
    allocate、deallocate、construct、destroy
*/


#ifndef _STL_SD_ALLOCATOR_INTERFACE_
#define _STL_SD_ALLOCATOR_INTERFACE_

#include <new>  // for placement new
#include <cstddef>// for ptrdiff_t
#include <cstdlib> // for exit
#include <climits> // for UNIX_MAX
#include <iostream> // for cerr
#include"../../uitls/utils.h"

namespace wrwSTL
{
    template<class T, class S>
    inline void _construct(T* p, const S& val)//对象构建
    {
        new (p) T(val);
    }
    template<class T, class S>//一些重载
    inline void _construct(T* p)
    {
        new(p) T();
    }
    template<class T, class... Args>
    inline void _construct(T* p, Args... args)
    {
        new(p) T(wrwSTL::forward<Args>(args)...);
    }

    template<class T>
    inline void _destroy(T* ptr)//对象销毁
    {
        ptr->~T();
    }

    template<class T>
    inline T* _allocate(size_t n, T*)//内存分配，这里也可以不指定参数的，C++允许
    {
        T* p = (T*)(::operator new(n * sizeof(T)));
        if (p == 0)
        {
            std::cout << "[Error]: out of memory.\n";
            exit(1);
        }
        return p;
    }
    template<class T>
    inline void _deallocate(T* ptr)//内存释放
    {
        ::operator delete(ptr);
    }

    //包裹一层，封装为一个class
    template<class T, class S>
    class my_stl_sd_allocator
    {
    public:
        T* allocate(size_t n, const void* hint = 0) {//内存分配，const void* hint = 0设置了默认值，是可选参数，调用时是否传入该参数都无所谓
            return _allocate(n, (T*)hint);
            //只有这里，比较特殊，想要的参数是参数型别，而不是参数
            //当然可以写和_allocate一模一样的参数，但是这样的话
            //_allocate()这里如何调用参数呢？所以不方便
            //这里的方案是，传入一个void*指针，且有默认值，外部可传可不传，
            // 然后强转为(T*)类型
        }
        void deallocate(T* ptr) {//内存释放
            _deallocate(ptr);
        }
        void construct(T* ptr, const S& val) {//对象构建
            _construct(ptr, val);
        }
        void destroy(T* ptr) {//对象销毁
            _destroy(ptr);
        }
    };
}

#endif