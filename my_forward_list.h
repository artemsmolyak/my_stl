#pragma once

#include <cstddef> //size_t
#include <new>
#include <iostream>

#include "my_alloc.h"

template <typename U>
struct Node
{
    Node* next;
    U value; 
};

template <typename T, typename Allocator = my_alloc<Node <T> > >
class my_forward_list
{
    Node<T>* m_head;
    size_t m_size;

    Allocator m_alloc; 

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
        Node<T>* memNode = m_alloc.allocate(1);
        new (memNode) Node<T>{nullptr, val};
        m_head = memNode; 
    }
    else
    {
        Node<T>* memNode = m_alloc.allocate(1);
        new (memNode) Node<T>{m_head, val};
        m_head = memNode;
    }
    m_size++;
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
    
    to_remove->~Node<T>();
    operator delete(to_remove);
}

};