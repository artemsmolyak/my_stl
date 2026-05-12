#pragma once

#include <iostream>
#include <cassert>

namespace my_unique_ptr_tests
{

struct Counter
{
    inline static int constructions = 0;
    inline static int destructions = 0;

    Counter() { ++constructions; }
    ~Counter() { ++destructions; }
};

 void test_basic_ownership();
 void test_move_constructor();
 void test_move_assignment();
 void test_reset();
 void test_release();
}