#ifndef CORE_FREELIST_HPP_INCLUDED
#define CORE_FREELIST_HPP_INCLUDED

#include <iterator>
#include <cstddef>

#include "define.h"

namespace gce {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief A freelist that keeps track of available index effectively providing o(1) at access, insertion and deletion.
	/// But not very optimised for iterating through.
	/// @param T Type of stored object.
	/// @remarks T must be greater than sizeof(int32)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class T>
	class FreeList
	{
	public:

		constexpr FreeList(uint64 capacity);
		constexpr FreeList(FreeList const& other);
		constexpr FreeList(FreeList&& other) noexcept;

		constexpr FreeList& operator=(FreeList const& other);
		constexpr FreeList& operator=(FreeList&& other) noexcept;

		constexpr T& operator[](uint64 index);
		constexpr T const& operator[](uint64 index) const;

		constexpr FreeList operator+(FreeList& other);

		~FreeList();

		uint64 Put(const T& obj);
		constexpr T& Get(uint64 index);
		constexpr void Remove(uint32 index);
		constexpr void Resize(uint64 newCapacity);
		constexpr void Clear();

		constexpr uint32 Size() const;
		constexpr uint32 Capacity() const;
		constexpr bool IsEmpty(uint32 id) const;

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			Iterator(pointer ptr, FreeList* listPtr, uint32 startIndex) : m_ptr(ptr), m_pList(listPtr), m_currentIndex(startIndex) {}

			reference operator*() const { return *m_ptr; }
			pointer operator->() { return m_ptr; }

			Iterator& operator++()
			{
				//While gap, advance iterator

				++m_ptr;
				++m_currentIndex;
				while (m_pList->IsEmpty(m_currentIndex) && m_currentIndex != m_pList->Capacity()-1)
				{
					++m_currentIndex;
					++m_ptr;
				}

			if (m_currentIndex == m_pList->Capacity() - 1)
					++m_ptr;

				return *this;
			}

			Iterator operator++(int)
			{
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }

		private:
			pointer m_ptr;
			FreeList* m_pList;
			uint32 m_currentIndex = 0;
		};

		constexpr Iterator begin()
		{
			if (m_capacity == 0) return end();
			uint32 idx = 0;
			// find first non-empty slot
			while (idx < m_capacity && IsEmpty(idx))
				++idx;
			if (idx >= m_capacity)
				return end();
			return Iterator(&m_data[idx], this, idx);
		}
		constexpr Iterator end() { return Iterator(&m_data[m_capacity], this, m_capacity); } //end() always return an out of bound iterator

	private:
		static constexpr int32 m_invalidIndex{ -1 };
		int32 m_lastFreeIndex = 0;
		uint32 m_capacity = 0;
		uint32 m_size = 0;
		T* m_data = nullptr;
		bool* m_emptyList = nullptr;

