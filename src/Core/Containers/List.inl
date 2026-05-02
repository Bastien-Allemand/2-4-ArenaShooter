#ifndef CORE_LIST_INL_INCLUDED
#define CORE_LIST_INL_INCLUDED

#include <type_traits>
namespace gce {// Node
	template<typename T>
	List<T>::Node::Node(T const& value) : Data(value), pPrev(nullptr), pNext(nullptr) {}


	// Iterator
	template<typename T>
	List<T>::Iterator::Iterator(Node* n) : m_pNode(n) {}


	template<typename T>
	T& List<T>::Iterator::operator*() { return m_pNode->Data; }

	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator++() { m_pNode = m_pNode->pNext; return *this; }

	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator--() { m_pNode = m_pNode->pPrev; return *this; }

	template<typename T>
	bool List<T>::Iterator::operator!=(Iterator const& other) const { return m_pNode != other.m_pNode; }

	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator+=(int n) {
		for (int i = 0; i < n && m_pNode; ++i)
			m_pNode = m_pNode->pNext;
		return *this;
	}

	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator-=(int n) {
		for (int i = 0; i < n && m_pNode; ++i)
			m_pNode = m_pNode->pPrev;
		return *this;
	}

	template<typename T>
	typename List<T>::Iterator List<T>::Iterator::operator+(int n) const {
		Iterator it = *this;
		for (int i = 0; i < n && it.m_pNode; ++i)
			it.m_pNode = it.m_pNode->pNext;
		return it;
	}

	template<typename T>
	typename List<T>::Iterator List<T>::Iterator::operator-(int n) const {
		Iterator it = *this;
		for (int i = 0; i > n && it.m_pNode; ++i)
			it.m_pNode = it.m_pNode->pPrev;
		return it;
	}

	template <typename T>
	bool List<T>::Iterator::operator==(Iterator const& other) const { return m_pNode == other.m_pNode; }

	template<typename T>
	bool List<T>::Iterator::operator<(Iterator const& other) const { return m_pNode < other.m_pNode; }

	template<typename T>
	bool List<T>::Iterator::operator>(Iterator const& other) const { return m_pNode > other.m_pNode; }

	template<typename T>
	bool List<T>::Iterator::operator>=(Iterator const& other) const { return m_pNode >= other.m_pNode; }

	template<typename T>
	bool List<T>::Iterator::operator<=(Iterator const& other) const { return m_pNode <= other.m_pNode; }

	//ConstIterator
	template<typename T>
	List<T>::ConstIterator::ConstIterator(Node const* n) : m_pNODE(n) {}

	template<typename T>
	T const& List<T>::ConstIterator::operator*() const { return m_pNODE->Data; }

	template<typename T>
	typename List<T>::ConstIterator& List<T>::ConstIterator::operator++() {
		m_pNODE = m_pNODE->pNext;
		return *this;	
	}

	template<typename T>
	typename List<T>::ConstIterator& List<T>::ConstIterator::operator--() {
		m_pNODE = m_pNODE->pPrev;
		return *this;
	}

	template<typename T>
	bool List<T>::ConstIterator::operator!=(ConstIterator const& other) const {
		return m_pNODE != other.m_pNODE;
	}

	template<typename T>
	bool List<T>::ConstIterator::operator==(ConstIterator const& other) const {
		return m_pNODE == other.m_pNODE;
	}

	template<typename T>
	typename List<T>::ConstIterator& List<T>::ConstIterator::operator+=(int n) {
		for (int i = 0; i < n && m_pNODE; ++i)
			m_pNODE = m_pNODE->pNext;
		return *this;
	}

	template<typename T>
	typename List<T>::ConstIterator& List<T>::ConstIterator::operator-=(int n) {
		for (int i = 0; i < n && m_pNODE; ++i)
			m_pNODE = m_pNODE->pPrev;
		return *this;
	}

	template<typename T>
	typename List<T>::ConstIterator List<T>::ConstIterator::operator+(int n) const {
		ConstIterator it = *this;
		for (int i = 0; i < n && it.m_pNODE; ++i)
			it.m_pNODE = it.m_pNODE->pNext;
		return it;
	}

