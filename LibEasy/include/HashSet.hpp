/*
 * File Name:	HashSet.hpp
 * Date/Time:
 * 	Start: 	2019-12-13 17:27 week:5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_HASHSET_H_
#define _EASY_HASHSET_H_

#include <cstring>

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "String.hpp"
#include "System.hpp"
#include "AutoPointer.hpp"
#include "Container.hpp"

//#include "Console.hpp"

namespace Easy {

/**
	\class HashSet
	
	\brief 哈希表实现.
*/

template<class E>
class HashSet {
	
class Node {
public:
	E     m_item;
	Node* m_next;
	e_int m_hashCode;
	
	Node(const E& item, e_int h) {
		m_item     = item;
		m_hashCode = h;
		m_next     = nullptr;
	}
	
	Node(E&& item, e_int h) {
		m_item     = System::move(item);
		m_hashCode = h;
		m_next     = nullptr;
	}
	
	~Node() {}
}; // Node

class HsIterator : public Iterator<E> {
private:
	const HashSet* m_set;
	e_int m_expectSize;
public:
	HsIterator(const HashSet* set, Node** table, e_int capacity, e_int length) {
		m_set 	   = set;
		m_table    = table;
		m_capacity = capacity;
		m_index	   = 0;
		m_expectSize = length;
		_getCur();
	}
	
	e_bool isEnd() const override {
		return m_cur == nullptr;
	}
	
	E& current() const override {
		if (isEnd()) 
			throw IteratorIsEndException{__func__, __LINE__};
		if (m_set->size() != m_expectSize)
			throw ConcurrentModificationException(__func__, __LINE__, L"HashSet");
		return m_cur->m_item;
	}
	
	e_bool advance() override {
		if (m_cur == nullptr) return false;
		m_cur = m_cur->m_next;
		if (m_cur == nullptr) {
			m_index++;
			_getCur();
		}
		return true;
	}
	
	~HsIterator() {}
private:
	Node** m_table;
	e_int  m_capacity;
	e_int  m_index;
	Node*  m_cur;
	
	e_void _getCur() {
		for (e_int i=m_index; i<m_capacity; i++) {
			if (m_table[i] != nullptr) {
				m_cur = m_table[i];
				return;
			} else {
				m_index++;
			}
		}
		m_cur = nullptr;
	}
	
	HsIterator(HsIterator& copy) = delete;
	HsIterator(HsIterator&& move) = delete;
	HsIterator& operator=(HsIterator& copy) = delete;
	HsIterator& operator=(HsIterator&& move) = delete;
};

private:
	Node** m_table;
	e_int m_capacity;
	e_int m_size;
	
	constexpr e_int threshold() {return (e_int) (m_capacity * 0.75);}
	
public:
	/**
	 * \brief 构造函数.
	 *
	 * 如果 initCapacity < 16, 函数会将其修改为 16. 
	 * @param initCapacity 初始表容量, 默认为 16.
	*/
	explicit HashSet(e_int initCapacity=16) {
		if (initCapacity < 16)
			initCapacity = 16;
		_init(initCapacity);
	}
	
		/**
		\brief 析构函数.
	*/
	inline ~HashSet(){
		_dispose();
	}
	
	/**
		\brief copy 构造函数.
		
		@param copy
	*/
	inline HashSet(const HashSet& copy) {
		_init();
		_copy(copy);
	}
	
	/**
		\brief move 构造函数.
		
		@param move
	*/
	inline HashSet(HashSet&& move) {
		m_table         = move.m_table;
		m_capacity      = move.m_capacity;
		m_size 		 	= move.m_size;
		move.m_table 	= new Node*[16];
		move.m_capacity = 16;
		move.m_size 	= 0;
		for (e_int i=0; i<move.m_capacity; i++) {
			move.m_table[i] = nullptr;
		}
		
	}
	
	/**
		\brief copy 赋值.
		
		@param copy
	*/
	HashSet& operator=(const HashSet& copy) {
		_dispose();
		_init();
		_copy(copy);
		return *this;
	}
	
