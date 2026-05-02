#pragma once
#include <vector>

template <typename T>
class Node;

template <typename T>
struct Link
{
	Node<T>* m_other;
	float m_weight;
	bool m_active = true;
};

///////////
/// T is a tile type
/// It has to have two methods, 
/// - GetTargetDistance(T* _target) that finds the distance between this and the target tile
///		This should take in account the same weight as neighbor links
/// - SetDistance()
/// 
/////////////
template <typename T>
class Node
{
	T* mp_data;

public:
	bool m_visited;
	Node<T>* m_cameFrom;
	std::vector<Link<T>> m_neighbors;

	float m_distance;
	float m_distanceFromStart;
	float m_distanceFromTarget;

	Node(T* data);

	T* GetData() { return mp_data; };
	void AddNeighbor(Link<T> _link);
	float GetTargetDistance(Node<T>* _target);
	void SetDistance(float _distanceFromStart, Node<T>* _target);
};

template<typename T>
inline Node<T>::Node(T* data)
{
	mp_data = data;
	m_visited = false;
	m_cameFrom = nullptr;
}

template <typename T>
inline void Node<T>::AddNeighbor(Link<T> _link)
{
	m_neighbors.push_back(_link);
}

template<typename T>
inline float Node<T>::GetTargetDistance(Node<T>* _target)
{
	return mp_data->GetTargetDistance(_target->mp_data);
}

template<typename T>
inline void Node<T>::SetDistance(float _distanceFromStart, Node<T>* _target)
{
	m_distanceFromStart = _distanceFromStart;
	m_distanceFromTarget = mp_data->GetTargetDistance(_target->mp_data);
	m_distance = m_distanceFromStart + m_distanceFromTarget;
}