	template<typename T>
	typename List<T>::ConstIterator List<T>::ConstIterator::operator-(int n) const {
		ConstIterator it = *this;
		for (int i = 0; i < n && it.m_pNODE; ++i)
			it.m_pNODE = it.m_pNODE->pPrev;
		return it;
	}

	template<typename T>
	bool List<T>::ConstIterator::operator<(ConstIterator const& other) const {
		return m_pNODE < other.m_pNODE;
	}

	template<typename T>
	bool List<T>::ConstIterator::operator>(ConstIterator const& other) const {
		return m_pNODE > other.m_pNODE;
	}

	template<typename T>
	bool List<T>::ConstIterator::operator>=(ConstIterator const& other) const {
		return m_pNODE >= other.m_pNODE;
	}

	template<typename T>
	bool List<T>::ConstIterator::operator<=(ConstIterator const& other) const {
		return m_pNODE <= other.m_pNODE;
	}

	// List
	template<typename T>
	List<T>::List() : m_pHead(nullptr), m_pTail(nullptr), m_Size(0) {}

	template<typename T>
	List<T>::~List() { Clear(); }

	template<typename T>
	void List<T>::PushBack(T const& value) {
		Node* n = new Node(value);
		if (!m_pTail) {
			m_pHead = m_pTail = n;
		}
		else {
			m_pTail->pNext = n;
			n->pPrev = m_pTail;
			m_pTail = n;
		}
		++m_Size;
	}

	template<typename T>
	void List<T>::PushFront(T const& value) {
		Node* n = new Node(value);
		if (!m_pHead) {
			m_pHead = m_pTail = n;
		}
		else {
			n->pNext = m_pHead;
			m_pHead->pPrev = n;
			m_pHead = n;
		}
		++m_Size;
	}

	template<typename T>
	void List<T>::PushFront(T&& value) {
		Node* n = new Node(Forward<T>(value));
		if (!m_pHead) {
			m_pHead = m_pTail = n;
		}
		else {
			n->pNext = m_pHead;
			m_pHead->pPrev = n;
			m_pHead = n;
		}
		++m_Size;
	}

	template<typename T>
	void List<T>::PopBack() {
		if (m_pTail) {
			Node* toDelete = m_pTail;
			if (m_pTail->pPrev) {
				m_pTail = m_pTail->pPrev;
				m_pTail->pNext = nullptr;
			}
			else {
				m_pHead = m_pTail = nullptr;
			}
			delete toDelete;
			--m_Size;
		}
	}

	template<typename T>
	void List<T>::PopFront() {
		if (m_pHead) {
			Node* toDelete = m_pHead;
			if (m_pHead->pNext) {
				m_pHead = m_pHead->pNext;
				m_pHead->pPrev = nullptr;
			}
			else {
				m_pHead = m_pTail = nullptr;
			}
			delete toDelete;
			--m_Size;
		}
	}

	template<typename T>
	void List<T>::Clear() {
		while (m_pHead) {
			PopFront();
		}
	}

	template<typename T>
	const T& List<T>::Front() const {
		if (m_pHead) {
			return m_pHead->Data;
		}
		throw ("List is Empty");
	}

	template<typename T>
	const T& List<T>::Back() const {
		if (m_pTail) {
			return m_pTail->Data;
		}
		throw "List is Empty";
	}

	template<typename T>
	uint64 List<T>::Size() const {
		return m_Size;
	}

	template<typename T>
	bool List<T>::Empty() const {
		return m_Size == 0;
	}

	template<typename T>
	void List<T>::Swap(List& other) {
		Node* tmpHead = m_pHead;
		Node* tmpTail = m_pTail;
		uint64 tmpSz = m_Size;

		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		m_Size = other.m_Size;

		other.m_pHead = tmpHead;
		other.m_pTail = tmpTail;
		other.m_Size = tmpSz;
	}


