/*
 * File Name:	List.hpp
 * Date/Time:
 * 	Start: 	2019-12-05 15:39 week:4
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_LIST_H_
#define _EASY_LIST_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Container.hpp"
#include "Iterator.hpp"
#include "System.hpp"
#include "AutoPointer.hpp"

namespace Easy {

/**
 * \class List
 *
 * \brief Double-linked list.
 * 
 * @author Tes Alone
 * @since 1.0
*/

template<class E>
class List {
private:
class Node {
	public:
		E     m_item;
		Node* m_next;
		Node* m_front;
		
		Node(const E& item, Node* front, Node* next) {
			m_item  = item;
			m_front = front;
			m_next  = next;
			
			if (m_front != nullptr) {
				m_front->m_next = this;
			}
			
			if (m_next != nullptr) {
				m_next->m_front = this;
			}
		}

		Node(E&& item, Node* front, Node* next) {
			m_item  = System::move(item);
			m_front = front;
			m_next  = next;
			
			if (m_front != nullptr) {
				m_front->m_next = this;
			}
			
			if (m_next != nullptr) {
				m_next->m_front = this;
			}
		}
};

class LsIterator : public Iterator<E> {
	Node* m_curr;
	const List* m_list;
	e_int m_expectSize;
public:
	LsIterator(const List* list, Node* curr, e_int length) : Iterator<E>() {
		m_list = list;
		m_curr = curr;
		m_expectSize = length;
	}
	
	~LsIterator() {}

	e_bool	isEnd() const override {
		return m_curr->m_next == nullptr;
	}

	E& current() const  override {
		if (isEnd())
			throw IteratorIsEndException{__func__, __LINE__};
		if (m_list->size() != m_expectSize)
			throw ConcurrentModificationException(__func__, __LINE__, L"List");
		return m_curr->m_next->m_item;
	}
	
	e_bool advance()  override {
		if (m_curr->m_next == nullptr) return false;
		m_curr = m_curr->m_next;
		return true;
	}
private:
	LsIterator(const LsIterator& copy) = delete;
	LsIterator& operator=(const LsIterator& copy) = delete;
	LsIterator(LsIterator&& move) = delete;
	LsIterator& operator=(LsIterator&& move) = delete;
}; // LsIterator
	
	Node* m_head;
	Node* m_tail;
	Node* m_curr;
	e_int m_size;
	e_int m_index;

public:
	/**
	 * \brief ctor.
	 * 
	 * Create a empty list.
	*/
	inline List() {
		_init();
	}

	/**
	 * \brief dtor.
	*/
	inline ~List() {
		_dispose();
	}

	/**
	 * \brief copy ctor.
	 * 
	 * 
	 * @param copy 
	*/
	inline List(const List& copy) {
		_init();
		_copy(copy);
	}

	/**
	 * \brief move ctor.
	 * 
	 * @param move 
	*/
	List(List&& move) {
		m_head = new Node(E{}, nullptr, move.m_head->m_next);
		m_tail = new Node(System::move(move.m_tail->m_item), move.m_tail->m_front, nullptr);
		m_curr = m_head->m_next;
		m_size = move.m_size;
		m_index = -1;
		move.m_head->m_next = nullptr;
		move.m_tail = move.m_head;
		move.m_curr = nullptr;
		move.m_size = 0;
		move.m_index = -1;
	}

	/**
	 * \brief copy assignment.
	 * 
	 * @param copy 
	*/
	List& operator=(const List& copy) {
		_dispose();
		_init();
		_copy(copy);
		return *this;
	}
	
	/**
	 * \brief move assignment.
	 * 
	 * @param move 
	*/
	List& operator=(List&& move) {
		_dispose();
		m_head = new Node(E{}, nullptr, move.m_head->m_next);
		m_tail = new Node(System::move(move.m_tail->m_item), move.m_tail->m_front, nullptr);
		m_curr = m_head->m_next;
		m_size = move.m_size;
		m_index = -1;
		move.m_head->m_next = nullptr;
		move.m_tail = move.m_head;
		move.m_curr = nullptr;
		move.m_size = 0;
		move.m_index = -1;
		return *this;
	}
	
	/**
	 * \brief Check equality.
	 * 
	 * Element type need implements equals().
	 * <br/>
	 * 
	 * If receiver and other has the same length, then
	 * Compare receiver and other, one by one, if they are all equals, return true;
	 * otherwise return false.
	 * If their length not equal, return false.
	 * 
	 * @param other the object compared to receiver.
	 * @return result.
	 * */
	inline e_bool equals(const List<E>& other) const {
		return Container::equals<List<E>, List<E>, E>(*this, other);
	}

