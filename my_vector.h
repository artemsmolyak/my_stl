#pragma once
#include "utils.h"

// Interesting topics here:
// 1. Exception safety in push_back
// 2. Allocator support
// 3. Self-assignment problem
// 4. Copy-and-swap idiom

// 5. small vector optimization
// 6. is_trivially_copyable optimisation

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

 

template <typename T, typename Allocator = my_alloc<T>>
class my_vector
{
    T* m_data; //8 byte
    size_t m_size; //8 byte
    size_t m_capacity; //8 byte

    //memory
    [[no_unique_address]] Allocator m_alloc; //! can be 8 bytes or 0 with no_unique_address  

public:
my_vector():
m_data(nullptr), m_size(0), m_capacity(0)
{}

my_vector(const my_vector& other):
m_size(other.m_size),
m_capacity(other.m_capacity),
m_alloc(other.m_alloc),
m_data(nullptr)
{
    m_data = m_alloc.allocate(m_capacity);
    size_t constructed = 0;
    try{        
        for(size_t i = 0; i < other.m_size; ++i)
        {
            new (m_data + i) T(other.m_data[i]);
            ++constructed;
        }
    }
    catch(...)
    {
        for(size_t i = 0; i < constructed; i++)
            m_data[i].~T();
            
        m_alloc.deallocate(m_data, m_capacity);
        throw;    
    } 
}

my_vector(my_vector&& other) noexcept:
m_size(std::exchange(other.m_size, 0)),
m_capacity(std::exchange(other.m_capacity, 0)),
m_alloc(std::move(other.m_alloc)),
m_data(std::exchange(other.m_data, nullptr))
{
    log("my_vector(my_vector&& other)");
}

~my_vector()
{
    for(size_t i = 0; i < m_size; ++i)
        m_data[i].~T();

    if (m_data)
        m_alloc.deallocate(m_data, m_capacity);
}


void reallocation(size_t new_capacity)
{
    T* tmp = nullptr;
    //size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
    size_t constructed = 0;
    try 
    {
        tmp = m_alloc.allocate(new_capacity); // can throw bad alloc
        for(int i = 0; i < m_size; ++i)
        { 
            new (tmp + i) T(std::move_if_noexcept(m_data[i])); //can throw T exception 
            ++constructed;
        } 
        }
    catch(...)
    {
        for(size_t i = 0; i < constructed; ++i)
            tmp[i].~T();

        m_alloc.deallocate(tmp, new_capacity);

        throw;
    }

    for(int i = 0; i < m_size; ++i)
    {
        m_data[i].~T();
    }

    m_alloc.deallocate(m_data, m_capacity); 

    m_data = tmp;
    m_capacity = new_capacity;
}



// =======>  exception safety <==========
// v.push_back(v[0]) - self assigmnent problem

void push_back(const T& temp_value)
{ 
    log("void push_back(const T& temp_value)");

    // for self assigmnent problem
    T value = temp_value;

    if (m_size >= m_capacity)
    {  
        reallocation((m_capacity == 0) ? 1 : m_capacity * 2);
    }

    new (m_data + m_size) T(value);
    ++m_size;  
}

void push_back(T&& value)
{
    log("push_back(T&& value)");

    if (m_size >= m_capacity)
    {
        reallocation((m_capacity == 0) ? 1 : m_capacity * 2);
    }    

    new (m_data + m_size) T(std::move_if_noexcept(value));
    ++m_size;
}

size_t size() const
{
    return m_size;
}
size_t capacity()
{
    return m_capacity;
}

bool empty() const
{
    return m_size == 0;
}



// =====> copy and swap idiom <=============
// if copy is down - current object isn't changed
void swap(my_vector& other)
{
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_alloc, other.m_alloc);
    std::swap(m_data, other.m_data);
}

my_vector& operator=(const my_vector& other)
{   
    my_vector(other).swap(*this); 
    return *this;
}

my_vector& operator=(my_vector&& other)
{   
    my_vector(std::move(other)).swap(*this); 
    return *this;
}

bool operator==(const T& other)
{
    return m_size == other.m_size && std::equal(begin(), end(), other.begin());
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

//============ basic servises ===============

void pop_back()
{
    --m_size;
}

T& front()
{
    return m_data[0];
}

T& back()
{
    return m_data[m_size - 1];
}

T& operator[](size_t i)
{
    return m_data[i];
}

void reserve(size_t capacity)
{
    if ((m_capacity == 0) || (capacity > m_capacity && m_size == 0))
    {
        m_data = m_alloc.allocate(capacity);
        m_capacity = capacity;
    }
    else if (m_size != 0) 
    { 
        reallocation(capacity);
    } 
}

};