	template<typename T>
	void List<T>::PrependRange(const List& other) {
		if (other.Empty()) return;
		Node* current = other.m_pTail;
		while (current) {
			PushFront(current->Data);
			current = current->pPrev;
		}
	}
	template <typename T>
	void List<T>::AppendRange(List& other) {
		if (other.Empty()) return;
		if (!m_pTail) { // Liste vide
			m_pHead = other.m_pHead;
			m_pTail = other.m_pTail;
		}
		else {
			m_pTail->pNext = other.m_pHead;
			other.m_pHead->pPrev = m_pTail;
			m_pTail = other.m_pTail;
		}
		m_Size += other.m_Size;
		other.m_pHead = nullptr;
		other.m_pTail = nullptr;
		other.m_Size = 0;
	}
	
	
	template<typename T>
	void List<T>::Sort() {
		if (m_Size < 2) return;
		bool swapped;
		do {
			swapped = false;
			Node* current = m_pHead;
			while (current && current->pNext) {
				if (current->Data > current->pNext->Data) {
					_Swap(current->Data, current->pNext->Data);
					swapped = true;
				}
				current = current->pNext;
			}
		} while (swapped);
	}
	template<typename T>
	uint64 List<T>::max_size() const {
		return static_cast<uint64>(-1) / sizeof(Node);
	}

	template<typename T>
	typename List<T>::Iterator List<T>::Insert(ConstIterator index, const T& value) {
		Node* posNode = const_cast<Node*>(index.m_pNODE);
		Node* n = new Node(value);
		if (!posNode) {
			if (!m_pTail) {
				m_pHead = m_pTail = n;
			}
			else {
				m_pTail->pNext = n;
				n->pPrev = m_pTail;
				m_pTail = n;
			}
		}
		else if (posNode == m_pHead) {
			n->pNext = m_pHead;
			m_pHead->pPrev = n;
			m_pHead = n;
		}
		else {
			n->pNext = posNode;
			n->pPrev = posNode->pPrev;
			if (posNode->pPrev) {
				posNode->pPrev->pNext = n;
			}
			posNode->pPrev = n;
		}
		++m_Size;
		return Iterator(n);
	}

	template<typename T>
	typename List<T>::Iterator List<T>::Insert(ConstIterator index, uint64 count, const T& value) {
		Node* posNode = const_cast<Node*>(index.m_pNODE);
		Node* prevNode = posNode ? posNode->pPrev : m_pTail;
		for (uint64 i = 0; i < count; ++i) {
			Node* n = new Node(value);
			if (!posNode) {
				if (!m_pTail) {
					m_pHead = m_pTail = n;
				}
				else {
					m_pTail->pNext = n;
					n->pPrev = m_pTail;
					m_pTail = n;
				}
			}
			else if (posNode == m_pHead) {
				n->pNext = m_pHead;
				m_pHead->pPrev = n;
				m_pHead = n;
			}
			else { 
				n->pNext = posNode;
				n->pPrev = prevNode;
				if (prevNode) {
					prevNode->pNext = n;
				}
				posNode->pPrev = n;
			}
			prevNode = n;
			++m_Size;	
		}
		return Iterator(prevNode);
	}

	template<typename T>
	template<typename InputIt>
	typename std::enable_if<!std::is_integral<InputIt>::value, typename List<T>::Iterator>::type
		List<T>::Insert(List<T>::ConstIterator index, InputIt first, InputIt last){
		Node* posNode = const_cast<Node*>(index.m_pNODE);
		Node* prevNode = posNode ? posNode->pPrev : m_pTail;
		while (first != last) {
			Node* n = new Node(*first);
			if (!posNode) { 
				if (!m_pTail) { 
					m_pHead = m_pTail = n;
				}
				else {
					m_pTail->pNext = n;
					n->pPrev = m_pTail;
					m_pTail = n;
				}
			}
			else if (posNode == m_pHead) {
				n->pNext = m_pHead;
				m_pHead->pPrev = n;
				m_pHead = n;
			}
			else {
				n->pNext = posNode;
				n->pPrev = prevNode;
				if (prevNode) {
					prevNode->pNext = n;
				}
				posNode->pPrev = n;
			}
			prevNode = n;
			++m_Size;
			++first;
		}
		return Iterator(prevNode);
	}

	template <typename T>
	typename List<T>::Iterator List<T>::Insert(ConstIterator index, const List& other) {
		return Insert(index, other.CBegin(), other.CEnd());
	}

