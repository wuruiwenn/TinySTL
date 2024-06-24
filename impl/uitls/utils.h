#pragma once
/*
    utils.h
    用于实现一些通用功能，
    如：完美转发forward，move
 */
#include <cstddef>
#include"../impl/allocator/SGI_STL/std_alloc/object_handler/type_traits.h"

namespace wrwSTL
{
    /*************实现 std::remove_reference****************/
    //type_traits.h中已经实现
    // template<class T>
    // struct remove_reference {
    //     using type = T;
    // };

    // template<class T>
    // struct remove_reference<T&> {
    //     using type = T;
    // };

    // template<class T>
    // struct remove_reference<T&&> {
    //     using type = T;
    // };

    /**************实现std::remove_const***************** */
    //type_traits.h中已经实现
    // template<class T>
    // struct remove_const {
    //     using type = T;
    // };
    // template<class T>
    // struct remove_const<const T> {
    //     using type = T;
    // };

    /******************实现std::forward(完美转发)************ */
    template<class T>
    T&& forward(typename wrwSTL::remove_reference<T>::type& arg) noexcept {
        return static_cast<T&&>(arg);
    }
} // namespace wrwSTL