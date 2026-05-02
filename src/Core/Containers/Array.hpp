#ifndef CORE_CONTAINERS_ARRAY_HPP_INCLUDED
#define CORE_CONTAINERS_ARRAY_HPP_INCLUDED

#include <array>
#include "define.h"

namespace gce {


// ==========================================================
/// @brief Container that encapsulates fixed size arrays.
/// @tparam T Element type
/// @tparam size Number of elements in the array
// ==========================================================
template <typename T, uint64 size>
struct Array
{
    using value_type      = T;
    using size_type       = uint64;
    using reference       = T&;
    using const_reference = T const&;
    using pointer         = T*;
    using const_pointer   = T const*;

    using iterator               = typename std::array<T, size>::iterator;
    using const_iterator         = typename std::array<T, size>::const_iterator;
    using reverse_iterator       = typename std::array<T, size>::reverse_iterator;
    using const_reverse_iterator = typename std::array<T, size>::const_reverse_iterator;
    
    constexpr Array() = default;
    ~Array() = default;

    constexpr Array( Array const& ) = default;
    constexpr Array( Array&& ) = default;
    constexpr Array& operator=( Array const& ) = default;
    constexpr Array& operator=( Array&& ) = default;

    // ===== Element Access ===== //

    // ==========================================================
    /// @brief Access specified element with bounds checking.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // ==========================================================
    [[nodiscard]] constexpr reference At( size_type const pos ) { return std.at( pos ); }
    // ==========================================================
    /// @brief Access specified element with bounds checking.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // ==========================================================
    [[nodiscard]] constexpr const_reference At( size_type const pos ) const { return std.at( pos ); }

    // =================================================
    /// @brief Access specified element.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // =================================================
    [[nodiscard]] constexpr reference operator[]( size_type const pos ) { return std[pos]; }
    // =================================================
    /// @brief Access specified element.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // =================================================
    [[nodiscard]] constexpr const_reference operator[]( size_type const pos ) const { return std[pos]; }

    // ============================================
    /// @brief Access the first element.
    /// @return Reference to the first element.
    // ============================================
    [[nodiscard]] constexpr reference Front() { return std.front(); }
    // ============================================
    /// @brief Access the first element.
    /// @return Reference to the first element.
    // ============================================
    [[nodiscard]] constexpr const_reference Front() const { return std.front(); }

    // ===========================================
    /// @brief Access the last element.
    /// @return Reference to the last element.
    // ===========================================
    [[nodiscard]] constexpr reference Back() { return std.back(); }
    // ===========================================
    /// @brief Access the last element.
    /// @return Reference to the last element.
    // ===========================================
    [[nodiscard]] constexpr const_reference Back() const { return std.back(); }

    // ===============================================================================================================
    /// @brief Direct access to the underlying contiguous storage.
    /// @return Pointer to the underlying element storage.\n
    ///         For non-empty containers, the returned pointer compares equal to the address of the first element.
    // ===============================================================================================================
    [[nodiscard]] constexpr pointer Data() { return std.data(); }
    // ===============================================================================================================
    /// @brief Direct access to the underlying contiguous storage.
    /// @return Pointer to the underlying element storage.\n
    ///         For non-empty containers, the returned pointer compares equal to the address of the first element.
    // ===============================================================================================================
    [[nodiscard]] constexpr const_pointer Data() const { return std.data(); }

    // ===== Iterators ===== //

    // =======================================
    /// @return Iterator to the beginning.
    // =======================================
    [[nodiscard]] iterator begin() { return std.begin(); }
    // =======================================
    /// @return Iterator to the beginning.
    // =======================================
    [[nodiscard]] const_iterator begin() const { return std.begin(); }
    // =======================================
    /// @return Iterator to the beginning.
    // =======================================
    [[nodiscard]] const_iterator CBegin() const { return std.cbegin(); }

    // =================================
    /// @return Iterator to the end.
    // =================================
    [[nodiscard]] iterator end() { return std.end(); }
    // =================================
    /// @return Iterator to the end.
    // =================================
    [[nodiscard]] const_iterator end() const { return std.end(); }
    // =================================
    /// @return Iterator to the end.
    // =================================
    [[nodiscard]] const_iterator CEnd() const { return std.cend(); }

    // ===============================================
    /// @return Reverse iterator to the beginning.
    // ===============================================
    [[nodiscard]] iterator RBegin() { return std.rbegin(); }
    // ===============================================
    /// @return Reverse iterator to the beginning.
    // ===============================================
    [[nodiscard]] const_iterator RBegin() const { return std.rbegin(); }
    // ===============================================
    /// @return Reverse iterator to the beginning.
    // ===============================================
    [[nodiscard]] const_iterator RCBegin() const { return std.rcbegin(); }

