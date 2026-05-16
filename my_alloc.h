#pragma once

// simple stateless allocator
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

// for scheme container <-> std::allocator_trats<T>  <-> your allocator
using propagate_on_container_copy_assignment = std::false_type;
using propagate_on_container_move_assignment = std::true_type; 

arena_allocator(const arena_allocator& other) = delete;
arena_allocator& operator=(const arena_allocator&) = delete;

arena_allocator(arena_allocator&& other):
m_memory(std::exchange(other.m_memory, nullptr)),
m_all_size(std::exchange(other.m_all_size, 0)),
m_cur_size(std::exchange(other.m_cur_size, 0))
{
}

arena_allocator()
:arena_allocator(DEFAULT_SIZE)
{
}

arena_allocator(size_t size):
m_all_size(size),
m_cur_size(0)
{
    m_memory = static_cast<T*>(operator new(size * sizeof(T))); // can throw bad alloc
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
    //skip deallocation for small allocations    
}

~arena_allocator()
{
    operator delete(m_memory);
}

};