	/**
	 * \brief Check order.
	 * 
	 * Element type need implements compare().
	 * <br/>
	 * 
	 * Compare receiver's elements and other's, one by one. 
	 * If reach a point that receiver's element bigger than
	 * the other's return 1, less than return -1; if reach 
	 * a object's end and their length equal, return 0;
	 * if reach a object's end and their length not equal,
	 * if receiver is longer, return 1, otherwise return -1.
	 * 
	 * @param other the object compared to receiver.
	 * @return result.
	 * */
	inline e_int compare(const List<E>& other) const {
		return Container::compare<List<E>, List<E>, E>(*this, other);
	}

	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	inline e_int hashCode() const {return Long::hashCode(reinterpret_cast<e_long>(this));}

	/**
	 * \brief To string．
	 *
	 * Element type need implements toString().
	 * <br/>
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {
		return System::move(Container::toString<List<E>, E>(*this));
	}

	/**
	 * \brief Add element.
	 * 
	 * Add item to receiver's end, and change current pos to end.
	 * <br/>
	 * Using copis.
	 * 
	 * @param item .
	 * @return the element just added to receiver, not item.
	*/
	E& add(const E& item) {
		m_tail = new Node(item, m_tail, nullptr);
		m_curr = m_tail;
		m_size++;
		m_index++;
		return m_tail->m_item;
	}

	/**
	 * \brief Add element.
	 * 
	 * Add item to receiver's end, and change current pos to end.
	 * <br/>
	 * Using moves.
	 * 
	 * @param item .
	 * @return the element just added to receiver, not item.
	*/
	E& add(E&& item) {
		m_tail = new Node(System::move(item), m_tail, nullptr);
		m_curr = m_tail;
		m_size++;
		m_index++;
		return m_tail->m_item;
	}

	/**
	 * \brief Get element.
	 * 
	 * Get the index-th element of receiver.
	 * 
	 * @param index position of getting.
	 * @return result.
	 * @throw if index<0 || index>=List::size(), throws IndexOutOfBoundsExeption.
	*/
	inline E& get(e_int index) { // never can be const.
		moveTo(index);
		return get();
	}
	
	/**
	 * \brief Get first element.
	 * 
	 * Get the first element of receiver.
	 * 
	 * @return the first element of receiver.
	 * @throw  if container is empty, throws EmptyContainerException.
	 * */
	inline E& getFirst() const {
		if (m_size == 0) {
			throw EmptyContainerException{__func__, __LINE__, L"List"};
		}
		return m_head->m_next->m_item;
	}
	
	/**
	 * \brief Get last element.
	 * 
	 * Get the last element of receiver.
	 * 
	 * @return the last element of receiver.
	 * @throw  if container is empty, throws EmptyContainerException.
	 * */
	inline E& getLast() const {
		if (m_size == 0) {
			throw EmptyContainerException{__func__, __LINE__, L"List"};
		}
		return m_tail->m_item;
	}

	/**
	 * \brief Get element at index.
	 * 
	 * Get the index-th element of receiver.
	 * 
	 * @param index position of getting.
	 * @return result.
	 * @throw if index<0 || index>=List::size(), throws IndexOutOfBoundsExeption.
	*/
	inline E& operator[] (e_int index) { // never can be const.
		return get(index);
	}

	/**
	 * \brief Get current element.
	 * 
	 * Get current element of receiver.
	 * <br/>
	 * The current pos can be set by List::moveTo(), and may change by List::add(),
	 * List::remove(), List::removeFirst(), List::removeLast().
	 * 
	 * @return current element of receiver.
	 * @throw if container is empty, throws EmptyContainerException.
	*/
	inline E& get() const {
		if (m_size == 0) 
			throw  EmptyContainerException{__func__, __LINE__, String{L"List"}};
		return m_curr->m_item;
	}
	
	/**
		\brief Insert element.
		
		Insert element to receiver's index-th spot.
		
		<br/>
		The method do not changed current pos.
		
		@param index position of inserting.
		@param item .
		@return the element just inserted, not item.
		@throw if index < 0 || index >= List::size(), throws IndexOutOfBoundsException.
	*/
	inline E& insert(e_int index, const E& item) {
		moveTo(index);
		return insert(item);
	}

	/**
		\brief Insert element.
		
		Insert element to receiver's index-th spot.
		<br/>
		The method do not changed current pos.
		
		@param index position of inserting.
		@param item .
		@return the element just inserted, not item.
		@throw if index < 0 || index >= List::size(), throws IndexOutOfBoundsException.
	*/
	inline E& insert(e_int index, E&& item) {
		moveTo(index);
		return insert(System::move(item));
	}
	