    // =========================================
    /// @return Reverse iterator to the end.
    // =========================================
    [[nodiscard]] iterator REnd() { return std.rend(); }
    // =========================================
    /// @return Reverse iterator to the end.
    // =========================================
    [[nodiscard]] const_iterator REnd() const { return std.rend(); }
    // =========================================
    /// @return Reverse iterator to the end.
    // =========================================
    [[nodiscard]] const_iterator RCEnd() const { return std.rcend(); }
    
    // ===== Capacity ===== //

    // =============================================================
    /// @brief Checks if the container has no elements.
    /// @return true if the container is empty, false otherwise.
    // =============================================================
    [[nodiscard]] constexpr bool Empty() const { return std.empty(); }
    // =================================================
    /// @return Number of elements in the container.
    // =================================================
    [[nodiscard]] constexpr size_type Size() const { return std.size(); }
    // ======================================================================
    /// @return Maximum number of elements the container is able to hold.
    // ======================================================================
    [[nodiscard]] constexpr size_type MaxSize() const { return std.max_size(); }

    // ===== Operations ===== //

    // =================================================================
    /// @brief Assigns the 'value' to all elements in the container.
    /// @param value The value to assign to the elements
    // =================================================================
    void Fill( const_reference value ) { std.fill( value ); }
    // ==========================================================================
    /// @brief Exchanges the contents of the container with those of 'other'.
    /// @param other Container to exchange the contents with
    // ==========================================================================
    void Swap( Array& other ) { std.swap( other.std ); }

