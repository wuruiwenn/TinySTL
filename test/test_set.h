#include"../impl/allocator/STL_SD/stl_sd_allocator_interface.h"
#include"../impl/allocator/SGI_STL/std_alloc/object_handler/construct.h"
#include"../impl/allocator/SGI_STL/std_allocator/std_allocator.h"

using namespace wrwSTL;
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
    std::ostream& operator<<(std::ostream& stream, const Entity& e) {
        stream << "{Entity:" << e.id << "}\n";
        return stream;
    }

    //测试方法
    inline void test_stl_sd_interface()
    {
        cout << "\n============stl_sd_interface==============\n";

        //内存分配
        my_stl_sd_allocator<Entity, int> giver;
        void* p = nullptr;
        Entity* e = giver.allocate(5);
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

    //测试std::allocator
    inline void test_std_allocator() {
        cout << "\n============std::allocator==============\n";
        //先分配内存
        Entity* e = allocator<Entity>::allocate();
        cout << "丨===> allocator<Entity>::allocate(),分配的内存的首地址 e = " << e << endl;

        Entity* e1 = allocator<Entity>::allocate(2);
        cout << "丨===> allocator<Entity>::allocate(2),分配的内存的首地址 e1 = " << e1 << endl;

        //再构建对象
        allocator<Entity>::construct(e, 12);
        cout << "丨===> allocator<Entity>::construct(e, 12), *e = " << *e;

        allocator<Entity>::construct(e1, 16);
        cout << "丨===> allocator<Entity>::construct(e1, 16), *e1 = " << *e1;

        allocator<Entity>::destroy(e);
        cout << "丨===> allocator<Entity>::destroy(e), *e = " << *e;
        allocator<Entity>::destroy(e1);
        cout << "丨===> allocator<Entity>::destroy(e1), *e1 = " << *e1;

        cout << "仅调用目标对象的析构函数，并不会把内容从内存上抹去，内存仍处于有效状态\n";

        cout << "真正释放内存的操作，只能是deallocate：\n";
        allocator<Entity>::deallocate(e);
        cout << "丨===> allocator<Entity>::deallocate(e), *e = " << *e;
        // allocator<Entity>::deallocate(e1);
        delete e1;
        cout << "丨===>  delete e1; , *e1 = " << *e1;
        cout << "原始C++环境下的delete，是delete operator，delete操作符,\n";
        cout << "delete底层就是调用operator delete函数，所以delete这里就相当于自己实现的 deallocate+destroy\n";

        // allocator<Entity>::deallocate(e);
        // cout << "丨===> allocator<Entity>::deallocate(e)：后*e =  " << *e;
        // allocator<Entity>::deallocate(e1);
        // cout << "丨===> allocator<Entity>::deallocate(e1)：后*e1 =  " << *e1;
    }

    inline void test_std_alloc_construct() {

    }
}