#pragma once
 
#include <memory>


// Interesting topics here:
// 1. empty class optimisation for deleter
// 


template <typename T, typename D = std::default_delete<T>>
class my_unique_ptr : private D
{
    T* ptr_;
public:
    explicit my_unique_ptr(T* ptr) noexcept:ptr_(ptr)
    {
    }
    
    my_unique_ptr(T* ptr, D deleter) noexcept:ptr_(ptr), D(std::move(deleter))
    {
    }
    
    my_unique_ptr(const my_unique_ptr& un_ptr) = delete;
    my_unique_ptr& operator=(const my_unique_ptr& un_ptr) = delete;
    
    my_unique_ptr(my_unique_ptr&& un_ptr) noexcept: ptr_(un_ptr.ptr_)
    {
        un_ptr.ptr_ = nullptr;
    }
    void operator=(my_unique_ptr&& un_ptr) noexcept
    {
        ptr_ = un_ptr.ptr_;
        un_ptr.ptr_ = nullptr;
    }
    T* get()
    {
        return ptr_;
    }

    T operator*()
    {
        return *ptr_;
    }

    void reset(T* ptr = nullptr)
    {
        delete ptr_; 
        ptr_ = ptr;
    }

    T* release()
    {
        T* res = ptr_;
        ptr_ = nullptr;
        return res;
    }

    // void reset()
    // {
    //     delete ptr_;
    // }

    ~my_unique_ptr() noexcept
    {
        delete ptr_;
    }
};
 