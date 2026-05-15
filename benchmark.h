#pragma once

#include <string>

template<typename VectorType, typename Generator>
void benchmark_push_back(const std::string& name, size_t count, Generator gen)
{
    auto start = std::chrono::high_resolution_clock::now();

    VectorType vec;

    for(size_t i = 0; i < count; ++i)
    {
        vec.push_back(gen(i));
    }

    volatile size_t sink = vec.size(); // for compiler

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << name << " push_back: " << duration.count() << " ms \n";
}

struct MyTestType
{
    int a;
    std::string s;
};


 

 