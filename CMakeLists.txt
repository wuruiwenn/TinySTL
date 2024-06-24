



cmake_minimum_required(VERSION 3.22) #指定cmake的最低版本

# INCLUDE(CMakeForceCompiler) # 包含 CMakeForceCompiler 模块，该模块提供了强制指定编译器的功能。
# CMAKE_FORCE_C_COMPILER(gcc GNU)  # 强制指定C编译器为 gcc。参数 GNU 表示这是一个 GNU 风格的编译器。
# CMAKE_FORCE_CXX_COMPILER(g++ GNU) # 强制指定C++编译器为 g++
# SET(CMAKE_CXX_COMPILER /usr/bin/g++)

project(TinySTL) #项目名字

set(CXX STANDARD 11) #设置C++语言标准

# include_directories(impl test) # 用来指定头文件（.h）的搜索路径
# 附加包含目录，方便include，无需#include"../../"，只需#include"xxx.h"
include_directories(test) 


#将指定目录下的源文件（.cpp）列表，存放到一个变量中，
aux_source_directory(impl src_file_aux)
aux_source_directory(test src_file_2) 




# 使用指定的源文件来生成目标可执行文件exe
add_executable(tinySTL_EXE ${src_file_2} ${src_file_aux})
