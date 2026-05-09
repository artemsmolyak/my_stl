cmake_minimum_required(VERSION 3.10)

project(my_vector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(my_vector
    utils.h
    utils.cpp
    my_vector.h
    my_vector_tests.h
    my_vector_tests.cpp
    main.cpp
)

