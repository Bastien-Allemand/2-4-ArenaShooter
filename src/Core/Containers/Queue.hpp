#ifndef CORE_CONTAINERS_QUEUE_HPP_INCLUDED
#define CORE_CONTAINERS_QUEUE_HPP_INCLUDED

#include <cstdarg>

using uint16 = unsigned __int16;

template<typename T>
struct Queue
{
	Queue() = default;
	/// <summary>
	/// Copy the content of the other queue but isn't link to it.
	/// </summary>
	/// <param name="other">queue that is copied.</param>
	Queue(Queue<T> const& other);
	/// <summary>
	/// Create a queue with the content in argument.
	/// </summary>
	/// <typeparam name="sizeList">Automatically detect the size of the list.</typeparam>
	/// <param name="table">table under the form {{}} or ({}).</param>
	template <int sizeList>
	Queue(T const(&table)[sizeList]);

	/// <summary>
	/// Adds an element at the end of the queue.
	/// </summary>
	/// <param name="value">value to add.</param>
	void Push(T value);
	/// <summary>
	/// Adds a range of element at the end of the queue.
	/// </summary>
	/// <typeparam name="sizeList">Automatically detect the size of the list.</typeparam>
	/// <param name="table">table under the form {{}} or ({}).</param>
	template <int sizeList>
	void PushRange(T const(&table)[sizeList]);
	/// <summary>
	/// Removes the first element and returns the value.
	/// </summary>
	/// <returns>The value of the deleted element.</returns>
	T Pop();
	/// <summary>
	/// Returns back the first element of the queue.
	/// </summary>
	T& Front();
	/// <summary>
	/// Returns the last element of the queue.
	/// </summary>
	T& Back();
	/// <summary>
	/// Returns true if the queue is empty else false.
	/// </summary>
	bool Empty() const;
	/// <summary>
	/// Returns the size of the queue.
	/// </summary>
	uint16 Size();
	/// <summary>
	/// Exchange the content and the node of two different queue.
	/// </summary>
	/// <param name="other"> queue to exchange content with.</param>
	void Swap(Queue<T>& other);

	Queue<T>& operator=(Queue<T>& other);
	bool operator==(Queue<T>& other) const;
	bool operator!=(Queue<T>& other) const;

	~Queue();

private:
	struct Node
	{
		T data;
		Node* pNext = nullptr;

		Node(T data)
		{
			this->data = data;
		}

		~Node() = default;
	};

	Node* m_pFirst = nullptr;
	Node* m_pLast = nullptr;
	uint16 m_size = 0;
};

template<typename T>
inline Queue<T>::Queue(const Queue<T>& other)
{
	Node* current = other.m_pFirst;
	while (current != nullptr)
	{
		Push(current->data);
		current = current->pNext;
	}

	return *this;
}

template<typename T>
template<int sizeList>
Queue<T>::Queue(T const(&table)[sizeList])
{
	for (int i = 0; i < sizeList; i++)
	{
		Push(table[i]);
	}
}

template<typename T>
inline void Queue<T>::Push(T value)
{

	if (m_size == 0)
	{
		m_pFirst = new Node(value);
		m_pLast = m_pFirst;
	}
	else
	{
		m_pLast->pNext = new Node(value);
		m_pLast = m_pLast->pNext;
	}
	m_size++;
}

template<typename T>
inline T Queue<T>::Pop()
{
	if (Empty())
	{
		throw;
	}
	Node* toDel = m_pFirst;
	m_pFirst = m_pFirst->pNext;
	T toReturn = toDel->data;
	delete toDel;
	m_size--;
	return toReturn;
}

template<typename T>
inline T& Queue<T>::Front()
{
	return m_pFirst->data;
}

template<typename T>
inline T& Queue<T>::Back()
{
	return m_pLast->data;
}

template<typename T>
inline bool Queue<T>::Empty() const
{
	return m_size == 0;
}

template<typename T>
inline uint16 Queue<T>::Size()
{
	return m_size;
}

template<typename T>
inline void Queue<T>::Swap(Queue<T>& other)
{
	Node* first = this->m_pFirst;
	Node* last = this->m_pLast;
	uint16 size = this->m_size;

	this->m_pFirst = other.m_pFirst;
	this->m_pLast = other.m_pLast;
	this->m_size = other.m_size;

	other.m_pFirst = first;
	other.m_pLast = last;
	other.m_size = size;
}

template<typename T>
inline Queue<T>& Queue<T>::operator=(Queue<T>& other)
{
	if (this == &other)
	{
		return *this;
	}

	while (!Empty())
	{
		Pop();
	}

	Node* current = other.m_pFirst;
	while (current != nullptr)
	{
		Push(current->data);
		current = current->pNext;
	}

	return *this;
}

template<typename T>
inline bool Queue<T>::operator==(Queue<T>& other) const
{
	if (m_size != other.m_size)
	{
		return false;
	}
	Node* node = m_pFirst;
	Node* otherNode = other.m_pFirst;

	for (int i = 0; i < m_size; i++)
	{
		if (node->data != otherNode->data)
		{
			return false;
		}
		node = node->pNext;
		otherNode = otherNode->pNext;
	}
	return true;
}

template<typename T>
inline bool Queue<T>::operator!=(Queue<T>& other) const
{
	if (m_size != other.m_size)
	{
		return true;
	}
	Node* node = m_pFirst;
	Node* otherNode = other.m_pFirst;

	for (int i = 0; i < m_size; i++)
	{
		if (node->data != otherNode->data)
		{
			return true;
		}
		node = node->pNext;
		otherNode = otherNode->pNext;
	}
	return false;
}

template<typename T>
inline Queue<T>::~Queue()
{
	while (!Empty())
	{
		Pop();
	}
}

template<typename T>
template<int sizeList>
inline void Queue<T>::PushRange(T const(&table)[sizeList])
{
	for (int i = 0; i < sizeList; i++)
	{
		Push(table[i]);
	}
}

#endif