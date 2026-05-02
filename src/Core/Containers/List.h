#ifndef CORE_LIST_H_INCLUDED
#define CORE_LIST_H_INCLUDED

#include "define.h"
#include <type_traits>
#include <list>
namespace gce {
	template<typename T>
	class List {
	private:
		struct Node {
			T Data;
			Node* pNext;
			Node* pPrev;
			Node(T const& value);
		};

		Node* m_pHead;
		Node* m_pTail;
		uint64 m_Size;

	public:
		class Iterator {
			Node* m_pNode;
		public:
			Iterator(Node* n);
			Iterator() : Node(nullptr) {}
			//Main operators
			T& operator*();
			Iterator& operator++();
			Iterator& operator--();
			bool operator!=(Iterator const& other) const;
			bool operator==(Iterator const& other) const;
			//Auxiliary operators
			Iterator& operator+=(int n);
			Iterator& operator-=(int n);
			Iterator operator+(int n) const;
			Iterator operator-(int n) const;

			bool operator<(Iterator const& other) const;
			bool operator>(Iterator const& other) const;
			bool operator<=(Iterator const& other) const;
			bool operator>=(Iterator const& other) const;

			friend class List<T>;
		};
		class ConstIterator {
			Node const* m_pNODE;
		public:
			ConstIterator(Node const* n);
			ConstIterator() : m_pNODE(nullptr) {}

			// Access operator
			T const& operator*() const;

			// Main operators
			ConstIterator& operator++();
			ConstIterator& operator--();
			bool operator!=(ConstIterator const& other) const;
			bool operator==(ConstIterator const& other) const;
			// Auxiliary operators
			ConstIterator& operator+=(int n);
			ConstIterator& operator-=(int n);
			ConstIterator operator+(int n) const;
			ConstIterator operator-(int n) const;

			bool operator<(ConstIterator const& other) const;
			bool operator>(ConstIterator const& other) const;
			bool operator<=(ConstIterator const& other) const;
			bool operator>=(ConstIterator const& other) const;

			// Conversion implicite d'un itérateur non-const vers const
			ConstIterator(Iterator const& it) : m_pNODE(it.m_pNode) {}


			friend class List<T>;
		};

		List();
		~List();

		// -------Element Access-------
		T const& Front() const;
		T const& Back() const;

		// -------Iterators-------
		Iterator Begin();
		ConstIterator CBegin() const;
		Iterator End();
		ConstIterator CEnd() const;

		// -------Capacity-------
		bool Empty() const;
		uint64 Size() const;
		uint64 max_size() const;

		// -------Modifiers-------
		void Clear();
		Iterator Insert(ConstIterator index, T const& value);

		Iterator Insert(ConstIterator index, uint64 count, T const& value);
		template<typename InputIt>
		typename std::enable_if<!std::is_integral<InputIt>::value, Iterator>::type
			Insert(List<T>::ConstIterator index, InputIt first, InputIt last);

		Iterator Insert(ConstIterator index, List const& other);
		Iterator InsertRange(ConstIterator pos, List const& other);
		Iterator Emplace(ConstIterator pos, T&& value);
		Iterator Erase(Iterator pos);
		Iterator Erase(ConstIterator pos);
		Iterator Erase(Iterator first, Iterator last);
		Iterator Erase(ConstIterator first, ConstIterator last);
		void PushBack(T const& value);
		T& EmplaceBack(T&& value);
		void AppendRange(List& other);
		void PopBack();
		void PushFront(T const& value);
		void PushFront(T&& value);
		T& EmplaceFront(T&& value);
		void PrependRange(List const& other);
		void PopFront();
		void Resize(uint64 newSize);
		void Resize(uint64 newSize, T const& value = T());
		void Swap(List& other);

		// -------Operations-------
		void Merge(List& other);
		void Merge(List&& other);
		template <typename Compare>
		void Merge(List& other, Compare comp);
		template <typename Compare>
		void Merge(List&& other, Compare comp);

		void Splice(ConstIterator pos, List& other);
		void Splice(ConstIterator pos, List&& other);
		void Splice(ConstIterator pos, List& other, ConstIterator it);
		void Splice(ConstIterator pos, List&& other, ConstIterator it);
		void Splice(ConstIterator pos, List& other, ConstIterator first, ConstIterator last);
		void Splice(ConstIterator pos, List&& other, ConstIterator first, ConstIterator last);

		uint64 Remove(T const& value);

		template< class UnaryPred >
		uint64 RemoveIf(UnaryPred predicate);

		void Reverse();
		void Unique();
		template< class BinaryPred >
		uint64 Unique(BinaryPred p);
		void Sort();
	};

	template<typename Iterator>
	void Advance(Iterator& it, int n);

	template<typename U>
	void _Swap(U& a, U& b);

	template<typename U>
	U&& Forward(U& arg);

	template<typename Iterator>
	uint64 Distance(Iterator first, Iterator last);


	template <bool _Test, class _Ty = void>
	struct enable_if {}; 

	template <class _Ty>
	struct enable_if<true, _Ty> {
		using type = _Ty;
	};

	// Custom is_integral
	template<class _Ty>
	struct is_integral {
		static bool const Value = false;
	};

	template<>
	struct is_integral<int> {
		static bool const Value = true;
	};

	template<>
	struct is_integral<uint64> {
		static bool const Value = true;
	};

	template<>
	struct is_integral<unsigned int> {
		static bool const Value = true;
	};
};
#include "List.inl"

#endif