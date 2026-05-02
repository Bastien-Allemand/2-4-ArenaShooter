#pragma once
#include <list>
#include "Sequence.hpp"
#include "Node.hpp"


template <typename T>
struct Path
{
	Sequence<Node<T>> m_list;
	bool m_temporary = false;
};