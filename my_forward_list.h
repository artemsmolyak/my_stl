#pragma once

#include <cstddef> //size_t
#include <new>
#include <iostream>

#include "my_alloc.h"


template <typename T, typename Allocator = My_alloc<T> >
class my_forward_list
{
    struct Node
    {
        Node* next;
        T value; 
    };

    // should be rebind pattern using NodeAlloc = std::allocator_traits<Allocator>::rebind_alloc<Node>;
    using NodeAllocator = My_alloc<Node>;
    NodeAllocator m_alloc;

    Node* m_head;
    size_t m_size;

public:

my_forward_list():
m_head(nullptr),
m_size(0)
{
}

void push_front(const T& val)
{
    if (!m_head)
    {
        Node* memNode = m_alloc.allocate(1);
        new (memNode) Node{nullptr, val};
        m_head = memNode; 
    }
    else
    {
        Node* memNode = m_alloc.allocate(1);
        new (memNode) Node{m_head, val};
        m_head = memNode;
    }
    m_size++;
}

size_t size() const
{
    return m_size;
}

bool empty()
{
    return m_size == 0;
}

void pop_front()
{
    auto to_remove = m_head;
    m_head = m_head->next;
    --m_size;
    
    to_remove->~Node();
    operator delete(to_remove);
}

};