	template <typename T>
	typename List<T>::Iterator List<T>::InsertRange(ConstIterator pos, const List& other) {
		Node* posNode = const_cast<Node*>(pos.pNode);
		Node* prevNode = posNode ? posNode->pPrev : m_pTail;
		Node* current = other.m_pHead;
		while (current) {
			Node* n = new Node(current->Data);
			if (!posNode) {
				if (!m_pTail) {
					m_pHead = m_pTail = n;
				}
				else {
					m_pTail->pNext = n;
					n->pPrev = m_pTail;
					m_pTail = n;
				}
			}
			else if (posNode == m_pHead) { 
				n->pNext = m_pHead;
				m_pHead->pPrev = n;
				m_pHead = n;
			}
			else {
				n->pNext = posNode;
				n->pPrev = prevNode;
				if (prevNode) {
					prevNode->pNext = n;
				}
				posNode->pPrev = n;
			}
			prevNode = n;
			current = current->pNext;
			++m_Size;
		}
		return Iterator(prevNode);
	}
	
	template<typename T>
	void List<T>::Resize(uint64 newSize) {
		while (m_Size < newSize) {
			PushBack(T());
		}
		while (m_Size > newSize) {
			PopBack();
		}
	}

	template<typename T>
	void List<T>::Resize(uint64 newSize, const T& value) {
		while (m_Size < newSize) {
			PushBack(value);
		}
		while (m_Size > newSize) {
			PopBack();
		}
	}

	template<typename T>
	uint64 List<T>::Remove(const T& value) {
		uint64 count = 0;
		Node* current = m_pHead;
		while (current) {
			if (current->Data == value) {
				Node* toDelete = current;
				if (current->pPrev) {
					current->pPrev->pNext = current->pNext;
				}
				else {
					m_pHead = current->pNext;
				}
				if (current->pNext) {
					current->pNext->pPrev = current->pPrev;
				}
				else {
					m_pTail = current->pPrev;
				}
				current = current->pNext;
				delete toDelete;
				--m_Size;
				++count;
			}
			else {
				current = current->pNext;
			}
		}
		return count;
	}

	template<typename T>
	template< class UnaryPred >
	uint64 List<T>::RemoveIf(UnaryPred pred) {
		uint64 count = 0;
		Node* current = m_pHead;
		while (current) {
			if (pred(current->Data)) {
				Node* toDelete = current;
				if (current->pPrev) {
					current->pPrev->pNext = current->pNext;
				}
				else {
					m_pHead = current->pNext;
				}
				if (current->pNext) {
					current->pNext->pPrev = current->pPrev;
				}
				else {
					m_pTail = current->pPrev;
				}
				current = current->pNext;
				delete toDelete;
				--m_Size;
				++count;
			}
			else {
				current = current->pNext;
			}
		}
		return count;
	}


	template<typename T>
	void List<T>::Reverse() {
		if (m_Size < 2) return;
		Node* current = m_pHead;
		Node* temp = nullptr;

		while (current) {
			temp = current->pPrev;
			current->pPrev = current->pNext;
			current->pNext = temp;
			current = current->pPrev;
		}

		temp = m_pHead;
		m_pHead = m_pTail;
		m_pTail = temp;
	}

	template<typename T>
	void List<T>::Unique() {
		if (m_Size < 2) return;
		Node* current = m_pHead;
		while (current && current->pNext) {
			if (current->Data == current->pNext->Data) {
				Node* toDelete = current->pNext;
				current->pNext = toDelete->pNext;
				if (toDelete->pNext) {
					toDelete->pNext->pPrev = current;
				}
				else {
					m_pTail = current;
				}
				delete toDelete;
				--m_Size;
			}
			else {
				current = current->pNext;
			}
		}
	}


	template <typename T>
	template< class BinaryPred >
	uint64 List<T>::Unique(BinaryPred pred) {
		if (m_Size < 2) return m_Size;
		Node* current = m_pHead;
		while (current && current->pNext) {
			if (pred(current->Data, current->pNext->Data)) {
				Node* toDelete = current->pNext;
				current->pNext = toDelete->pNext;
				if (toDelete->pNext) {
					toDelete->pNext->pPrev = current;
				}
				else {
					m_pTail = current;
				}
				delete toDelete;
				--m_Size;
			}
			else {
				current = current->pNext;
			}
		}
		return m_Size;
	}