	/**
		\brief Insert element.
		
		Insert element to receiver's current pos.
		<br/>
	 	The current pos can be set by List::moveTo(), and may change by List::add(),
	 	List::remove(), List::removeFirst(), List::removeLast().
		<br/>
		The method do not changed current pos.
		
		@param item .
		@return the element just inserted, not item..
	*/
	E& insert(const E& item) {
		if (m_size == 0) {
			return add(item);
		}
		Node* node = new Node(item, m_curr, m_curr->m_next);
		if (node->m_next == nullptr) {
			m_tail = node;
		}
		m_size++;
		return node->m_item;
	}

	/**
		\brief Insert element.
		
		Insert element to receiver's current pos.
		<br/>
	 	The current pos can be set by List::moveTo(), and may change by List::add(),
	 	List::remove(), List::removeFirst(), List::removeLast().
		<br/>
		The method do not changed current pos.
		
		@param item .
		@return the element just inserted, not item..
	*/
	E& insert(E&& item) {
		if (m_size == 0) {
			return add(System::move(item));
		}
		Node* node = new Node(System::move(item), m_curr, m_curr->m_next);
		if (node->m_next == nullptr) {
			m_tail = node;
		}
		m_size++;
		return node->m_item;
	}
	
	/**
	 * \brief Remove first element.
	 * 
	 * Remove the first element of receiver.
	 * <br/>
	 * This method may change current pos.
	 * 
	 * @return the removed element's copy.
	 * @throw if container is empty, throws EmptyContainerException.
	*/
	E removeFirst() {
		if (m_size == 0)
			throw  EmptyContainerException{__func__, __LINE__, String{L"List"}};
		E item = System::move(m_head->m_next->m_item);
		
		if (m_curr == m_head->m_next) {
			m_curr = m_head->m_next->m_next;
		}
		
		Node* del  = m_head->m_next;
		Node* next = m_head->m_next->m_next;
		
		m_head->m_next = next;
		
		if (next != nullptr) {
			next->m_front = m_head;
		} else { // there is no node now.
			m_tail  = m_head;
			m_index = -1;
		}
		
		delete del;
		m_size--;
		return System::move(item);
	}

	/**
	 * \brief Remove last element.
	 * 
	 * Remove the last element of receiver.
	 * <br/>
	 * This method may change current pos.
	 * 
	 * @return the removed element's copy.
	 * @throw if container is empty, throws EmptyContainerException.
	*/
	E removeLast() {
		if (m_size == 0)
			throw  EmptyContainerException{__func__, __LINE__, String{L"List"}};
		E item = System::move(m_tail->m_item);
		if (m_curr == m_tail) {
			m_curr = m_curr->m_front;
			if (m_curr == m_head) { // there is no node now
				m_curr = nullptr;
				m_index = -1;
			} else {
				m_index--;
			}
		}
		Node* del   = m_tail;
		Node* front = m_tail->m_front;		
		m_tail = front;
		front->m_next = nullptr;
		delete del;
		m_size--;
		return System::move(item);
	}

	/**
	 * \brief Remove current element.
	 * 
	 * Remove the current element of receiver.
	 * <br/>
	 * The current pos can be set by List::moveTo(), and may change by List::add(),
	 * List::remove(), List::removeFirst(), List::removeLast().
	 * <br/>
	 * 
	 * @return the removed element's copy.
	 * @throw if container is empty, throws EmptyContainerException.
	*/
	E remove() {
		if (m_size == 0)
			throw  EmptyContainerException{__func__, __LINE__, String{L"List"}};
	
		E item = System::move(m_curr->m_item);
		Node* move  = m_curr;
		Node* front = move->m_front;
		Node* next  = move->m_next;
		
		front->m_next = next;
		
		if (next != nullptr) {
			next->m_front = front;
			m_curr = next; // need not update m_index, the curr pos not change.
		} else {
			m_tail = m_curr->m_front;
			if (m_tail == m_head) { // there is no node
				m_curr  = nullptr;
				m_index = -1;
			} else {
				m_curr = m_tail;
			}
		}
		
		delete move;
		m_size--;
		
		return System::move(item);
	}

