#include"../impl/allocator/STL_SD/stl_sd_allocator_interface.h"

using namespace mystl;
using std::cout;
using std::endl;

namespace test_collections
{
    class Entity //用于测试的类
    {
    private:
        int id;
    public:
        Entity(int _id) :id(_id) { cout << "Entity构造函数\n"; };
        ~Entity() { cout << "Entity析构\n"; };
        friend std::ostream& operator<<(std::ostream& stream, const Entity& e);
    };
    std::ostream& operator<<(std::ostream& stream, const Entity& e)
    {
        stream << "{Entity:" << e.id << "}\n";
        return stream;
    }

    //测试方法
    inline void test_stl_sd_interface()
    {
        //内存分配
        my_stl_sd_allocator<Entity, int> giver;
        void* p = nullptr;
        Entity* e = giver.allocate(5, p);
        cout << "丨===> allocate分配的内存的首地址 e = " << e << endl;

        //在这个地址上构建目标对象
        giver.construct(e, 5);
        cout << "丨===> 该地址上运用placement new机制-> construct构建的对象为：" << (*e);

        //把该对象销毁
        giver.destroy(e);
        cout << "丨===> 该地址上destroy对象后：" << (*e);

        //释放该内存块
        giver.deallocate(e);
        cout << "丨===> 该地址上deallocate释放内存后：" << (*e);
    }
}