	template<typename T>
	void List<T>::Merge(List& other) {
		if (other.Empty()) return;
		if (this->Empty()) {
			this->Swap(other);
			return;
		}
		Node* current = m_pHead;
		Node* otherCurrent = other.m_pHead;
		while (current && otherCurrent) {
			if (current->Data <= otherCurrent->Data) {
				current = current->pNext;
			}
			else {
				Node* toInsert = otherCurrent;
				otherCurrent = otherCurrent->pNext;

				// Détacher toInsert de other
				if (toInsert->pPrev) toInsert->pPrev->pNext = toInsert->pNext;
				if (toInsert->pNext) toInsert->pNext->pPrev = toInsert->pPrev;
				if (other.m_pHead == toInsert) other.m_pHead = toInsert->pNext;
				if (other.m_pTail == toInsert) other.m_pTail = toInsert->pPrev;

				// Insérer toInsert avant current
				toInsert->pPrev = current->pPrev;
				toInsert->pNext = current;
				if (current->pPrev) current->pPrev->pNext = toInsert;
				else m_pHead = toInsert;
				current->pPrev = toInsert;
				++m_Size;
				--other.m_Size;
			}
		}
		if (otherCurrent) {
			m_pTail->pNext = otherCurrent;
			otherCurrent->pPrev = m_pTail;
			m_pTail = other.m_pTail;
			m_Size += other.m_Size;
			other.m_pHead = other.m_pTail = nullptr;
			other.m_Size = 0;
		}
	}

	template<typename T>
	void List<T>::Merge(List&& other) {
		if (other.Empty()) return;
		if (this->Empty()) {
			m_pHead = other.m_pHead;
			m_pTail = other.m_pTail;
			m_Size = other.m_Size;
			other.m_pHead = nullptr;
			other.m_pTail = nullptr;
			other.m_Size = 0;
			return;
		}
		Node* current = m_pHead;
		Node* otherCurrent = other.m_pHead;
		while (current && otherCurrent) {
			if (current->Data <= otherCurrent->Data) {
				current = current->pNext;
			}
			else {
				Node* toInsert = otherCurrent;
				otherCurrent = otherCurrent->pNext;

				// Détacher toInsert de other
				if (toInsert->pPrev) toInsert->pPrev->pNext = toInsert->pNext;
				if (toInsert->pNext) toInsert->pNext->pPrev = toInsert->pPrev;
				if (other.m_pHead == toInsert) other.m_pHead = toInsert->pNext;
				if (other.m_pTail == toInsert) other.m_pTail = toInsert->pPrev;

				// Insérer toInsert avant current
				toInsert->pPrev = current->pPrev;
				toInsert->pNext = current;
				if (current->pPrev) current->pPrev->pNext = toInsert;
				else m_pHead = toInsert;
				current->pPrev = toInsert;
				++m_Size;
				--other.m_Size;
			}
		}
		if (otherCurrent) {
			m_pTail->pNext = otherCurrent;
			otherCurrent->pPrev = m_pTail;
			m_pTail = other.m_pTail;
			m_Size += other.m_Size;
			other.m_pHead = other.m_pTail = nullptr;
			other.m_Size = 0;
		}
	}

	template<typename T>
	template <typename Compare>
	void List<T>::Merge(List& other, Compare comp) {
		if (other.Empty()) return;
		if (this->Empty()) {
			this->Swap(other);
			return;
		}
		Node* current = m_pHead;
		Node* otherCurrent = other.m_pHead;
		while (current && otherCurrent) {
			if (comp(current->Data, otherCurrent->Data)) {
				current = current->pNext;
			}
			else {
				Node* toInsert = otherCurrent;
				otherCurrent = otherCurrent->pNext;

				// Détacher toInsert de other
				if (toInsert->pPrev) toInsert->pPrev->pNext = toInsert->pNext;
				if (toInsert->pNext) toInsert->pNext->pPrev = toInsert->pPrev;
				if (other.m_pHead == toInsert) other.m_pHead = toInsert->pNext;
				if (other.m_pTail == toInsert) other.m_pTail = toInsert->pPrev;

				// Insérer toInsert avant current
				toInsert->pPrev = current->pPrev;
				toInsert->pNext = current;
				if (current->pPrev) current->pPrev->pNext = toInsert;
				else m_pHead = toInsert;
				current->pPrev = toInsert;
				++m_Size;
				--other.m_Size;
			}
		}
		if (otherCurrent) {
			m_pTail->pNext = otherCurrent;
			otherCurrent->pPrev = m_pTail;
			m_pTail = other.m_pTail;
			m_Size += other.m_Size;
			other.m_pHead = other.m_pTail = nullptr;
			other.m_Size = 0;
		}
	}
	
