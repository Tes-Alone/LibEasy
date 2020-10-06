/*
 * File Name:	HashMap.hpp
 * Date/Time:
 * 	Start: 	2019-12-14 19:44 week:6
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_HASHMAP_H_
#define _EASY_HASHMAP_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "MapEntry.hpp"
#include "Container.hpp"
#include "AutoPointer.hpp"
#include "System.hpp"

#include "HashSet.hpp"
//#include "Console.hpp"

namespace Easy {

/**
	\class HashMap
	
	\brief Hash table map implement.
	
	This class allows same keys exists.
	
	@author Tes Alone
	@since 1.0
*/
template<class K, class V>
class HashMap {

class Node {
public:
	MapEntry<K, V> m_entry;
	Node* m_next;
	e_int m_hashCode;
	
	Node(const MapEntry<K, V>& entry, e_int h) {
		m_entry    = entry;
		m_hashCode = h;
		m_next     = nullptr;
	}
	
	Node(MapEntry<K, V>&& entry, e_int h) {
		m_entry    = System::move(entry);
		m_hashCode = h;
		m_next     = nullptr;
	}
	
	~Node() {}
}; // Node

class HmIterator : public Iterator<MapEntry<K,V>> {
private:
	const HashMap* m_map; // for access modify problem.
	e_int m_expectSize; // for access modify problem.
public:
	HmIterator(const HashMap* map, Node** table, e_int capacity, e_int length) {
		m_map 	   = map;
		m_table    = table;
		m_capacity = capacity;
		m_index	   = 0;
		m_expectSize = length;
		_getCur();
	}
	
	e_bool isEnd() const override {
		return m_cur == nullptr;
	}
	
	MapEntry<K,V>& current() const override {
		if (isEnd()) 
			throw IteratorIsEndException{__func__, __LINE__};
		if (m_map->size() != m_expectSize)
			throw ConcurrentModificationException(__func__, __LINE__, L"HashMap");
		return m_cur->m_entry;
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
	
	~HmIterator() {}
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
	
	HmIterator(HmIterator& copy) = delete;
	HmIterator(HmIterator&& move) = delete;
	HmIterator& operator=(HmIterator& copy) = delete;
	HmIterator& operator=(HmIterator&& move) = delete;
};

private:
	Node** m_table;
	e_int  m_capacity;
	e_int  m_size;
	
	constexpr e_int threshold() {return (e_int) (m_capacity * 0.75);}
	
public:
	/**
	 * \brief ctor.
	 * 
	 * If initCapacity < 16, initCapacity will be set to 16.
	 *
	 * @param initCapacity init capacity, default16.
	*/
	explicit HashMap(e_int initCapacity=16) {
		if (initCapacity < 16)
			initCapacity = 16;		
		_init(initCapacity);
	}
	
	/**
		\brief dtor.
	*/
	inline ~HashMap(){
		_dispose();
	}
	
	/**
		\brief copy ctor.
		
		@param copy
	*/
	inline HashMap(const HashMap& copy) {
		_init();
		_copy(copy);
	}
	
