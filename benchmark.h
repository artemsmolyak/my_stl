#pragma once

#include <string>
#include <iomanip> //std::setw

struct MyTestType
{
    int a;
    std::string s;
};

template<typename VectorType, typename Generator, typename... Args>
void benchmark_push_back(const std::string& name, size_t count, Generator gen, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();

    VectorType vec(std::forward<Args>(args)...);
    //vec.reserve(count);

    for(size_t i = 0; i < count; ++i)
    {
        vec.push_back(gen(i));
    }

    volatile size_t sink = vec.size(); // for compiler

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << std::left
          << std::setw(60)
          << (name + " push_back:")
          << duration.count()
          << " ms\n";
}

 
template <typename ListType, typename Generator>
void benchmark_push_front(const std::string& name, size_t count, Generator gen)
{
    auto start = std::chrono::high_resolution_clock::now();

    ListType list;

    for (size_t i = 0; i < count; ++i)
    {
        list.push_front(gen(i));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << std::left
        << std::setw(60)
        << (name + " push_front:")
        << duration.count() 
        << " ms\n";
}


 

 