#pragma once
#include <vector>


template <typename T>
class Sequence
{

public:
	int m_direction;
	int m_index;
	std::vector<T*> m_data;

	Sequence(): m_index(0), m_direction(1) {};

	void Forward();
	void Backward();
	T* GetCurrent() { return m_data.at(m_index); };
	void SetCurrent(T* _value) { m_data.at(m_index) = _value; };
	void SetEnd() { m_index = m_direction < 0 ? 0 : m_data.size() - 1; };
	void SwapDirection() { m_direction = -m_direction; };
	bool IsAtEnd();
	void Reverse();
	void ResetIndex();
	T* GetStart();
	T* GetEnd();
	int GetEndIndex();
	T* GetForward();
};

template<typename T>
inline void Sequence<T>::Forward()
{
	m_index = (m_index + m_direction >= m_data.size() || m_index + m_direction < 0) ? m_index : m_index + m_direction;
}

template<typename T>
inline void Sequence<T>::Backward()
{
	m_index = (m_index - m_direction >= m_data.size() || m_index - m_direction < 0) ? m_index : m_index - m_direction;
}

template<typename T>
inline bool Sequence<T>::IsAtEnd()
{
	return GetCurrent() == GetEnd();
}

template<typename T>
inline void Sequence<T>::Reverse()
{
	m_index = m_data.size() - m_index - 1;
	SwapDirection();
}

template<typename T>
inline void Sequence<T>::ResetIndex()
{
	if (m_direction < 0)
	{
		m_index = m_data.size() - 1;
	}

	else
	{
		m_index = 0;
	}
}

template<typename T>
inline T* Sequence<T>::GetEnd()
{
	if (m_direction < 0)
	{
		return m_data.front();
	}

	else
	{
		return m_data.back();
	}
}

template<typename T>
inline int Sequence<T>::GetEndIndex()
{
	if (m_direction < 0)
	{
		return 0;
	}

	else
	{
		return m_data.size();
	}
}


template<typename T>
inline T* Sequence<T>::GetStart()
{
	if (m_direction < 0)
	{
		return m_data.back();
	}

	else
	{
		return m_data.front();
	}
}
template<typename T>
inline T* Sequence<T>::GetForward()
{
	return (m_index + m_direction >= m_data.size() || m_index + m_direction < 0) ? m_data[m_index] : m_data[m_index + m_direction];
}