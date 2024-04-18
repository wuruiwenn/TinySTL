
#include"std_allocator.h"

namespace wrwSTL
{
    template<class T>
    T* allocator<T>::allocate(size_t n, T*) {
        T* p = (T*)(::operator new(n * sizeof(T)));
        if (p == 0)
        {
            std::cerr << "out of memory.\n";
            exit(1);
        }
        return p;
    }

    template<class T>
    void allocator<T>::deallocate(T* ptr) {
        ::operator delete(ptr);
    }

    template<class T>
    void allocator<T>::construct(T* ptr, const T& val) {
        return wrwSTL::construct(ptr, val);
    }

    template<class T>
    void allocator<T>::destroy(T* ptr) {
        wrwSTL::destroy(ptr);
    }

    //下次从这里开始...这上面好像有问题
    //1.为什么内部调用construct.h中的函数，不用加模板<T>
    //比如：construct<T>(ptr,val)；？

    //2、为什么这里destroy ctrl+鼠标左键，会跳到2个地方？
}