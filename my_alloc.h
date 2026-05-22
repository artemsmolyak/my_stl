#pragma once

// simple stateless allocator
template <typename T>
class My_alloc
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

    // not a necessary part of the allocator; std::allocator_traits handles it
    // void construct(T* p, const T& value);
    // void destroy(T* p);
};


// Arena-based allocator
template <typename T>
class Arena_allocator
{
    static constexpr size_t DEFAULT_SIZE = 50'000'000;

    T* m_memory;
    size_t m_all_size;
    size_t m_cur_size;

public: 

// TO DO for scheme container <-> std::allocator_trats<T>  <-> your allocator
using propagate_on_container_copy_assignment = std::false_type;
using propagate_on_container_move_assignment = std::true_type; 


Arena_allocator(const Arena_allocator& other) = delete;
Arena_allocator& operator=(const Arena_allocator&) = delete;

Arena_allocator(Arena_allocator&& other):
m_memory(std::exchange(other.m_memory, nullptr)),
m_all_size(std::exchange(other.m_all_size, 0)),
m_cur_size(std::exchange(other.m_cur_size, 0))
{
}

Arena_allocator()
:Arena_allocator(DEFAULT_SIZE)
{
}

Arena_allocator(size_t size):
m_all_size(size),
m_cur_size(0)
{
    m_memory = static_cast<T*>(operator new(size * sizeof(T)));
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

~Arena_allocator()
{
    operator delete(m_memory);
}

};



template <typename T>
class SequentialBufferAllocator
{
public:
    // stl requrements
    using value_type = T;
    using size_type = std::size_t;

private:    
    // can't use T* to avoid construction of T
    std::byte* m_buf;
    std::byte* m_curPointer;
    // size of T objects
    size_type m_size;

public:
    SequentialBufferAllocator(std::byte* ptr, size_type size) noexcept:
    m_buf(ptr),
    m_curPointer(ptr), 
    m_size(size)
    {
    }

    // We can't delete the copy constructor because STL containers need it
    // SequentialBufferAllocator(const SequentialBufferAllocator&) = delete;

    // size of T objects
    T* allocate(size_t size)
    {
        if (m_buf + m_size * sizeof(T) <= m_curPointer + size * sizeof(T))
            throw std::bad_alloc();

        auto cur = m_curPointer;
        m_curPointer += size * sizeof(T);
        return reinterpret_cast<T*>(cur);    
    }

    void deallocate(T* ptr, size_t size)
    {
    }
};