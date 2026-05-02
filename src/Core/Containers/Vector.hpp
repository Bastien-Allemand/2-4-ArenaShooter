#ifndef CORE_CONTAINERS_VECTOR_HPP_INCLUDED
#define CORE_CONTAINERS_VECTOR_HPP_INCLUDED

#include <vector>
#include "define.h"

namespace gce {


// =====================================================================
/// @brief sequence container that encapsulates dynamic size arrays.
/// @tparam T The type of the elements
// =====================================================================
template <typename T>
struct Vector
{
    using value_type      = T;
    using size_type       = uint64;
    using reference       = T&;
    using const_reference = T const&;
    using pointer         = T*;
    using const_pointer   = T const*;
    using rvalue          = T&&;

    using iterator               = typename std::vector<T>::iterator;
    using const_iterator         = typename std::vector<T>::const_iterator;
    using reverse_iterator       = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
    
    Vector() = default;
    ~Vector() = default;

    explicit Vector( size_type const size ): std( size ) {}
    Vector( size_type const count, const_reference value ): std( count, value ) {}
    explicit Vector( std::initializer_list<T> init ): std( init ) {}

    Vector( Vector const& ) = default;
    Vector( Vector&& ) = default;
    Vector& operator=( Vector const& ) = default;
    Vector& operator=( Vector&& ) noexcept = default;

    // ========================================================================
    /// @brief Replaces the contents with 'count' copies of 'value'.
    /// @param count The new size of the container
    /// @param value The value to initialize elements of the container with
    // ========================================================================
    void Assign( size_type const count, const_reference value ) { return std.assign( count, value ); }

    // ===== Element Access ===== //
    
    // ==========================================================
    /// @brief Access specified element with bounds checking.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // ==========================================================
    [[nodiscard]] reference At( size_type const pos ) { return std.at( pos ); }
    // ==========================================================
    /// @brief Access specified element with bounds checking.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // ==========================================================
    [[nodiscard]] const_reference At( size_type const pos ) const { return std.at( pos ); }
    
    // =================================================
    /// @brief Access specified element.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // =================================================
    [[nodiscard]] reference operator[]( size_type const pos ) { return std[pos]; }
    // =================================================
    /// @brief Access specified element.
    /// @param pos Position of the element to return
    /// @return Reference to the requested element.
    // =================================================
    [[nodiscard]] const_reference operator[]( size_type const pos ) const { return std[pos]; }
    
    // ============================================
    /// @brief Access the first element.
    /// @return Reference to the first element.
    // ============================================
    [[nodiscard]] reference Front() { return std.front(); }
    // ============================================
    /// @brief Access the first element.
    /// @return Reference to the first element.
    // ============================================
    [[nodiscard]] const_reference Front() const { return std.front(); }
    
    // ===========================================
    /// @brief Access the last element.
    /// @return Reference to the last element.
    // ===========================================
    [[nodiscard]] reference Back() { return std.back(); }
    // ===========================================
    /// @brief Access the last element.
    /// @return Reference to the last element.
    // ===========================================
    [[nodiscard]] const_reference Back() const { return std.back(); }
    
    // ============================================================================================================================================================
    /// @brief Direct access to the underlying contiguous storage.
    /// @return Pointer to the underlying element storage.\n
    ///         For non-empty containers, the returned pointer compares equal to the address of the first element.
    // ============================================================================================================================================================
    [[nodiscard]] pointer Data() { return std.data(); }
    // ============================================================================================================================================================
    /// @brief Direct access to the underlying contiguous storage.
    /// @return Pointer to the underlying element storage.\n
    ///         For non-empty containers, the returned pointer compares equal to the address of the first element.
    // ============================================================================================================================================================
    [[nodiscard]] const_pointer Data() const { return std.data(); }

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
    [[nodiscard]] bool Empty() const { return std.empty(); }
    // =================================================
    /// @return Number of elements in the container.
    // =================================================
    [[nodiscard]] size_type Size() const { return std.size(); }
    // ==========================================================================================================================
    /// @return Maximum number of elements the container is able to hold due to system or library implementation limitations.
    // ==========================================================================================================================
    [[nodiscard]] size_type MaxSize() const { return std.max_size(); }
    // =====================================================================================
    /// @return Number of elements that the container has currently allocated space for.
    // =====================================================================================
    [[nodiscard]] size_type Capacity() const { return std.capacity(); }

    // =================================================================================================
    /// @brief Increase the capacity of the vector to a value that's greater or equal to 'capacity'.
    /// @param capacity New capacity of the vector, in number of elements
    // =================================================================================================
    void Reserve( size_type const capacity ) { std.reserve( capacity ); }
    // ==========================================================
    /// @brief Reduces memory usage by freeing unused memory.
    // ==========================================================
    void ShrinkToFit() { std.shrink_to_fit(); }

    // ===== Modifiers ===== //

    // ===================================================
    /// @brief Erases all elements from the container.
    // ===================================================
    void Clear() { std.clear(); }

