#include <cassert>
#include <iostream>

#include "my_vector_tests.h"
#include "my_vector.h"
 

int main() {
    
    test_reallocation();
    test_push_back();
    test_copy_constructor();
    test_move_constructor(); 
    test_pop_back(); 

    log("All tests passed");

    return 0;
}