	template<typename T>
	template <typename Compare>
	void List<T>::Merge(List&& other, Compare comp) {
		if (other.Empty()) return;
		if (this->Empty()) {
			m_pHead = other.m_pHead;
			m_pTail = other.m_pTail;
			m_Size = other.m_Size;
			other.m_pHead = nullptr;
			other.m_pTail = nullptr;
			other.m_Size = 0;
			return;
		}
		Node* current = m_pHead;
		Node* otherCurrent = other.m_pHead;
		while (current && otherCurrent) {
			if (comp(current->Data, otherCurrent->Data)) {
				current = current->pNext;
			}
			else {
				Node* toInsert = otherCurrent;
				otherCurrent = otherCurrent->pNext;

				// Détacher toInsert de other
				if (toInsert->pPrev) toInsert->pPrev->pNext = toInsert->pNext;
				if (toInsert->pNext) toInsert->pNext->pPrev = toInsert->pPrev;
				if (other.m_pHead == toInsert) other.m_pHead = toInsert->pNext;
				if (other.m_pTail == toInsert) other.m_pTail = toInsert->pPrev;

				// Insérer toInsert avant current
				toInsert->pPrev = current->pPrev;
				toInsert->pNext = current;
				if (current->pPrev) current->pPrev->pNext = toInsert;
				else m_pHead = toInsert;
				current->pPrev = toInsert;
				++m_Size;
				--other.m_Size;
			}
		}
		if (otherCurrent) {
			m_pTail->pNext = otherCurrent;
			otherCurrent->pPrev = m_pTail;
			m_pTail = other.m_pTail;
			m_Size += other.m_Size;
			other.m_pHead = other.m_pTail = nullptr;
			other.m_Size = 0;
		}
	}

	template<typename T>
	void List<T>::Splice(ConstIterator pos, List& other) {
		if (other.Empty()) return;
		Node* insertPos = const_cast<Node*>(pos.m_pNODE);

		if (!insertPos) { // Insertion en fin de liste
			if (!m_pTail) {
				m_pHead = other.m_pHead;
				m_pTail = other.m_pTail;
			}
			else {
				m_pTail->pNext = other.m_pHead;
				other.m_pHead->pPrev = m_pTail;
				m_pTail = other.m_pTail;
			}
		}
		else if (insertPos == m_pHead) { // Début de liste
			other.m_pTail->pNext = m_pHead;
			m_pHead->pPrev = other.m_pTail;
			m_pHead = other.m_pHead;
		}
		else { // Milieu
			Node* before = insertPos->pPrev;
			before->pNext = other.m_pHead;
			other.m_pHead->pPrev = before;
			other.m_pTail->pNext = insertPos;
			insertPos->pPrev = other.m_pTail;
		}
		m_Size += other.m_Size;
		other.m_pHead = other.m_pTail = nullptr;
		other.m_Size = 0;
	}

	template<typename T>
	void List<T>::Splice(ConstIterator pos, List&& other) {
		Splice(pos, static_cast<List&>(other));
	}

	template<typename T>
	void List<T>::Splice(ConstIterator pos, List& other, ConstIterator it) {
		Node* posNode = const_cast<Node*>(pos.m_pNODE);
		Node* itNode = const_cast<Node*>(it.m_pNODE);
		if (!itNode || &other == this) return;
		// Détacher itNode de other
		if (itNode->pPrev) itNode->pPrev->pNext = itNode->pNext;
		else other.m_pHead = itNode->pNext;
		if (itNode->pNext) itNode->pNext->pPrev = itNode->pPrev;
		else other.m_pTail = itNode->pPrev;
		other.m_Size--;

		// Insérer dans *this
		if (!posNode) { // Fin
			if (!m_pTail) {
				m_pHead = m_pTail = itNode;
				itNode->pPrev = itNode->pNext = nullptr;
			}
			else {
				m_pTail->pNext = itNode;
				itNode->pPrev = m_pTail;
				itNode->pNext = nullptr;
				m_pTail = itNode;
			}
		}
		else if (posNode == m_pHead) { // Début
			itNode->pNext = m_pHead;
			m_pHead->pPrev = itNode;
			m_pHead = itNode;
			itNode->pPrev = nullptr;
		}
		else { // Milieu
			itNode->pNext = posNode;
			itNode->pPrev = posNode->pPrev;
			posNode->pPrev->pNext = itNode;
			posNode->pPrev = itNode;
		}
		m_Size++;
	}

