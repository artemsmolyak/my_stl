cmake_minimum_required(VERSION 3.10)

project(my_stl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(my_stl
    utils.h
    utils.cpp
    my_vector.h
    my_unique_ptr.h
    my_forward_list.h

    my_vector_tests.h
    my_vector_tests.cpp
    my_unique_ptr_tests.h
    my_unique_ptr_tests.cpp
    my_forward_list_tests.h
    my_forward_list_tests.cpp

    benchmark.h

    main.cpp
)