	/**
		\brief move ctor.
		
		@param move
	*/
	HashMap(HashMap&& move) {
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
		\brief copy assignment.
		
		@param copy
	*/
	HashMap& operator=(const HashMap& copy) {
		_dispose();
		_init();
		_copy(copy);		
		return *this;
	}
	
	/**
		\brief move assignment.
		
		@param move
	*/
	HashMap& operator=(HashMap&& move) {
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
	 * \brief Check equality.
	 * 
	 * The method compare receiver and other, if receiver's elements equals 
	 * other's, one by one, return true, otherwise retunr false.
	 * <br/>
	 * K types need implement equals() and hashCode().
	 * 
	 * @param other .
	 * @return  compared result.
	 * */
	inline e_bool equals(const HashMap<K,V>& other) const {
		return Container::equals<HashMap<K,V>, HashMap<K,V>, MapEntry<K,V>>(*this, other);
	}

	/**
	 * \brief Check order.
	 * 
	 * The method compare receiver and other, if receiver's
	 * elements is bigger than other's, one by one, return 1, 
	 * equals return 0, otherwise return -1.
	 * <br/>
	 * K types need implement compare().
	 * 
	 * @param other .
	 * @return compared result.
	 * */
	inline e_int compare(const HashMap<K,V>& other) const {
		return Container::compare<HashMap<K,V>, HashMap<K, V>, MapEntry<K,V>>(*this, other);
	}

	/**
	 * \brief get hash code.
	 * 
	 * Get receiver`s hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	inline e_int hashCode() const {return Long::hashCode(reinterpret_cast<e_long>(this));}

	/**
	 * \brief To string represent．
	 * 
	 * Element types need implement toString().
	 * 
	 * @return receiver`s string represent.
	*/
	inline String toString() const {
		return System::move(Container::toString<HashMap<K,V>, MapEntry<K,V>>(*this));
	}
	
	/**
		\brief Add key-value pair.

		@param key .
		@param value .
		@return key in receiver after putting.
		@throw if key's hashCode() is negative, throws IllegalHashCodeException.
	*/
	K& put(const K& key, const V& value) {
		if (m_size > threshold()) {
			_rehash(m_capacity*2);
		}
		
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		
		if (m_table[slot] == nullptr) {
			m_table[slot] = new Node(System::move(MapEntry<K, V>{key, value}), hash);
			m_size++;
			return m_table[slot]->m_entry.m_key;
		} else {
			Node* cur = m_table[slot];
			Node* prev = cur;
			while (cur) {
				prev = cur;
				cur = cur->m_next;
			}
			prev->m_next = new Node(System::move(MapEntry<K, V>{key, value}), hash);
			m_size++;
			return prev->m_next->m_entry.m_key;
		}
	}
	
	/**
		\brief Add key-value pair.

		@param key .
		@param value .
		@return key in receiver after putting.
		@throw if key's hashCode() is negative, throws IllegalHashCodeException.
	*/
	K& put(const K& key, V&& value) {
		if (m_size > threshold()) {
			_rehash(m_capacity*2);
		}
		
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		
		if (m_table[slot] == nullptr) {
			m_table[slot] = new Node(System::move(MapEntry<K, V>{key, System::move(value)}), hash);
			m_size++;
			return m_table[slot]->m_entry.m_key;
		} else {
			Node* cur = m_table[slot];
			Node* prev = cur;
			while (cur) {
				prev = cur;
				cur = cur->m_next;
			}
			prev->m_next = new Node(System::move(MapEntry<K, V>{key, System::move(value)}), hash);
			m_size++;
			return prev->m_next->m_entry.m_key;
		}
	}
	
	/**
		\brief Add key-value pair.

		@param key .
		@param value .
		@return key in receiver after putting.
		@throw if key's hashCode() is negative, throws IllegalHashCodeException.
	*/
	K& put(K&& key, const V& value) {
		if (m_size > threshold()) {
			_rehash(m_capacity*2);
		}
		
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		
		if (m_table[slot] == nullptr) {
			m_table[slot] = new Node(System::move(MapEntry<K, V>{System::move(key), value}), hash);
			m_size++;
			return m_table[slot]->m_entry.m_key;
		} else {
			Node* cur = m_table[slot];
			Node* prev = cur;
			while (cur) {
				prev = cur;
				cur = cur->m_next;
			}
			prev->m_next = new Node(System::move(MapEntry<K, V>{System::move(key), value}), hash);
			m_size++;
			return prev->m_next->m_entry.m_key;
		}
	}
	
	/**
		\brief Add key-value pair.

		@param key .
		@param value .
		@return key in receiver after putting.
		@throw if key's hashCode() is negative, throws IllegalHashCodeException.
	*/
	K& put(K&& key, V&& value) {
		if (m_size > threshold()) {
			_rehash(m_capacity*2);
		}
		
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		
		if (m_table[slot] == nullptr) {
			m_table[slot] = new Node(System::move(MapEntry<K, V>{System::move(key), System::move(value)}), hash);
			m_size++;
			return m_table[slot]->m_entry.m_key;
		} else {
			Node* cur = m_table[slot];
			Node* prev = cur;
			while (cur) {
				prev = cur;
				cur = cur->m_next;
			}
			prev->m_next = new Node(System::move(MapEntry<K, V>{System::move(key), System::move(value)}), hash);
			m_size++;
			return prev->m_next->m_entry.m_key;
		}
	}
	
	/**
	 * \brief Delete key-value pair.
	 * 
	 * @param key deleted key.
	 * @return deleted value.
	 * @throw if key's hashCode() is negative, throws IllegalHashCodeException.
	 * @throw if receiver not contain key, throws HasNoSuchElementException.
	*/
	V remove(const K& key) {
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		Node* cur = m_table[slot];
		Node* prev = cur;
		while (cur) {
			if (cur->m_entry.m_key.equals(key)) {
				break;
			}
			prev = cur;
			cur = cur->m_next;
		}
		
		if (cur == nullptr) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"HashMap"}};
		}
		
		if (cur == m_table[slot]) {
			m_table[slot] = cur->m_next;
		} else {
			prev->m_next = cur->m_next;
		}
		
