#ifndef CORE_CONTAINERS_SINGLY_LINKED_LIST_HPP_INCLUDED
#define CORE_CONTAINERS_SINGLY_LINKED_LIST_HPP_INCLUDED

#include "define.h"


template <typename T>
struct SinglyLinkedList
{
public:
    SinglyLinkedList() = default;
    SinglyLinkedList( SinglyLinkedList const& ) = default;
    ~SinglyLinkedList() = default;

private:
    struct Node;

    Node* m_pTail = nullptr;
    uint64 m_size = 0;
};


#endif