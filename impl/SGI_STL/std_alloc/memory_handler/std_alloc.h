
/*
    std::alloc
    真正的核心，SGI STL 分配器的核心就是std::alloc，也是各大容器使用的默认分配器
    其核心内容如下：
    1、内存处理(construct、destroy)、对象处理分工合作
    2、关于 内存的处理，采用双层配置器：整体使用宏定义 USE_MALLOC 来配置全局使用哪一层配置器
        第一层配置器：malloc_alloc_template：4个函数，allocate，reallocate，deallocate，set_new_handler
        第二层配置器：default_alloc_template：整体上2个函数：allocate(包括refill)，deallocate。
                        union，free_list，内存池
*/

#ifndef STD_ALLOC
#define STD_ALLOC

namespace mystl
{

}


#endif