		MapEntry<K,V> _moveEntry = System::move(cur->m_entry);
		delete cur;
		m_size--;
		return System::move(_moveEntry.m_value);
	}
	
	/**
	 * \brief Get value.
	 * 
	 * @param key key of key-value pair.
	 * @return value of key-value pair.
	 * @throw if key's hashCode() is negative, throws IllegalHashCodeException.
	 * @throw if receiver not contain key, throws HasNoSuchElementException.
	*/
	V& get(const K& key) const {
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		Node* cur = m_table[slot];
		while (cur) {
			if (cur->m_entry.m_key.equals(key)) {
				break;
			}
			cur = cur->m_next;
		}
		if (cur == nullptr) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"HashMap"}};
		}
		
		return cur->m_entry.m_value;
	}
	
	/**
	 * \brief Get value.
	 * 
	 * @param key key of key-value pair.
	 * @return value of key-value pair.
	 * @throw if key's hashCode() is negative, throws IllegalHashCodeException.
	 * @throw if receiver not contain key, throws HasNoSuchElementException.
	*/
	V& operator[](const K& key) const {
		return get(key);
	}
	
	/**
		\brief Check whether contains key.

		@param key .
		@return if receiver contains key, return true; otherwise return false.
		@throw if key.hashCode() is negative, throws IllegalHashCodeException.
	*/
	e_bool contains(const K& key) const {
		e_int hash = key.hashCode();
		if (hash < 0) {
			throw IllegalHashCodeException{__func__, __LINE__, hash};
		}
		e_int slot = hash % m_capacity;
		Node* cur  = m_table[slot];
		while (cur) {
			if (cur->m_entry.m_key.equals(key)) {
				return true;
			}
			cur = cur->m_next;
		}
		return false;
	}
	
	/**
	 * \brief Trim capacity to fix receiver's length.
	 * 
	 * @return receiver after trimming.
	*/
	HashMap& trimCapacity() {
		if (m_capacity == 16)
			return *this;
		if (m_size != m_capacity) {
			_rehash((m_size<16)?16:m_size);
		}
		return *this;
	}
	
	/**
	 * \brief Get size.
	 * 
	 * @return receiver's size.
	*/
	inline e_int size() const {
		return m_size;
	}
	
	/**
	 * \brief Check whether is empty.
	 * 
	 * @return receiver's size == 0.
	*/
	inline e_bool empty() const {
		return m_size == 0;
	}
	
	/**
	 * \brief Get capacity used.
	 * 
	 * @return capacity receiver used, by element.
	*/
	inline e_int capacity() const {
		return m_capacity;
	}
	
	/**
	 * \brief Clean buffer.
	 * 
	 * Clean buffer and reinitialize.
	*/
	inline e_void clean() {
		_dispose();
		_init();
	}
	
	/**
	 * \brief Get %iterator.
	 * 
	 * <b>The %iterator's status will change after receiver changed after this method called.</b>
	 * 
	 * @return receiver's %iterator.
	 * @sa Iterator
	*/
	inline Iterator<MapEntry<K,V>>* iterator() const {
		return new HmIterator{this, m_table, m_capacity, m_size};
	}
	
	/**
	 * \brief Get key set.
	 * 
	 * @return receiver's key set.
	 * */
	 HashSet<K> keySet() const {
	 	HashSet<K> set;
		AutoPointer<Iterator<MapEntry<K, V>>> iter{this->iterator()};
		while (!iter->isEnd()) {
			set.add(iter->current().m_key);
			iter->advance();
		}
		return System::move(set);
	 }
	
private:
	e_void _init(e_int initCapacity=16) {
		m_capacity = initCapacity;
		m_table = new Node*[m_capacity];
		m_size  = 0;
		for (e_int i=0; i<m_capacity; i++) {
			m_table[i] = nullptr;
		}
	}
	
	e_void _copy(const HashMap& copy) {
		AutoPointer<Iterator<MapEntry<K,V>>> iter{copy.iterator()};
		while (!(*iter).isEnd()) {
			this->put((*iter).current().m_key, (*iter).current().m_value);
			(*iter).advance();
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
						newTable[newSlot] = new Node((MapEntry<K,V>&&)cur->m_entry, cur->m_hashCode);
					} else {
						Node* newCur = newTable[newSlot];
						Node* prev = newCur;
						while (newCur) {
							prev = newCur;
							newCur = newCur->m_next;
						}
						prev->m_next = new Node((MapEntry<K,V>&&)cur->m_entry, cur->m_hashCode);
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
}; // HashMap
} // Easy

#endif // _EASY_HASHMAP_H_