	/**
		\brief move 赋值.
		
		@param move
	*/
	HashSet& operator=(HashSet&& move) {
		_dispose();
		m_table         = move.m_table;
		m_capacity      = move.m_capacity;
		m_size 		 	= move.m_size;
		move.m_table 	= new Node*[16];
		move.m_capacity = 16;
		move.m_size 	= 0;
		for (e_int i=0; i<move.m_capacity; i++) {
			move.m_table[i] = nullptr;
		}
		return *this;
	}
	
	/**
		\brief 等于函数.

		集合元素需实现 equals() 方法.

		@param other 一个集合.
		@return 如果接受者和 other 的元素对应相等, 返回真; 否则, 返回假.
	*/
	inline e_bool equals(const HashSet<E>& other) const {
		return Container::equals<HashSet<E>, HashSet<E>, E>(*this, other);
	}

	/**
		\brief 比较函数.

		集合元素需实现 compare() 方法.

		@param other 集合对象.
		@return 如果接受者的元素按顺序大于 other 的元素, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	inline e_int compare(const HashSet<E>& other) const {
		return Container::compare<HashSet<E>, HashSet<E>, E>(*this, other);
	}

	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	inline e_int hashCode() const {return Long::hashCode(reinterpret_cast<e_long>(this));}

	/**
	 * \brief 转到字符串.
	 * 
	 * 集合的元素需实现 toString() 方法.
	 * 
	 * @return 接受者的字符串表示.
	*/
	inline String toString() const {
		return System::move(Container::toString<HashSet<E>, E>(*this));
	}
	
	/**
		\brief 添加元素.
		
		如果接受者中包含等价元素, 函数会再次添加元素.

		@param item 要添加的元素.
		@return 被添加的元素.
	*/
	E& add(const E& item) {
		if (m_size > threshold()) {
			_rehash(m_capacity*2);
		}
		
		e_int hash = item.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		
		if (m_table[slot] == nullptr) {
			m_table[slot] = new Node(item, hash);
			m_size++;
			return m_table[slot]->m_item;
		} else {
			Node* cur = m_table[slot];
			Node* prev = cur;
			while (cur) {
				prev = cur;
				cur = cur->m_next;
			}
			prev->m_next = new Node(item, hash);
			m_size++;
			return prev->m_next->m_item;
		}
	}
	
	/**
		\brief 添加元素.
		
		如果接受者中包含等价元素, 函数会再次添加元素.

		@param item 要添加的元素.
		@return 被添加的元素.
	*/
	E& add(E&& item) {
		if (m_size > threshold()) {
			_rehash(m_capacity*2);
		}

		e_int hash = item.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		
		if (m_table[slot] == nullptr) {
			m_table[slot] = new Node(System::move(item), hash);
			m_size++;
			return m_table[slot]->m_item;
		} else {
			Node* cur  = m_table[slot];
			Node* prev = cur;
			while (cur) {
				prev = cur;
				cur  = cur->m_next;
			}
			prev->m_next = new Node(System::move(item), hash);
			m_size++;
			return prev->m_next->m_item;
		}
	}
	
