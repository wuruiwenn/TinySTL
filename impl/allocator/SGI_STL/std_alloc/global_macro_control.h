# pragma once

// 进行类似于"组态设置"，就是进行一些宏定义，这些宏定义被其他文件用来控制程序块
// 比如 std::alloc中的双层配置器，就是通过宏USE_MALLOC来控制使用哪一个配置器

// #define _USE_MALLOC_