    std::array<T, size> std;
};


template <typename T, uint64 size>
[[nodiscard]] bool operator==( Array<T, size> const& a, Array<T, size> const& b ) { return a.std == b.std; }

template <typename T, uint64 size>
[[nodiscard]] bool operator!=( Array<T, size> const& a, Array<T, size> const& b ) { return a.std != b.std; }


// template <typename T, uint64 t_size>
// struct Array
// {
// public:
// 	// Default Constructor
// 	Array() = default;
// 	// Copy Constructor
// 	Array(Array const&) = default;
// 	// Destructor
// 	~Array() = default;
//
// 	// Constructor with a specific value
// 	template <typename... Args>
// 	Array(Args... args) requires (sizeof...(Args) > 0 && sizeof...(Args) <= t_size)
// 	{
// 		static_assert(sizeof...(Args) <= t_size, "Too many initializers for Array");
//
// 		T values[] = { args... };
// 		for (uint64 i = 0; i < sizeof...(Args); ++i)
// 		{
// 			m_data[i] = values[i];
// 		}
//
// 		if (sizeof...(Args) < t_size)
// 		{
// 			for (uint64 i = sizeof...(Args); i < t_size; ++i)
// 			{
// 				m_data[i] = T();
// 			}
// 		}
// 	}
// 	// Copy assignment operator
// 	Array& operator=(Array const& other) = default;
//
// 	// Find a value in the array at a specific position
// 	uint64 At(uint64 index);
// 	// Overloading for accessing elements
// 	T& operator[](uint64 index);
// 	// const overloading for accessing elements
// 	T const& operator[](uint64 index) const;
//
// 	// Get the first elements
// 	T Front();
// 	// Get the last elements
// 	T Back();
// 	// Get the data pointer
// 	T* Data();
//
// 	// Check if the array is empty
// 	bool Empty() const;
// 	// Get the size of the array
// 	uint64 Size() const;
// 	// Get the maximum size of the array
// 	uint64 MaxSize() const;
//
// 	// Get the beginning iterators
// 	T* Begin();
// 	// Get the beginning const iterators
// 	T const* CBegin() const;
// 	// Get the end iterators
// 	T* End();
// 	// Get the end const iterators
// 	T const* CEnd() const;
// 	// Get the reverse iterators
// 	T* RBegin();
// 	// Get the reverse const iterators
// 	T const* CRBegin() const;
// 	// Get the reverse end iterators
// 	T* REnd();
// 	// Get the reverse end const iterators
// 	T const* CREnd() const;
//
// 	// Replace all elements in the array with a specific value
// 	void Fill(T const& value);
// 	// Swap the contents of this array with another array
// 	void Swap(Array& other);
//
// 	//Get the element at a specific index in the array
// 	T& Get(uint64 index);
// 	//Convert a C style array to an Array
// 	static Array<T, t_size> ToArray(T(&a)[t_size]);
//
// private:
// 	T m_data[t_size]{};
// };
//
// //Get the element at a specific index in the array
// template<typename T, uint64 t_size>
// inline T& Array<T, t_size>::Get(uint64 index)
// {
// 	if (index >= t_size)
// 	{
// 		throw;
// 	}
// 	return m_data[index];
// }
//
// //Convert a C style array to an Array
// template<typename T, uint64 t_size>
// inline Array<T, t_size> Array<T, t_size>::ToArray(T(&a)[t_size])
// {
// 	Array < T, t_size > array;
// 	for (int i = 0; i < t_size; ++i)
// 	{
// 		array[i] = a[i];
// 	}
// 	return array;
// }
//
// ////Creating Array from multiple elements
// //template<typename A, typename... U>
// //Array<A, 1 + sizeof...(U)> ToArray(A first, U... rest) const
// //{
// //	Array< A, 1 + sizeof...(U)> list;
// //	A temp[] = { first, A(rest)... };
// //
// //	for (uint64 i = 0; i < 1 + sizeof...(U); i++)
// //	{
// //		list[i] = temp[i];
// //	}
// //	return list;
// //}
//
// template<typename T, uint64 t_size>
// inline uint64 Array<T, t_size>::At(uint64 index)
// {
// 	if (index >= t_size)
// 	{
// 		throw;
// 	}
// 	return m_data[index];
// }
//
// template<typename T, uint64 t_size>
// inline T& Array<T, t_size>::operator[](size_t index)
// {
// 	return m_data[index];
// }
//
// template<typename T, uint64 t_size>
// inline T& Array<T, t_size>::operator[](size_t index) const
// {
// 	return m_data[index];
// }
//
// template<typename T, uint64 t_size>
// inline T Array<T, t_size>::Front()
// {
// 	static_assert(t_size > 0, "Array size must be greater than 0");
//
// 	return m_data[0];
// }
//
// template<typename T, uint64 t_size>
// inline T Array<T, t_size>::Back()
// {
// 	static_assert(t_size > 0, "Array size must be greater than 0");
//
// 	return m_data[t_size - 1];
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::Data()
// {
// 	return m_data;
// }
//
// template<typename T, uint64 t_size>
// inline bool Array<T, t_size>::Empty() const
// {
// 	return t_size == 0;
// }
//
// template<typename T, uint64 t_size>
// inline uint64 Array<T, t_size>::Size() const
// {
// 	return t_size;
// }
//
// template<typename T, uint64 t_size>
// inline uint64 Array<T, t_size>::MaxSize() const
// {
// 	return t_size;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::Begin()
// {
// 	return m_data;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::CBegin() const
// {
// 	return m_data;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::End()
// {
// 	return m_data + t_size;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::CEnd() const
// {
// 	return m_data + t_size;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::RBegin()
// {
// 	return m_data + t_size - 1;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::CRBegin() const
// {
// 	return m_data + t_size - 1;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::REnd()
// {
// 	return m_data - 1;
// }
//
// template<typename T, uint64 t_size>
// inline T* Array<T, t_size>::CREnd() const
// {
// 	return m_data - 1;
// }
//
// template<typename T, uint64 t_size>
// inline void Array<T, t_size>::Fill(T const& value)
// {
// 	for (uint64 i = 0; i < t_size; ++i)
// 	{
// 		m_data[i] = value;
// 	}
// }
//
// template<typename T, uint64 t_size>
// inline void Array<T, t_size>::Swap(Array& other)
// {
// 	for (uint64 i = 0; i < t_size; ++i)
// 	{
// 		T temp = m_data[i];
// 		m_data[i] = other.m_data[i];
// 		other.m_data[i] = temp;
// 	}
// }
//
// /// operator overload
//
// template<typename T, uint64 t_size>
// inline bool operator==(Array<T, t_size> const& lhs, Array<T, t_size> const& rhs)
// {
// 	for (uint64 i = 0; i < t_size; ++i)
// 	{
// 		if (lhs[i] != rhs[i])
// 		{
// 			return false;
// 		}
// 	}
// 	return true;
// }
//
// template<typename T, uint64 t_size>
// inline bool operator!=(Array<T, t_size> const& lhs, Array<T, t_size> const& rhs)
// {
// 	return !(lhs == rhs);
// }
//
// template<typename T, uint64 t_size>
// inline bool operator<=(Array<T, t_size> const& lhs, Array<T, t_size> const& rhs)
// {
// 	return (lhs <= rhs);
// }
//
// template<typename T, uint64 t_size>
// inline bool operator>=(Array<T, t_size> const& lhs, Array<T, t_size> const& rhs)
// {
// 	return (lhs >= rhs);
// }


}

#endif