#pragma once

#include <iostream>
#include <cassert>


struct ThrowOnCopy
{
    inline static int copy_count = 0;
    inline static int fail_after = 100;

    inline static int constructions = 0;
    inline static int destructions = 0;

    int value;

    ThrowOnCopy(int v = 0) : value(v) 
    {
        ++constructions;
    }

    ThrowOnCopy(const ThrowOnCopy& other)
        : value(other.value)
    {
        copy_count += 1;
      
        if (copy_count > fail_after)
        { 
            throw std::runtime_error("copy failed");
        }

        ++constructions;
    }

    ThrowOnCopy(ThrowOnCopy&& other) = delete;
    ThrowOnCopy& operator=(const ThrowOnCopy& other) = delete;

    ~ThrowOnCopy()
    {  
        ++destructions;
    }
};
 

void test_reallocation();
void test_push_back();
void test_copy_constructor();
void test_move_constructor();
void test_pop_back();
void test_copy_assignment();
void test_move_assignment();
void test_push_back_self_reference_realloc_strong();

// with ThrowOnCopy
void test_strong_exception_safety();
void test_copy_constructor_exception_safety();