#pragma once
#include <vector>
#include <queue>
#include "Graph.hpp"

template <typename T>
struct SortNode
{
	bool operator()(Node<T>* a, Node<T>* b)
	{
		if (a->m_distance == b->m_distance)
		{
			return a->m_distanceFromStart < b->m_distanceFromStart;
		}
		return a->m_distance > b->m_distance;
	}
};

template <typename T>
inline Path<T>* FindPath(Node<T>* _currentNode, Node<T>* _targetNode, Graph<T>* _graph)
{
	_graph->ResetNodes();
	T* targetData = _targetNode->GetData();

	std::priority_queue <Node<T>*, std::vector<Node<T>*>, SortNode<T>> nodeQueue;
	nodeQueue.emplace(_currentNode);
	_currentNode->m_visited = true;

	while (nodeQueue.empty() == false)
	{
		Node<T>* current = nodeQueue.top();
		nodeQueue.pop();

		if (current == _targetNode)
		{
			return _graph->GetPath(current);
		}

		for (auto adjacentConnexion : current->m_neighbors)
		{
			if (adjacentConnexion.m_active == false)
			{
				continue;
			};
			Node<T>* adjacentNode = adjacentConnexion.m_other;
			if (adjacentNode->m_visited == true)
			{
				if (current->m_distanceFromStart + adjacentConnexion.m_weight < adjacentNode->m_distanceFromStart)
				{
					adjacentNode->m_cameFrom = current;
					adjacentNode->SetDistance(current->m_distanceFromStart + adjacentConnexion.m_weight, _targetNode);
				}
				continue;
			}

			adjacentNode->m_cameFrom = current;
			adjacentNode->m_visited = true;
			adjacentNode->SetDistance(current->m_distanceFromStart + adjacentConnexion.m_weight, _targetNode);

			nodeQueue.emplace(adjacentNode);
		}
	}

	return nullptr;
}