	template<typename T>
	void List<T>::Splice(ConstIterator pos, List&& other, ConstIterator it) {
		Splice(pos, static_cast<List&>(other), it);
	}

	template<typename T>
	void List<T>::Splice(ConstIterator pos, List& other, ConstIterator first, ConstIterator last) {
		Node* posNode = const_cast<Node*>(pos.m_pNODE);
		Node* firstNode = const_cast<Node*>(first.m_pNODE);
		Node* lastNode = const_cast<Node*>(last.m_pNODE);
		if (!firstNode || firstNode == lastNode || &other == this) return;

		// Déterminer la plage à déplacer
		Node* rangeStart = firstNode;
		Node* rangeEnd = lastNode ? lastNode->pPrev : other.m_pTail;
		if (!rangeEnd) return;

		// Détacher la plage de other
		Node* before = rangeStart->pPrev;
		Node* after = rangeEnd->pNext;
		if (before) before->pNext = after;
		else other.m_pHead = after;
		if (after) after->pPrev = before;
		else other.m_pTail = before;

		// Insérer dans *this
		if (!posNode) { // Fin
			if (!m_pTail) {
				m_pHead = rangeStart;
				m_pTail = rangeEnd;
				rangeStart->pPrev = nullptr;
				rangeEnd->pNext = nullptr;
			}
			else {
				m_pTail->pNext = rangeStart;
				rangeStart->pPrev = m_pTail;
				rangeEnd->pNext = nullptr;
				m_pTail = rangeEnd;
			}
		}
		else if (posNode == m_pHead) { // Début
			rangeEnd->pNext = m_pHead;
			m_pHead->pPrev = rangeEnd;
			m_pHead = rangeStart;
			rangeStart->pPrev = nullptr;
		}
		else { // Milieu
			rangeStart->pPrev = posNode->pPrev;
			rangeEnd->pNext = posNode;
			posNode->pPrev->pNext = rangeStart;
			posNode->pPrev = rangeEnd;
		}
		uint64 count = 0;
		for (Node* n = rangeStart; n != after; n = n->pNext) ++count;
		m_Size += count;
		other.m_Size -= count;
	}

	template<typename T>
	void List<T>::Splice(ConstIterator pos, List&& other, ConstIterator first, ConstIterator last) {
		Splice(pos, static_cast<List&>(other), first, last);
	}

	template<typename T>
	typename List<T>::Iterator List<T>::Emplace(ConstIterator pos, T&& value) {
		Node* posNode = const_cast<Node*>(pos.m_pNODE);
		Node* n = new Node(Forward<T>(value));

		if (!posNode) { // Insertion en fin de liste
			if (!m_pTail) { // Liste vide
				m_pHead = m_pTail = n;
			}
			else {
				m_pTail->pNext = n;
				n->pPrev = m_pTail;
				m_pTail = n;
			}
		}
		else if (posNode == m_pHead) { // Insertion en début de liste
			n->pNext = m_pHead;
			m_pHead->pPrev = n;
			m_pHead = n;
		}
		else { // Insertion au milieu
			n->pNext = posNode;
			n->pPrev = posNode->pPrev;
			if (posNode->pPrev) {
				posNode->pPrev->pNext = n;
			}
			posNode->pPrev = n;
		}
		++m_Size;
		return Iterator(n);
	}

	template <typename T>
	inline T& List<T>::EmplaceBack(T&& value) {
		Node* n = new Node(Forward<T>(value));
		if (!m_pTail) {
			m_pHead = m_pTail = n;
		}
		else {
			m_pTail->pNext = n;
			n->pPrev = m_pTail;
			m_pTail = n;
		}
		++m_Size;
		return m_pTail->Data;
	}

