#pragma once

#include "my_alloc.h"


// Interesting topics here:
// 1. control block
// 2. multithreading guarantee
//

template<typename T>
struct my_constrol_block
{
    my_constrol_block(T* p):
    ptr(p)
    {}
    
    std::atomic<int> counter = 0;
    std::atomic<int> weak_counter = 0;
    T* ptr;
};

template<typename T, typename Allocator = My_alloc<T>>
class my_shared_ptr
{
public:
    my_constrol_block<T>* ctlBlock;
    
private:
    explicit my_shared_ptr(ControlBlock<T>* ptr):
    ctlBlock(ptr)
    {}
    
public:
    static my_shared_ptr<T> CreateMySharedFromControlBlock(my_constrol_block<T>* ctlB)
    {
        return my_shared_ptr<T>(ctlB);
    }
    
 // ctors   
    my_shared_ptr()
    :ctlBlock(nullptr)
    {}
    
    my_shared_ptr(std::nullptr_t) noexcept
    :ctlBlock(nullptr)
    {}
    
    my_shared_ptr(T* ptr) noexcept
    :ctlBlock(nullptr)
    {
        if (!ptr)
            return;
      
        ctlBlock = new ControlBlock(ptr);

        ctlBlock->counter.fetch_add(1, std::memory_order_release); 
    }
 
    my_shared_ptr(const my_shared_ptr& obj) noexcept
    :ctlBlock(obj.ctlBlock)
    {
        if (obj.ctlBlock)
            ctlBlock->counter.fetch_add(1, std::memory_order_relaxed);
    }
    
    void operator=(const my_shared_ptr& obj) = delete;
    my_shared_ptr(my_shared_ptr&& obj) = delete;
    
    void operator=(my_shared_ptr&& obj) noexcept
    {
        if (this == &obj)
            return;
        
        this->~my_shared_ptr();
        
        ctlBlock = obj.ctlBlock;
        obj.ctlBlock = nullptr;
    }
    
    ~my_shared_ptr() noexcept
    {
        if (!ctlBlock)
            return;
        
        if (ctlBlock->counter.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            delete ctlBlock->ptr;
            
            if (ctlBlock->weak_counter.load(std::memory_order_acquire) == 0)
            {
                 delete ctlBlock;
            }
        }
    }

// ==========================    
    
    explicit operator bool()
    {
        if (ctlBlock && ctlBlock->ptr)
            return true;
        
        return false;
    }
    
    T& operator*() const
    {
        return *ctlBlock->ptr;
    }
    
    void sayHello()
    {
        std::cout << "hello" << std::endl;
    }
};

  
template<typename T>
class MyWeakPtr
{
    ControlBlock<T>* ctlBlock;
    
public:
    MyWeakPtr(const MySharedPtr<T>& shared):
    ctlBlock(shared.ctlBlock)
    {
        ctlBlock->weak_counter.fetch_add(1, std::memory_order_release);
    }
    
    MyWeakPtr(std::nullptr_t) noexcept
        : ctlBlock(nullptr)
    {}
    
    MyWeakPtr(const MyWeakPtr& other) = delete;
    
    void operator=(const MyWeakPtr& obj)
    {
        if (this == &obj)
            return;
      
        this->~MyWeakPtr();
        ctlBlock = obj.ctlBlock;
        if(ctlBlock)
            ctlBlock->weak_counter.fetch_add(1, std::memory_order_release);
    }
    
    ~MyWeakPtr()
    {
        if (!ctlBlock)
            return;
        
        if (ctlBlock->weak_counter.fetch_sub(1, std::memory_order_release) == 1
            && ctlBlock->counter.load(std::memory_order_acquire) == 0)
        {
            delete ctlBlock;
        }
    }
    
    MySharedPtr<T> lock()
    {
        if (!ctlBlock)
            return nullptr;
        
        int expected = ctlBlock->counter.load(std::memory_order_acquire);
        while(expected != 0)
        {
            if (ctlBlock->counter.compare_exchange_weak(expected,
                                                        expected + 1,
                                                        std::memory_order_acquire,
                                                        std::memory_order_relaxed))
            {
                ctlBlock->weak_counter.fetch_add(1, std::memory_order_release);
                return MySharedPtr<T>::CreateMySharedFromControlBlock(ctlBlock);
            }
        }
        return MySharedPtr<T>(nullptr);
    }
};