
#include"test_set.h"
#include<vector>
using namespace test_collections;

int main()
{
    test_stl_sd_interface();//测试STL标准分配器接口函数
    test_std_allocator();//测试std::allocator
    std::vector<int> v;
}