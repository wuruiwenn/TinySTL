
/*
    SGI STL 分配器：std::alloc，的，关于 <内存的处理>
    SGI STL 源码中， <内存的处理> 存于文件 <stl_alloc.h>

    关于 内存的处理，采用双层配置器：整体使用宏定义 USE_MALLOC 来配置全局使用哪一层配置器

    第一层配置器：malloc_alloc_template：
        4个函数，allocate，reallocate，deallocate，set_new_handler
    第二层配置器：default_alloc_template：
        整体上2个函数：allocate(包括refill)，deallocate。
                        union，free_list，内存池
*/

#ifndef _STD_ALLOC_H_
#define _STD_ALLOC_H_

namespace wrwSTL
{
    class alloc_1
    {

    };

    class alloc_2 {

    }
}


#endif














//以前写的：
// #ifndef STD_ALLOC
// #define STD_ALLOC


// #define __THROW_BAD_ALLOC throw bad_alloc

// #ifdef _USE_MALLOC
// typedef _malloc_alloc_template<0> malloc_alloc;
// typedef malloc_alloc alloc;
// #else
// typedef _default_alloc_template<0> alloc;
// #endif

// namespace mystl
// {
//     template<class T, class Alloc>
//     class simple_alloc
//     {
//     public:
//         static T* allocate(size_t n) {
//             return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
//         }
//         static T* allocate(void) {
//             return (T*)Alloc::allocate(sizeof(T));
//         }
//         static void deallocate(T* ptr, size_t n) {
//             if (0 != n)
//                 Alloc::deallocate(ptr, n * sizeof(T));
//         }
//         static void deallocate(T* ptr) {
//             Alloc::deallocate(ptr, sizeof(T));
//         }
//     };

//     //第二级配置器
//     template<int inst>
//     class _malloc_alloc_template {
//     private:
//         static void* oom_malloc(size_t);
//         static void* oom_realloc(void*, size_t);
//         static void (*_malloc_alloc_oom_handler)();
//     public:
//         static void allocate(size_t n) {
//             void* result = malloc(n);
//             if (0 == result)
//                 result = oom_malloc(n);
//             return result;
//         }
//         static void* deallocate(void* p, size_t) {
//             free(p);
//         }
//         static void* reallocate(void* p, size_t, size_t new_sz) {
//             void* result = realloc(p, new_sz);
//             if (0 == result)
//                 result = oom_realloc(p, new_sz);
//             return result;
//         }
//         static void(*set_malloc_handler(void(*f)()))(){
//             void(*old)() = _malloc_alloc_oom_handler;
//             _malloc_alloc_oom_handler = f;
//             return (old);
//         }

//     };

//     //该函数就是由客户端所指定的关于oom处理的set_new_handler处理函数
//     template<int inst>
//     void (*_malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = 0;

//     template<int inst>
//     void* _malloc_alloc_template<inst>::oom_malloc(size_t n) {
//         void(*my_malloc_handler)();//定义一个函数指针
//         void* result;
//         for (;;)
//         {
//             my_malloc_handler = _malloc_alloc_oom_handler;
//             if (my_malloc_handler == 0)//如果客户端没有指定oom的set_new_handler处理函数
//                 __THROW_BAD_ALLOC;//则直接抛内存分配失败的异常
//             (*my_malloc_handler)();//调用set_new_handler处理函数
//             result = malloc(n);//尝试重新分配内存
//             if (result)
//                 return (result);
//         }
//     }
//     template<int inst>
//     void* _malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
//         void(*my_malloc_handler)();//定义一个函数指针
//         void* result;
//         for (;;)
//         {
//             my_malloc_handler = _malloc_alloc_oom_handler;
//             if (0 == my_malloc_handler)
//                 __THROW_BAD_ALLOC;
//             (*my_malloc_handler)();
//             result = realloc(p, n);
//             if (result)
//                 return (result);
//         }
//     }


//     enum { __ALIGN = 8 };
//     enum { __MAX_BYTES = 128 };
//     enum { __NFREELISTS = __MAX_BYTES / ____ALIGN };
//     //第二级配置器
//     template<bool threads, int inst>
//     class _default_alloc_template {
//     private:
//         static size_t ROUND_UP(size_t bytes) {
//             return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
//         }
//     private:
//         union obj {
//             union obj* free_list_link;
//             char client_data[1];
//         }
//     private:
//         //16个free_list
//         static obj* volatile free_list[__NFREELISTS];
//         static void* refill(size_t n);

//         static char* chunk_alloc(size_t size, int* nobjs);
//     };
// }


// #endif