	/**
	 * \brief 删除元素.
	 * 
	 * 元素类型需实现 hashCode() 及 equals() 方法.
	 * 
	 * @param item 与要删除的元素等价的元素.
	 * @return 被删除的元素.
	 * @throw 如果接受者不包含元素 item, 抛出 HasNoSuchElementException.
	*/
	E remove(const E& item) {
		e_int hash = item.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		Node* cur = m_table[slot];
		Node* prev = cur;
		while (cur) {
			if (cur->m_item.equals(item)) {
				break;
			}
			prev = cur;
			cur = cur->m_next;
		}
		
		if (cur == nullptr) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"HashSet"}};
		}
		
		if (cur == m_table[slot]) {
			m_table[slot] = cur->m_next;
		} else {
			prev->m_next = cur->m_next;
		}
		
		E moveItem = System::move(cur->m_item);
		delete cur;
		m_size--;
		return System::move(moveItem);
	}

	/**
	 * \brief 获取元素.
	 * 
	 * @param item 与欲获取元素等价的元素.
	 * @return 接受者中与 item 等价的元素.
	 * @throw 如果接受者空或不包含 item, 抛出 HasNoSuchElementException.
	*/
	E& get(const E& item) const {
		e_int hash = item.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		Node* cur = m_table[slot];
		while (cur) {
			if (cur->m_item.equals(item)) {
				break;
			}
			cur = cur->m_next;
		}
		if (cur == nullptr) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"HashSet"}};
		}
		
		return cur->m_item;
	}
	
	/**
		\brief 判断接受者是否包含指定元素.
		
		元素类型需实现 hashCode() 以及 equals() 方法.
		
		@param item 欲查找的项.
		@return 如果接受者包含 item, 返回真; 否则, 返回假.
	*/
	e_bool contains(const E& item) const {
		e_int hash = item.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		Node* cur  = m_table[slot];
		while (cur) {
			if (cur->m_item.equals(item)) {
				return true;
			}
			cur = cur->m_next;
		}
		return false;
	}
	
	/**
		\brief 缩减内存.
		
		@return 缩减内存后的接受者.
	*/
	HashSet& trimCapacity() {
		if (m_capacity == 16)
			return *this;
		if (m_size != m_capacity) {
			_rehash((m_size<16)?16:m_size);
		}
		return *this;
	}
	
	/**
	 * \brief 获取元素个数.
	 * 
	 * @return 接受者包含的元素个数.
	*/
	inline e_int size() const {
		return m_size;
	}

	/**
	 * \brief 检查集合是否为空.
	 * 
	 * @return 如果集合为空, 返回真; 否则, 返回假.
	*/
	inline e_bool empty() const {
		return m_size == 0;
	}
	
	/**
	 * \brief 查看当前使用内存, 以元素大小计.
	 * 
	 * @return 接受者当前使用内存大小.
	*/
	inline e_int capacity() const {
		return m_capacity;
	}
	
	/**
	 * \brief 清空容器.
	 * 
	 * 清空内部缓存区并重新初始化.
	*/
	inline e_void clean() {
		_dispose();
		_init();
	}
	
	/**
	 * \brief 获取迭代器.
	 * 
	 * @return 接受者的迭代器.
	*/
	inline Iterator<E>* iterator() const {
		return new HsIterator{this, m_table, m_capacity, m_size};
	}
private:
	e_void _init(e_int initCapacity=16) {
		m_capacity = initCapacity;
		m_size = 0;
		
		m_table = new Node*[m_capacity];
		for (e_int i=0; i<m_capacity; i++) {
			m_table[i] = nullptr;
		}
	}
	
	e_void _copy(const HashSet& copy) {
		AutoPointer<Iterator<E>> iter{copy.iterator()};
		while (!iter->isEnd()) {
			this->add(iter->current());
			iter->advance();
		}
	}
	
	e_void _dispose() {
		for (e_int i=0; i<m_capacity; i++) {
			Node* cur = m_table[i];
			while (cur) {
				Node* old = cur;
				cur = cur->m_next;
				delete old;
			}
		}
		
		delete [] m_table;
	}
	
	e_void _rehash(e_int newCapacity) {
		e_int oldCapacity = m_capacity;
		m_capacity = newCapacity;
		
		Node** newTable = new Node*[m_capacity];
		
		for (e_int i=0; i<m_capacity; i++) {
			newTable[i] = nullptr;
		}
		
		for (e_int i=0; i<oldCapacity; i++) {
			if (m_table[i] != nullptr) {
				Node* cur = m_table[i];
				while (cur) {
					e_int newSlot = cur->m_hashCode % m_capacity;
					if (newTable[newSlot] == nullptr) {
						newTable[newSlot] = new Node((E&&)cur->m_item, cur->m_hashCode);
					} else {
						Node* newCur = newTable[newSlot];
						Node* prev = newCur;
						while (newCur) {
							prev = newCur;
							newCur = newCur->m_next;
						}
						prev->m_next = new Node((E&&)cur->m_item, cur->m_hashCode);
					}
					Node* old = cur;
					cur = cur->m_next;
					delete old;
				}
			}
		}
		
		delete [] m_table;
		m_table = newTable;
	}
}; // HashSet
} // Easy

#endif // _EASY_HASHSET_H_

