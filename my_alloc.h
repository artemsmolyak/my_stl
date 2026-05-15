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


template <typename T>
class arena_allocator
{
    T* m_memory;
    size_t m_all_size;
    size_t m_cur_size;

    static constexpr size_t DEFAULT_SIZE = 50'000'000;

public: 

arena_allocator()
:arena_allocator(DEFAULT_SIZE)
{
}

arena_allocator(size_t size)
{
    m_memory = static_cast<T*>(operator new(size * sizeof(T)));
    m_all_size = size;
    m_cur_size = 0;
}

T* allocate(size_t size)
{
    assert(size < (m_all_size - m_cur_size));

    auto* ptr = m_memory + m_cur_size; 

    m_cur_size += size;

    return ptr;
}

void deallocate(T* ptr, size_t size)
{
    
}

~arena_allocator()
{
    operator delete(m_memory);
}

};