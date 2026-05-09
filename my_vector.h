#pragma once
#include "utils.h"

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


// small vector optimization? probably we will need __aligned__
// allocator_traits?

template <typename T, typename Allocator = my_alloc<T>>
class my_vector
{
    T* m_data; //8 byte
    size_t m_size; //8 byte
    size_t m_capacity; //8 byte

    //memory
    [[no_unique_address]] Allocator m_alloc; //! can be 8 bytes or 0 with no_unique_address  

public: 

// constructors 
my_vector():
m_data(nullptr), m_size(0), m_capacity(4)
{     
    m_data = m_alloc.allocate(m_capacity); 
}

my_vector(const my_vector& other):
m_size(other.m_size),
m_capacity(other.m_capacity),
m_alloc(other.m_alloc)
{  
    m_data = m_alloc.allocate(m_capacity);

    // exception?
    for(size_t i = 0; i < other.m_size; ++i)
    {
        m_data[i] = other.m_data[i];
    } 
}

my_vector(my_vector&& other):
m_size(std::move(other.m_size)),
m_capacity(std::move(other.m_capacity)),
m_alloc(std::move(other.m_alloc))
{
    m_data = m_alloc.allocate(m_capacity);

    //exception?
    for(size_t i = 0; i < other.m_size; ++i)
    {
        m_data[i] = std::move(other.m_data[i]);
    } 

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

void push_back(const T& value)
{ 
    if (m_size >= m_capacity)
    {
        m_capacity *= 2;
        T* tmp = static_cast<T*>(operator new (m_capacity * sizeof(T))); // can throw bad alloc
        for(int i = 0; i < m_size; ++i)
        {
            new (tmp + i) T(std::move(m_data[i])); 
            m_data[i].~T(); 
        } 
         
        m_alloc.deallocate(m_data, 0); 
        m_data = tmp;
    }

    new (m_data + m_size) T(value);
    ++m_size;  
}

void push_back(T&& value)
{
    if (m_size >= m_capacity)
    {
        m_capacity *= 2;
        T* tmp = static_cast<T*>(operator new (m_capacity * sizeof(T))); // can throw bad alloc
        for(int i = 0; i < m_size; ++i)
        {
            new (tmp + i) T(std::move(m_data[i])); // move constructor  
        } 
         
        m_alloc.deallocate(m_data, 0); 
        m_data = tmp;
    }    

    new (m_data + m_size) T(std::move(value));
    ++m_size;
}

size_t size() const
{
    return m_size;
}

bool empty() const
{
    return m_size == 0;
}

T& operator[](size_t i)
{
    return m_data[i];
}

// requrement for using iterators with STL algorithms
T* begin()
{
    return m_data;
}

T* end()
{
    return m_data + m_size;
}

//============

void pop_back()
{
    --m_size;
}


};