	template <typename T>
	T& List<T>::EmplaceFront(T&& value) {
		Node* n = new Node(Forward<T>(value));
		if (!m_pHead) {
			m_pHead = m_pTail = n;
		}
		else {
			n->pNext = m_pHead;
			m_pHead->pPrev = n;
			m_pHead = n;
		}
		++m_Size;
		return m_pHead->Data;
	}

	template <typename T>
	typename List<T>::Iterator List<T>::Erase(Iterator pos) {
		Node* toDelete = pos.m_pNode;
		if (!toDelete) return End();
		if (toDelete->pPrev) {
			toDelete->pPrev->pNext = toDelete->pNext;
		}
		else {
			m_pHead = toDelete->pNext;
		}
		if (toDelete->pNext) {
			toDelete->pNext->pPrev = toDelete->pPrev;
		}
		else {
			m_pTail = toDelete->pPrev;
		}
		Iterator nextIt(toDelete->pNext);
		delete toDelete;
		--m_Size;
		return nextIt;
	}


	template <typename T>
	typename List<T>::Iterator List<T>::Erase(ConstIterator pos) {
		Node* toDelete = const_cast<Node*>(pos.node);
		if (!toDelete) return End();
		if (toDelete->pPrev) {
			toDelete->pPrev->pNext = toDelete->pNext;
		}
		else {
			m_pHead = toDelete->pNext;
		}
		if (toDelete->pNext) {
			toDelete->pNext->pPrev = toDelete->pPrev;
		}
		else {
			m_pTail = toDelete->pPrev;
		}
		Iterator nextIt(toDelete->pNext);
		delete toDelete;
		--m_Size;
		return nextIt;
	}


	template <typename T>
	typename List<T>::Iterator List<T>::Erase(Iterator first, Iterator last) {
		Node* startNode = first.m_pNode;
		Node* endNode = last.m_pNode;

		if (!startNode || startNode == endNode) return last;

		// Relier le précédent de startNode à endNode
		if (startNode->pPrev) {
			startNode->pPrev->pNext = endNode;
		}
		else {
			m_pHead = endNode;
		}
		if (endNode) {
			endNode->pPrev = startNode->pPrev;
		}
		else {
			m_pTail = startNode->pPrev;
		}

		while (startNode != endNode) {
			Node* toDelete = startNode;
			startNode = startNode->pNext;
			delete toDelete;
			--m_Size;
		}

		return Iterator(endNode);
	}


	template <typename T>
	typename List<T>::Iterator List<T>::Erase(ConstIterator first, ConstIterator last) {
		Node* startNode = const_cast<Node*>(first.node);
		Node* endNode = const_cast<Node*>(last.node);
		if (!startNode || startNode == endNode) return last;

		if (startNode->pPrev) {
			startNode->pPrev->pNext = endNode;
		}
		else {
			m_pHead = endNode;
		}
		if (endNode) {
			endNode->pPrev = startNode->pPrev;
		}
		else {
			m_pTail = startNode->pPrev;
		}

		while (startNode != endNode) {
			Node* toDelete = startNode;
			startNode = startNode->pNext;
			delete toDelete;
			--m_Size;
		}
		return Iterator(endNode);
	}



	template<typename T>
	typename List<T>::Iterator List<T>::Begin() {
		return Iterator(m_pHead);
	}

	template<typename T>
	typename List<T>::ConstIterator List<T>::CBegin() const {
		return ConstIterator(m_pHead);
	}

	template<typename T>
	typename List<T>::Iterator List<T>::End() {
		return Iterator(nullptr);
	}

	template<typename T>
	typename List<T>::ConstIterator List<T>::CEnd() const {
		return ConstIterator(nullptr);
	}

	template<typename Iterator>
	inline void Advance(Iterator& it, int n) {
		if (n >= 0) {
			while (n--) ++it;
		}
		else {
			while (n++) --it;
		}
	}

	template<typename U>
	inline void _Swap(U& a, U& b) {
		U tmp = a;
		a = b;
		b = tmp;
	}

	template<typename U>
	inline U&& Forward(U& arg) {
		return static_cast<U&&>(arg);
	}

	template <typename Iterator>
	inline uint64 Distance(Iterator first, Iterator last) {
		uint64 count = 0;
		while (first != last) {
			++first;
			++count;
		}
		return count;
	}
}

#endif