	private:
		constexpr void FillIndex(uint64 start);
		constexpr bool IsEnd(void* p) const;
	};


	template<class T>
	constexpr FreeList<T>::FreeList(uint64 const capacity) : m_capacity(capacity)
	{
		static_assert(sizeof(T) > sizeof(m_lastFreeIndex));
		m_data = static_cast<T*>(malloc(sizeof(T) * capacity));
		m_emptyList = static_cast<bool*>(malloc(sizeof(bool) * capacity));
		memset(m_emptyList, 1, sizeof(bool) * capacity);
		//fill list with free slots
		FillIndex(1);
	}

	template<class T>
	constexpr FreeList<T>::FreeList(FreeList const& other)
	{
		m_lastFreeIndex = other.m_lastFreeIndex;
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = static_cast<T*>(malloc(m_capacity));
		memcpy(m_data, other.m_data, other.m_capacity * sizeof(T));
	}

	template<class T>
	constexpr FreeList<T>::FreeList(FreeList&& other) noexcept
	{
		m_lastFreeIndex = other.m_lastFreeIndex;
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = other.m_data;
		other.m_data = nullptr;
	}

	template <class T>
	FreeList<T>::~FreeList()
	{
		free(m_data);
	}

	template<class T>
	constexpr FreeList<T>& FreeList<T>::operator=(FreeList const& other)
	{
		if (this == &other) return *this;

		m_lastFreeIndex = other.m_lastFreeIndex;
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = malloc(m_capacity);
		memcpy(m_data, other.m_data, other.m_capacity * sizeof(T));

		return *this;
	}

	template<class T>
	constexpr FreeList<T>& FreeList<T>::operator=(FreeList&& other) noexcept
	{
		m_lastFreeIndex = other.m_lastFreeIndex;
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = other.m_data;
		other.m_data = nullptr;

		return *this;
	}

	template <class T>
	constexpr T& FreeList<T>::operator[](uint64 const index)
	{
		assert(!IsEmpty(index));
		return m_data[index];
	}

	template <class T>
	constexpr T const& FreeList<T>::operator[](uint64 const index) const
	{
		assert(!IsEmpty(index));
		return m_data[index];
	}

	template <class T>
	constexpr FreeList<T> FreeList<T>::operator+(FreeList& other)
	{
		FreeList<T> temp = FreeList(m_capacity + other.m_capacity);
		memcpy(temp.m_data, m_data, m_capacity);
		memcpy(temp.m_data + m_capacity, other.m_data, other.m_capacity);
		return temp;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This function emplaces an object into the vector and blocks an index. If needed, increments memory blocks.
	/// @param obj The object you want to emplace into the vector.
	/// @return Returns the index chosen amongst the available ones.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	uint64 FreeList<T>::Put(const T& obj)
	{
		//TODO check if copy assignable
		//si nextId == -1 realloc
		if (m_lastFreeIndex == -1)
		{
			m_lastFreeIndex = m_capacity;
			Resize(m_capacity * 2);
			//m_capacity += m_capacity;
			//m_data = static_cast<T*>(realloc(m_data, sizeof(T) * m_capacity));
			//FillIndex(m_lastFreeIndex);
		}

		uint32 nextId = *reinterpret_cast<uint32*>(&m_data[m_lastFreeIndex]);
		uint32 cachedIndex = m_lastFreeIndex;

		if constexpr (std::is_move_constructible_v<T>)
		{
			new (&m_data[m_lastFreeIndex]) T(std::move(obj));
			//m_data[m_lastFreeIndex] = std::move(obj);
		}
		else if constexpr (std::is_copy_constructible_v<T>)
		{
			new (&m_data[m_lastFreeIndex]) T(obj);
			//m_data[m_lastFreeIndex] = obj;
		}
		else
			memcpy(&m_data[m_lastFreeIndex], &obj, sizeof(T));

		m_emptyList[m_lastFreeIndex] = false;
		m_lastFreeIndex = nextId == m_capacity ? -1 : nextId;
		m_size++;

		return cachedIndex;
	}

	////////////////////////////////////////////////////////////////////
	/// @brief This function gets an object in the vector at index.
	/// @param index The index which we get the value from.
	/// @return Returns the object at index.
	////////////////////////////////////////////////////////////////////
	template<class T>
	constexpr T& FreeList<T>::Get(uint64 const index)
	{
		assert(IsEmpty(index));
		return m_data[index];
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This function removes an object at index. Frees the index and puts it back into available ones. If needed, shrinks the memory to fit the new capacity.
	/// @param index The index which we remove the value from.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class T>
	constexpr void FreeList<T>::Remove(uint32 const index)
	{
		m_data[index].~T();
		memset(&m_data[index], 0xcc, sizeof(T));
		*reinterpret_cast<int32*>(&m_data[index]) = m_lastFreeIndex;
		m_lastFreeIndex = static_cast<int32>(index);
		m_emptyList[index] = true;
		m_size--;
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief This function clears the whole vector and erases all indexes.
	///////////////////////////////////////////////////////////////////////////
	template <class T>
	constexpr void FreeList<T>::Clear()
	{
		memset(m_data, 0xcc, sizeof(T) * m_capacity);
		memset(m_emptyList, true, sizeof(bool) * m_capacity);
		Resize(1);

		m_size = 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Resize the list to a set capacity. If the new capacity is smaller than the previous one, exceeding elements will be freed.
	/// @param newCapacity The new capacity of the freelist
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class T>
	constexpr void FreeList<T>::Resize(uint64 const newCapacity)
	{
		m_data = static_cast<T*>(realloc(m_data, sizeof(T) * newCapacity));
		m_emptyList = static_cast<bool*>(realloc(m_emptyList, sizeof(bool) * newCapacity));

		uint32 tempCapacity = m_capacity;
		m_capacity = newCapacity;
		if (newCapacity > tempCapacity)
			FillIndex(tempCapacity + 1);

	}

	///////////////////////////////////////////////////////////////////
	/// @brief Return the amount of active elements inside the freelist
	///////////////////////////////////////////////////////////////////
	template<class T>
	constexpr uint32 FreeList<T>::Size() const
	{
		return m_size;
	}

	/////////////////////////////////////////////////////////////////
	/// @brief Return the total capacity (allocated) of the free list
	/////////////////////////////////////////////////////////////////
	template<class T>
	constexpr uint32 FreeList<T>::Capacity() const
	{
		return m_capacity;
	}

	template <class T>
	constexpr bool FreeList<T>::IsEmpty(uint32 const id) const
	{
		return m_emptyList[id];
		//uint32 i = sizeof(m_lastFreeIndex); //Offset the first 4 bytes
		//uint8* pMem = reinterpret_cast<uint8*>(&m_data[id]);
		//while (pMem[i] == 0xcc && i < sizeof(T))
		//	i++; //if the whole "cell" is 0xcc it means that it's empty
		//return i == sizeof(T);
	}

	template <class T>
	constexpr bool FreeList<T>::IsEnd(void* p) const
	{
		uint8 val = *(static_cast<uint8*>(p) + sizeof(m_lastFreeIndex));
		return p > m_data + m_capacity;
	}

	template<class T>
	constexpr void FreeList<T>::FillIndex(uint64 const start)
	{
		for (uint64 i = start; i < m_capacity + 1; i++)
		{
			//0xcc uninitialized memory
			memset(&m_data[i - 1], 0xcc, sizeof(T));
			*reinterpret_cast<uint32*>(&m_data[i - 1]) = i;
		}
	}

}


#endif