    // ==================================================================
    /// @brief Inserts a copy of 'value' before 'pos'.
    /// @param pos Iterator before which the content will be inserted
    /// @param value Element value to insert
    /// @return Iterator pointing to the inserted 'value'.
    // ==================================================================
    iterator Insert( const_iterator pos, const_reference value ) { return std.insert( pos, value ); }
    // ========================================================================
    /// @brief Inserts 'value' before 'pos', possibly using move semantics.
    /// @param pos Iterator before which the content will be inserted
    /// @param value Element value to insert
    /// @return Iterator pointing to the inserted 'value'.
    // ========================================================================
    iterator Insert( const_iterator pos, rvalue value ) { return std.insert( pos, value ); }
    // =================================================================================================
    /// @brief Inserts 'count' copies of the 'value' before 'pos'.
    /// @param pos Iterator before which the content will be inserted
    /// @param count Number of elements to insert
    /// @param value Element value to insert
    /// @return Iterator pointing to the first element inserted, or 'pos' if no element is inserted.
    // =================================================================================================
    iterator Insert( const_iterator pos, size_type const count, const_reference value ) { return std.insert( pos, count, value ); }

    // =========================================================================
    /// @brief Constructs an element in-place directly before 'pos'.
    /// @param pos Iterator before which the new element will be constructed
    /// @param args Arguments to forward to the constructor of the element
    /// @return Iterator pointing to the emplaced element.
    // =========================================================================
    template <typename... Args> iterator Emplace( const_iterator pos, Args&&... args ) { return std.emplace( pos, args... ); }
    // =======================================================================
    /// @brief Constructs an element in-place at the end of the container.
    /// @param args Arguments to forward to the constructor of the element
    // =======================================================================
    template <typename... Args> void EmplaceBack( Args&&... args ) { return std.emplace_back( args... ); }
    // =======================================================================
    /// @brief Constructs an element in-place at the end of the container.
    /// @param args Arguments to forward to the constructor of the element
    // =======================================================================
    template <typename... Args> reference EmplaceBack( Args&&... args ) { return std.emplace_back( args... ); }

    // ======================================================================================
    /// @brief Removes the element at 'pos'.
    /// @param pos Iterator to the element to remove
    /// @return Iterator following the last removed element.\n
    ///         If 'pos' refers to the last element, then the end() iterator is returned.
    // ======================================================================================
    iterator Erase( iterator pos ) { return std.erase( pos ); }
    // ======================================================================================
    /// @brief Removes the element at 'pos'.
    /// @param pos Iterator to the element to remove
    /// @return Iterator following the last removed element.\n
    ///         If 'pos' refers to the last element, then the end() iterator is returned.
    // ======================================================================================
    iterator Erase( const_iterator pos ) { return std.erase( pos ); }
    // ================================================================================================
    /// @brief Removes the elements in the range ['first', 'last').
    /// @param first, last The pair of iterators defining the range of elements to remove
    /// @return Iterator following the last removed element.\n
    ///         If 'last' == end() prior to removal, then the updated end() iterator is returned.\n
    ///         If ['first', 'last') is an empty range, then 'last' is returned.
    // ================================================================================================
    iterator Erase( iterator first, iterator last ) { return std.erase( first, last ); }
    // ================================================================================================
    /// @brief Removes the elements in the range ['first', 'last').
    /// @param first, last The pair of iterators defining the range of elements to remove
    /// @return Iterator following the last removed element.\n
    ///         If 'last' == end() prior to removal, then the updated end() iterator is returned.\n
    ///         If ['first', 'last') is an empty range, then 'last' is returned.
    // ================================================================================================
    iterator Erase( const_iterator first, const_iterator last ) { return std.erase( first, last ); }

    // ==================================================================
    /// @brief Appends a copy of 'value' to the end of the container.
    /// @param value The value of the element to append
    // ==================================================================
    void PushBack( const_reference value ) { std.push_back( value ); }
    // ==================================================================
    /// @brief Appends a copy of 'value' to the end of the container.
    /// @param value The value of the element to append
    // ==================================================================
    void PushBack( rvalue value ) { std.push_back( value ); }

    // ======================================================
    /// @brief Removes the last element of the container.
    // ======================================================
    void PopBack() { std.pop_back(); }

    // ====================================================================================================================
    /// @brief Resizes the container to contain 'count' elements.
    /// @param size New size of the container
    /// @note If count is equal to the current size, does nothing.\n
    ///       If the current size is greater than count, the container is reduced to its first count elements.\n
    ///       If the current size is less than count, then additional copies of default-inserted elements are appended.
    // ====================================================================================================================
    void Resize( size_type const size ) { std.resize( size ); }
    // =============================================================================================================
    /// @brief Resizes the container to contain 'count' elements.
    /// @param size New size of the container
    /// @param value The value to initialize the new elements with
    /// @note If count is equal to the current size, does nothing.\n
    ///       If the current size is greater than count, the container is reduced to its first count elements.\n
    ///       If the current size is less than count, then additional copies of value are appended.
    // =============================================================================================================
    void Resize( size_type const size, const_reference value ) { std.resize( size, value ); }

    // =======================================================================================
    /// @brief Exchanges the contents and capacity of the container with those of 'other'.
    /// @param other Container to exchange the contents with
    // =======================================================================================
    void Swap( Vector& other ) { std.swap( other.std ); }

    std::vector<T> std;
};


template <typename T>
[[nodiscard]] bool operator==( Vector<T> const& a, Vector<T> const& b ) { return a.std == b.std; }

template <typename T>
[[nodiscard]] bool operator!=( Vector<T> const& a, Vector<T> const& b ) { return a.std != b.std; }


}

#endif