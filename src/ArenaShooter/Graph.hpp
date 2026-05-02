#pragma once
#include <vector>
#include "Path.hpp"

template <typename T>
class Graph
{
public:
	int size;
	std::vector<Node<T>*> m_data;

	void AddNode(Node<T>* _node);
	void ResetNodes();
	Path<T>* GetPath(Node<T>* _node);
};

template<typename T>
inline void Graph<T>::AddNode(Node<T>* _node)
{
	m_data.push_back(_node);
	++size;
}

template<typename T>
inline void Graph<T>::ResetNodes()
{
	for (auto node : m_data)
	{
		node->m_visited = false;
		node->m_cameFrom = nullptr;
		node->m_distanceFromStart = 0;
		node->m_distanceFromTarget = 0;
		node->m_distance = 0;
	}
}

template<typename T>
inline Path<T>* Graph<T>::GetPath(Node<T>* _node)
{
	Path<T>* path = new Path<T>();
	path->m_list.m_data.push_back(_node);
	while (_node->m_cameFrom != nullptr)
	{
		_node = _node->m_cameFrom;
		path->m_list.m_data.push_back(_node);
	}
	path->m_list.Reverse();
	return path;
}
