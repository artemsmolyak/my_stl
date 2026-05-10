#include <cassert>
#include <iostream>

#include "my_vector_tests.h"
#include "my_vector.h"
 

#define RUN_TEST(name) \
std::cout << "===== " #name " =====" << std::endl; \
name();


int main() 
{
    RUN_TEST(test_reallocation);
    RUN_TEST(test_push_back);
    RUN_TEST(test_copy_constructor);
    RUN_TEST(test_move_constructor); 
    RUN_TEST(test_pop_back); 
    RUN_TEST(test_copy_assignment);
    RUN_TEST(test_move_assignment);
    RUN_TEST(test_push_back_self_reference_realloc_strong);

    RUN_TEST(test_strong_exception_safety);
    RUN_TEST(test_copy_constructor_exception_safety);

    log("All tests passed");

    return 0;
}