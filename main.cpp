#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <forward_list>

#include "my_unique_ptr_tests.h"
#include "my_vector_tests.h"
#include "my_forward_list_tests.h"

#include "my_vector.h"
 
#include "benchmark.h"

#define RUN_TEST(name) \
std::cout << "===== " #name " =====" << std::endl; \
name();


int main() 
{
    // my vector
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

    //my queue ptr 
    RUN_TEST(my_unique_ptr_tests::test_basic_ownership); 
    RUN_TEST(my_unique_ptr_tests::test_move_constructor);
    RUN_TEST(my_unique_ptr_tests::test_move_assignment);
    RUN_TEST(my_unique_ptr_tests::test_reset);
    RUN_TEST(my_unique_ptr_tests::test_release);

    // my forward list
    RUN_TEST(my_forward_list_tests::test_empty);


    log("All tests passed\n\n");
    
    // benchmarks
    log("====== benchmarks vector <int> ======");
    constexpr size_t N = 500'000;

    {
        //ulimit -s 65532
        alignas(int) std::byte array[4 * N * sizeof(int)];
        SequentialBufferAllocator<int> buf(array, 4 * N);
        benchmark_push_back<std::vector<int, SequentialBufferAllocator<int> > >(
            "std::vector<int> + SequentialBufferAllocator", 
            N,
            [](size_t i) { return (int)i; },
            buf
        );
    }

    benchmark_push_back<std::vector<int>>(
        "std::vector<int>", N,
        [](size_t i) { return (int)i; }
    );

    {
        alignas(int) std::byte array[4 * N * sizeof(int)];
        SequentialBufferAllocator<int> buf(array, 4 * N);
        benchmark_push_back<my_vector<int, SequentialBufferAllocator<int> > >(
            "my_vector<int> + SequentialBufferAllocator", N,
            [](size_t i) { return (int)i; },
            buf
        );
    }

    benchmark_push_back<my_vector<int>>(
        "my_vector<int>", N,
        [](size_t i) { return (int)i; }
    );

    benchmark_push_back< my_vector<int, std::allocator<int> > >(
        "my_vector<int> + std::allocator", N,
        [](size_t i) { return (int)i; }
    );

    benchmark_push_back<my_vector<int, Arena_allocator<int> > >(
        "my_vector<int, my allocator>", N,
        [](size_t i) { return (int)i; }
    );

    log("\n");


    {
        alignas(std::string) std::byte array[4 * N * sizeof(sizeof(std::string))];
        SequentialBufferAllocator<std::string> buf(array, 4 * N);
        benchmark_push_back<std::vector<std::string, SequentialBufferAllocator<std::string> > >(
            "std::vector<string> + SequentialBufferAllocator", N,
            [](size_t i)
            {
                return std::string("hello_") + std::to_string(i);
            },
            buf
        );
    }

    benchmark_push_back<std::vector<std::string>>(
        "std::vector<string>", N,
        [](size_t i)
        {
            return std::string("hello_") + std::to_string(i);
        }
    );

    benchmark_push_back<my_vector<std::string>>(
        "my_vector<string>", N,
        [](size_t i)
        {
            return std::string("hello_") + std::to_string(i);
        }
    );

    benchmark_push_back<my_vector<std::string, std::allocator<std::string> > >(
        "my_vector<string> + std::allocator", N,
        [](size_t i)
        {
            return std::string("hello_") + std::to_string(i);
        }
    );

    benchmark_push_back< my_vector<std::string, Arena_allocator<std::string> > >(
        "my_vector<string> + arena allocator", N,
        [](size_t i)
        {
            return std::string("hello_") + std::to_string(i);
        }
    );
    log("\n");

    benchmark_push_back<std::vector<MyTestType>>(
        "std::vector my type",
        N,
        [](size_t i)
        {
            return MyTestType{
                (int)i,
                "data_" + std::to_string(i)
            };
        }
    );

    benchmark_push_back<my_vector<MyTestType>>(
        "my vector my type",
        N,
        [](size_t i)
        {
            return MyTestType{
                (int)i,
                "data_" + std::to_string(i)
            };
        }
    );

    benchmark_push_back<my_vector<MyTestType, std::allocator<MyTestType> > >(
        "my vector my type + std::allocator",
        N,
        [](size_t i)
        {
            return MyTestType{
                (int)i,
                "data_" + std::to_string(i)
            };
        }
    );

    benchmark_push_back< my_vector<MyTestType, Arena_allocator<MyTestType> > >(
        "my vector my type + arena allocator",
        N,
        [](size_t i)
        {
            return MyTestType{
                (int)i,
                "data_" + std::to_string(i)
            };
        }
    );

    log("\n====== benchmarks list ======");

     benchmark_push_front< my_forward_list<int> >(
        "my forward list <int>",
        N,
        [](size_t i) { return (int)i; }
    );
     
    benchmark_push_front< std::forward_list<int> >(
        "std::forward list",
        N,
        [](size_t i) { return (int)i; }
    );

    benchmark_push_front< my_forward_list<int, My_alloc<int> > >(
        "my forward list + my allocator",
        N,
        [](size_t i) { return (int)i; }
    );

    benchmark_push_front< my_forward_list<int, std::allocator<int> > >(
        "my forward list + std::allocator",
        N,
        [](size_t i) { return (int)i; }
    );

 
    return 0;
}