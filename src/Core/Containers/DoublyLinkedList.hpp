#ifndef CORE_CONTAINERS_DOUBLY_LINKED_LIST_HPP_INCLUDED
#define CORE_CONTAINERS_DOUBLY_LINKED_LIST_HPP_INCLUDED

#include "define.h"


template <typename T>
struct DoublyLinkedList
{
public:
    DoublyLinkedList() = default;
    DoublyLinkedList( DoublyLinkedList const& ) = default;
    ~DoublyLinkedList() = default;

private:
    struct Node;

    Node* m_pTail = nullptr;
    uint64 m_size = 0;
};


#endif