	/**
	 * \brief Remove element.
	 * 
	 * Remove the index-th element of receiver.
	 * <br/>
	 * The current pos can be set by List::moveTo(), and may change by List::add(),
	 * List::remove(), List::removeFirst(), List::removeLast().
	 * <br/>
	 * 
	 * @param index position of removing.
	 * @return the removed element's copy.
	 * @throw if index < 0 || index >= List::size(), throws IndexOutOfBoundsException.
	*/
	inline E remove(e_int index) {
		moveTo(index);
		return System::move(remove());
	}

	/**
		\brief Find element.

		From offset, check whether there is a element of receiver equals item, 
		one by one, if so, return the element's index, else return -1.
		<br/>
		Element type need implements equals().
		If offset < 0 or offset >= List::size() return -1.
		
		@param item .
		@param offset finding start, defualt 0, range betweens 0 ~ List::size()-1.
		@return result.
	*/
	e_int indexOf(const E& item, e_int offset=0) const {
		if (offset<0 || offset>=m_size)
			return -1;

		Node* curr  = m_head->m_next;
		e_int index = 0;
		for (; index<offset; index++) {
			curr = curr->m_next;
		}
		
		while (curr) {
			if (curr->m_item.equals(item)) {
				return index;
			}
			index++;
			curr = curr->m_next;
		}
		
		return -1;
	}

	/**
		\brief Finding element forward.

		From offset, forward, check whether there is a element of receiver equals item, 
		one by one, if so, return the element's index, else return -1.
		If offset < 0 or offset >= List::size() return -1.
		<br/>
		Element type need implements equals().
		
		@param item .
		@param offset findind start, defualt -1, indicates List::size() -1, range betweens 0 ~ List::size() -1.
		@return result.
	*/
	e_int lastIndexOf(const E& item, e_int offset=-1) const {
		if (offset == -1) offset = m_size - 1;
		if (offset<0 || offset>=m_size) 
			return -1;

		Node* curr = m_tail;
		e_int index = m_size-1;
		for (; index>offset; index--) {
			curr = curr->m_front;
		}

		while (curr) {
			if (curr->m_item.equals(item))
				return index;
			index--;
			curr = curr->m_front;
		}

		return -1;
	}
	
	/**
		\brief Check whether contains some element.
		
		Element type need implements equals().
		
		@param item .
		@return if contains return true, otherwise return false.
	*/
	inline e_bool contains(const E& item) const {
		return this->indexOf(item) != -1;
	}
	
	/**
	 * \brief Get length.
	 * 
	 * @return receiver's current length.
	*/
	inline e_int size() const {
		return m_size;
	}

	/**
	 * \brief Check whether is empty.
	 * 
	 * If receiver's length is 0, return true, else return false.
	 * 
	 * @return result.
	*/
	inline e_bool empty() const {
		return m_size == 0;
	}

	/**
		\brief Move current Pos.
		
		@param index new pos, range betweens 0 ~ List::size()-1.
		@throw if index < 0 || index >= List::size(), throws IndexOutOfBoundsException.
	*/
	e_void moveTo(e_int index)
	{		
		if (index<0 || index>=m_size)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"List"}, m_size, index};
		
		if (index == 0) {
			m_index = 0;
			m_curr = m_head->m_next;
			return;
		}
		
		if (index == m_size-1) {
			m_index = index;
			m_curr = m_tail;
			return;
		}
		
		e_int step = index - m_index;
		if (step < 0) {
			e_int t = -step;
			for (e_int i=0; i<t; i++) {
				m_curr = m_curr->m_front;
			}
		} else if (step > 0) {
			for (e_int i=0; i<step; i++) {
				m_curr = m_curr->m_next;
			}
		}
		m_index = index;
	}
	
	/**
	 * \brief Clean.
	 * 
	 * Clean and then reinitialize receiver.
	*/
	inline e_void clean() {
		_dispose();
		_init();
	}
	
	/**
	 * \brief Get Iterator.
	 * 
	 * @return receiver's iterator.
	*/
	inline Iterator<E>* iterator() const {
		return new LsIterator{this, m_head, m_size};
	}
private:
	e_void _init() {
		m_head  = new Node(E{}, nullptr, nullptr);
		m_tail  = m_head;
		m_curr  = nullptr;
		m_size  = 0;
		m_index = -1;
	}
	
	e_void _copy(const List<E>& copy) {
		AutoPointer<Iterator<E>> iter{copy.iterator()};
		while (!iter->isEnd()) {
			this->add(iter->current());
			iter->advance();
		}
	}
	
	e_void _dispose() {
		m_curr = m_head->m_next;
		delete m_head;
		while (m_curr) {
			Node* old = m_curr;
			m_curr = m_curr->m_next;
			delete old;
		}
	}
}; // List
} // Easy

#endif // _EASY_LIST_H_

