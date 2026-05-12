#pragma once

template <typename T>
class my_alloc
{
    public:
    T* allocate(size_t size)
    {
        return static_cast<T*>(operator new(size * sizeof(T)));
    }

    void deallocate(T* ptr, size_t size)
    {
        operator delete(ptr);
    }
};