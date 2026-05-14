#pragma once
#include "utils.h"
#include "my_alloc.h"

// Interesting topics here:
// 1. Exception safety in push_back
// 2. Allocator support
// 3. Self-assignment problem
// 4. Copy-and-swap idiom

// 5. TO DO small vector optimization
// 6. TO DO is_trivially_copyable optimisation
 

template <typename T, typename Allocator = my_alloc<T>>
class my_vector
{
public:
    using my_type = T;
    using iterator = T*;
    using const_iterator = const T*;

private:
    //memory
    [[no_unique_address]] Allocator m_alloc; //! can be 8 bytes or 0 with no_unique_address  

    T* m_data; //8 byte
    size_t m_size; //8 byte
    size_t m_capacity; //8 byte

public:
my_vector(const Allocator alloc = Allocator()):
m_data(nullptr), m_size(0), m_capacity(0)
{}

my_vector(const my_vector& other):
m_size(other.m_size),
m_capacity(other.m_capacity),
m_alloc(other.m_alloc),
m_data(m_alloc.allocate(other.m_capacity))
{
    // also can use std::uninialized_copy here
    auto current_ptr = begin();
    auto other_ptr = other.begin();
    try{        
        for(; current_ptr != end(); ++current_ptr, ++other_ptr)
        {
            new (current_ptr) T(*other_ptr);
        }
    }
    catch(...)
    {
        auto it = begin();
        for(; it != current_ptr; ++it)
            (*it).~T();
            
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
    T* tmp = m_alloc.allocate(new_capacity); // can throw bad alloc
    size_t constructed = 0;
    try 
    {   
        for(; constructed < m_size; ++constructed)
        { 
            new (tmp + constructed) T(std::move_if_noexcept(m_data[constructed])); //can throw T exception 
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

template <typename... Args> // set of types
void emplace_back(Args&&... args) // set of variables
{
    if (m_size >= m_capacity)
    {
        reallocation((m_capacity == 0) ? 1 : m_capacity * 2);
    }    

    new (m_data + m_size) T(std::forward<Args>(args)...);
    ++m_size;
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
iterator begin()
{
    return m_data;
}
const_iterator begin() const
{
    return m_data;
}
iterator end()
{
    return m_data + m_size;
}

const_iterator end() const
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
