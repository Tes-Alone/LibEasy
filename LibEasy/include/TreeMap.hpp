/*
 * File Name:	TreeMap.hpp
 * Date/Time:
 * 	Start: 	2019-12-09 18:35 week:1
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_TREEMAP_H_
#define _EASY_TREEMAP_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "MapEntry.hpp"
#include "AutoPointer.hpp"
#include "Iterator.hpp"
#include "Container.hpp"
#include "TreeSet.hpp"

#include "Console.hpp"

namespace Easy {

/**
	\class TreeMap
	
	\brief 红黑树 map 实现.
*/
template<class K, class V>
class TreeMap {

class Node {
public:
	MapEntry<K, V> m_entry;
	Node*  m_left;
	Node*  m_right;
	Node*  m_parent;
	e_bool m_isRed;
	
	Node(const MapEntry<K, V>& entry, Node* parent, e_bool isRed) {
		m_entry  = entry;
		m_left   = nullptr;
		m_right  = nullptr;
		m_isRed  = isRed;
		m_parent = parent;
	}
	
	Node(MapEntry<K, V>&& entry, Node* parent, e_bool isRed) {
		m_entry  = System::move(entry);
		m_left   = nullptr;
		m_right  = nullptr;
		m_isRed  = isRed;
		m_parent = parent;
	}
	
	e_bool isLeft() {
		return m_parent==nullptr ? true : (m_parent->m_left==this);
	}
}; // Node

class TmIterator : public Iterator<MapEntry<K, V>>
{
private:
	const TreeMap* m_map;
	e_int m_expectSize;
public:
	TmIterator(const TreeMap* map, Node* min, e_int length) {
		m_map = map;
		m_cur = min;
		m_expectSize = length;
	}
	
	~TmIterator() {}
	
	e_bool isEnd() const override{
		return m_cur == nullptr; 
	}

	MapEntry<K, V>& current() const override {
		if (isEnd()) 
			throw IteratorIsEndException{__func__, __LINE__};
		if (m_map->size() != m_expectSize)
			throw ConcurrentModificationException(__func__, __LINE__, L"TreeMap");
		return m_cur->m_entry;
	}
	
	e_bool advance() override {
		if (m_cur == nullptr) return false;
		
		if (m_cur->m_right) {
			m_cur = m_cur->m_right;
			
			while (m_cur->m_left) {
				m_cur = m_cur->m_left;
			}
		} else {			
			while (m_cur && !m_cur->isLeft()) {
				m_cur = m_cur->m_parent;
			}
			
			if (m_cur == nullptr) 
				return true;
			m_cur = m_cur->m_parent;
		}
		return true;
	}
private:	
	Node* m_cur;
	
	TmIterator(TmIterator& copy) =delete;
	TmIterator(TmIterator&& move) =delete;
	TmIterator& operator=(TmIterator& copy) =delete;
	TmIterator& operator=(TmIterator&& move) =delete;
}; // TmIterator

private:
	Node* m_root;
	e_int m_size;

public:
	/**
		\brief 构造函数.
	*/
	inline TreeMap() {
		_init();
	}
	
	/**
	 * \brief 析构函数.
	*/
	inline ~TreeMap() {
		_dispose(m_root);
	}

	/**
	 * \brief copy 构造函数.
	 * 
	 * @param copy
	*/
	inline TreeMap(const TreeMap& copy) {
		_init();
		_copy(copy);
	}

	/**
	 * \brief move 构造函数.
	 * 
	 * @param move
	*/
	TreeMap(TreeMap&& move) {
		m_root = move.m_root;
		m_size = move.m_size;
		move.m_root = nullptr;
		move.m_size = 0;
	}

	/**
	 * \brief copy 赋值.
	 * 
	 * @param copy
	*/
	TreeMap& operator=(const TreeMap& copy) {
		_dispose(m_root);
		_init();
		_copy(copy);
		return *this;
	}

	/**
	 * \brief move 赋值.
	 * 
	 * @param move
	*/
	TreeMap& operator=(TreeMap&& move) {
		_dispose(m_root);
		m_root = move.m_root;
		m_size = move.m_size;
		move.m_root = nullptr;
		move.m_size = 0;
		return *this;
	}
	
	/**
		\brief 等于函数.

		集合元素需实现 equals() 方法.

		@param other 一个集合.
		@return 如果接受者和 other 的元素对应相等, 返回真; 否则, 返回假.
	*/
	inline e_bool equals(const TreeMap<K,V>& other) const {
		return Container::equals<TreeMap<K,V>, TreeMap<K,V>, MapEntry<K,V>>(*this, other);
	}

	/**
		\brief 比较函数.

		集合元素需实现 compare() 方法.

		@param other 集合对象.
		@return 如果接受者的元素按顺序大于 other 的元素, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	inline e_int compare(const TreeMap<K,V>& other) const {
		return Container::compare<TreeMap<K,V>, TreeMap<K, V>, MapEntry<K,V>>(*this, other);
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
		return System::move(Container::toString<TreeMap<K,V>, MapEntry<K,V>>(*this));
	}
	
	/**
		\brief 添加键与值.
		
		如果接受者中包含与 key 等价的键, 函数不会再次添加.

		@param key 要添加的键.
		@param value 要添加的值.
		@return 被添加的键.
	*/
	K& put(K&& key, const V& value)
	{
		if (m_root == nullptr) {
			m_root = new Node(System::move(MapEntry<K, V>{System::move(key), value}), nullptr, false);
			m_size++;
			return m_root->m_entry.m_key;
		}
		
		Node* cur  = m_root;
		Node* prev = cur;
		
		e_int state = 0;
		while (cur) {
			state = cur->m_entry.m_key.compare(key);
			if (state == 0) {
				return cur->m_entry.m_key;
			}
			if (state > 0) {
				prev = cur;
				cur = cur->m_left;
			} else {
				prev = cur;
				cur = cur->m_right;
			}
		}
		
		cur = new Node(System::move(MapEntry<K, V>{System::move(key), value}), prev, true);
		if (state > 0) {
			prev->m_left = cur;
		} else {
			prev->m_right = cur;
		}
		
		_fixAfterInsert(cur);
		m_size++;
		return cur->m_entry.m_key;
	}

	/**
		\brief 添加键与值.
		
		如果接受者中包含与 key 等价的键, 函数不会再次添加.

		@param key 要添加的键.
		@param value 要添加的值.
		@return 被添加的键.
	*/
	e_bool put(const K& key, V&& value)
	{
		if (m_root == nullptr) {
			m_root = new Node(System::move(MapEntry<K, V>{key, System::move(value)}), nullptr, false);
			m_size++;
			return m_root->m_entry.m_key;
		}
		
		Node* cur  = m_root;
		Node* prev = cur;
		
		e_int state = 0;
		while (cur) {
			state = cur->m_entry.m_key.compare(key);
			if (state == 0) {
				return cur->m_entry.m_key;
			}
			if (state > 0) {
				prev = cur;
				cur = cur->m_left;
			} else {
				prev = cur;
				cur = cur->m_right;
			}
		}
		
		cur = new Node(System::move(MapEntry<K, V>{key, System::move(value)}), prev, true);
		if (state > 0) {
			prev->m_left = cur;
		} else {
			prev->m_right = cur;
		}
		
		_fixAfterInsert(cur);
		m_size++;
		return cur->m_entry.m_key;
	}

	/**
		\brief 添加键与值.
		
		如果接受者中包含与 key 等价的键, 函数不会再次添加.

		@param key 要添加的键.
		@param value 要添加的值.
		@return 被添加的键.
	*/
	K& put(const K& key, const V& value)
	{
		if (m_root == nullptr) {
			m_root = new Node(System::move(MapEntry<K, V>{key, value}), nullptr, false);
			m_size++;
			return m_root->m_entry.m_key;
		}
		
		Node* cur  = m_root;
		Node* prev = cur;
		
		e_int state = 0;
		while (cur) {
			state = cur->m_entry.m_key.compare(key);
			if (state == 0) {
				return cur->m_entry.m_key;
			}
			if (state > 0) {
				prev = cur;
				cur = cur->m_left;
			} else {
				prev = cur;
				cur = cur->m_right;
			}
		}
		
		cur = new Node(System::move(MapEntry<K, V>{key, value}), prev, true);
		if (state > 0) {
			prev->m_left = cur;
		} else {
			prev->m_right = cur;
		}
		
		_fixAfterInsert(cur);
		m_size++;
		return cur->m_entry.m_key;
	}

	/**
		\brief 添加键与值.
		
		如果接受者中包含与 key 等价的键, 函数不会再次添加.

		@param key 要添加的键.
		@param value 要添加的值.
		@return 被添加的键.
	*/
	K& put(K&& key, V&& value)
	{
		if (m_root == nullptr) {
			m_root = new Node(System::move(MapEntry<K, V>{System::move(key), System::move(value)}), nullptr, false);
			m_size++;
			return m_root->m_entry.m_key;
		}
		
		Node* cur  = m_root;
		Node* prev = cur;
		
		e_int state = 0;
		while (cur) {
			state = cur->m_entry.m_key.compare(key);
			
			if (state == 0) {
				return cur->m_entry.m_key;
			}
			if (state > 0) {
				prev = cur;
				cur = cur->m_left;
			} else {
				prev = cur;
				cur = cur->m_right;
			}
		}
		
		cur = new Node(System::move(MapEntry<K, V>{System::move(key), System::move(value)}), prev, true);
		if (state > 0) {
			prev->m_left = cur;
		} else {
			prev->m_right = cur;
		}
		
		_fixAfterInsert(cur);
		m_size++;
		return cur->m_entry.m_key;
	}

	/**
	 * \brief 根据键获取值.
	 * 
	 * @param key 键
	 * @return key 对应的值.
	 * @throw 如果接受者不包含 key, 抛出 HasNoSuchElementException.
	*/
	V& get(const K& key) const {
		Node* node = _findHelp(m_root, key);
		if (node == nullptr)
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"TreeMap"}};
		return node->m_entry.m_value;
	}
	
	/**
	 * \brief 根据键获取值.
	 * 
	 * @param key 键
	 * @return key 对应的值.
	 * @throw 如果接受者不包含 key, 抛出 HasNoSuchElementException.
	*/
	V& operator[](const K& key) const {
		return get(key);
	}

	/**
	 * \brief 删除键与值.
	 * 
	 * @param key 键, 用于搜索值.
	 * @return 被删除的键/值的值.
	 * @throw 如果接受者不包含 key, 抛出 HasNoSuchElementException.
	*/
	V remove(const K& key) {
		Node* _move = _findHelp(m_root, key);
		if (_move == nullptr) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"TreeMap"}};
		}
		MapEntry<K, V> _moveEntry = System::move(_move->m_entry);
	label:
		if (_move->m_left==nullptr && _move->m_right==nullptr) {
			if (_move->isLeft()) {
				if (_move != m_root) {
					_move->m_parent->m_left = nullptr;
					if (!_move->m_isRed) {
						_fixAfterRemoveBlackLeaf(_move);
					}
				} else {
					m_root = nullptr;
				}
			} else {
				if (_move != m_root) {
					_move->m_parent->m_right = nullptr;
					if (!_move->m_isRed) {
						_fixAfterRemoveBlackLeaf(_move);
					}
				} else {
					m_root = nullptr;
				}
			}			
		} else if (_move->m_left!=nullptr && _move->m_right==nullptr) {
			if (_move->isLeft()) {
				if (_move != m_root) {
					_move->m_parent->m_left = _move->m_left;
					_move->m_left->m_parent = _move->m_parent;
				} else {
					m_root = _move->m_left;
					m_root->m_parent = nullptr;
				}
			} else {
				if (_move != m_root) {
					_move->m_parent->m_right = _move->m_left;
					_move->m_left->m_parent = _move->m_parent;
				} else {
					m_root = _move->m_left;
					m_root->m_parent = nullptr;
				}
			}
			_move->m_left->m_isRed = false;
		} else if (_move->m_left==nullptr && _move->m_right!=nullptr) {
			if (_move->isLeft()) {
				if (_move != m_root) {
					_move->m_parent->m_left = _move->m_right;
					_move->m_right->m_parent = _move->m_parent;
				} else {
					m_root = _move->m_right;
					m_root->m_parent = nullptr;
				}
			} else {
				if (_move != m_root) {
					_move->m_parent->m_right = _move->m_right;
					_move->m_right->m_parent = _move->m_parent;
				} else {
					m_root = _move->m_right;
					m_root->m_parent = nullptr;
				}
			}
			_move->m_right->m_isRed = false;
		} else { // _move->m_left != null && _move->m_right != null 
			Node* _rightMin = _getMin(_move->m_right);
			_move->m_entry = System::move(_rightMin->m_entry);
			_move = _rightMin;
			goto label;
		}
		
		m_size--;
		delete _move;
		return System::move(_moveEntry.m_value);
	}
	
	/**
		\brief 判断接受者是否包含指定键.

		@param key 欲查找的键.
		@return 如果接受者包含 key, 返回真; 否则, 返回假.
	*/
	inline e_bool contains(const K& key) const {
		return _findHelp(m_root, key) != nullptr;
	}
	
	/**
	 * \brief 获取最大键的值.
	 * 
	 * @return 接受者中的最大键的值.
	 * @throw 如果接受者空, 抛出 EmptyContainerException.
	*/
	V& max() const {
		Node* _max =  _getMax(m_root);
		if (_max == nullptr) {
			throw  EmptyContainerException{__func__, __LINE__, String{L"TreeMap"}};
		}
		return _max->m_entry.m_value;
	}

	/**
	 * \brief 获取最小键的值.
	 * 
	 * @return 接受者中的最小键的值.
	 * @throw 如果接受者空, 抛出 EmptyContainerException.
	*/
	V& min() const {
		Node* _min =  _getMin(m_root);
		if (_min == nullptr) {
			throw  EmptyContainerException{__func__, __LINE__, String{L"TreeMap"}};
		}
		return _min->m_entry.m_value;
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
	inline Iterator<MapEntry<K, V>>* iterator() const {
		return new TmIterator{this, _getMin(m_root), m_size};
	}
	
	/**
	 * \brief 获取键集合.
	 * 
	 * @return 接受者的所有键.
	 * */
	 TreeSet<K> keySet() const {
	 	TreeSet<K> set;
		AutoPointer<Iterator<MapEntry<K, V>>> iter{this->iterator()};
		while (!iter->isEnd()) {
			set.add(iter->current().m_key);
			iter->advance();
		}
		return System::move(set);
	 }

private:
	e_void _init() {
		m_root = nullptr;
		m_size = 0;
	}
	
	e_void _dispose(Node* root) {
		if (root == nullptr) return;
		_dispose(root->m_left);
		_dispose(root->m_right);
		delete root;
	}
	
	e_void _copy(const TreeMap& copy) {
		AutoPointer<Iterator<MapEntry<K,V>>> iter{copy.iterator()};
		while (!(*iter).isEnd()) {
			this->put((*iter).current().m_key, (*iter).current().m_value);
			(*iter).advance();
		}
	}

	Node* _findHelp(Node* root, const K& key) const {
		if (root == nullptr) return nullptr;
		e_int state = root->m_entry.m_key.compare(key);
		if (state == 0) return root;
		if (state < 0) {
			return _findHelp(root->m_right, key);
		} else {
			return _findHelp(root->m_left, key);
		}
	}

	Node* _findHelp(Node* root, const K&& key) const {
		if (root == nullptr) return nullptr;
		e_int state = root->m_entry.m_key.compare(key);
		if (state == 0) return root;
		if (state < 0) {
			return _findHelp(root->m_right, key);
		} else {
			return _findHelp(root->m_left, key);
		}
	}

	Node* _getMin(Node* root) const {
		Node* cur = root;
		Node* prev = cur;
		while (cur) {
			prev = cur;
			cur = cur->m_left;
		}
		
		return prev;
	}

	Node* _getMax(Node* root) const {
		Node* cur = root;
		Node* prev = cur;
		while (cur) {
			prev = cur;
			cur = cur->m_right;
		}
		
		return prev;
	}

	e_void _fixAfterRemoveBlackLeaf(Node* node) {
		while (node!=m_root && !node->m_isRed) {
			Node* parent = node->m_parent;
			Node* brother = nullptr;
			if (node->isLeft()) {
				brother = parent->m_right;
				if (brother != nullptr) {
					if (brother->m_isRed) {
						_rightParentRightChild(parent);
					} else {
						if ((brother->m_left==nullptr || !brother->m_left->m_isRed) &&
								(brother->m_right==nullptr || !brother->m_right->m_isRed)) {
							if (parent->m_isRed) {
								parent->m_isRed = false;
								brother->m_isRed = true;
								break;
							} else {
								brother->m_isRed = true;
								node = parent;
							}
						} else if (brother->m_left!=nullptr && brother->m_left->m_isRed) {
							_rightParentLeftChild(parent);
							break;
						} else {
							_rightParentRightChild(parent);
							break;
						}
					}
				} else {
					break;
				}
			} else {
				brother = parent->m_left;
				if (brother != nullptr) {
					if (brother->m_isRed) {
						_leftParentLeftChild(parent);
					} else {
						if ((brother->m_left==nullptr || !brother->m_left->m_isRed) &&
								(brother->m_right==nullptr || !brother->m_right->m_isRed)) {
							if (parent->m_isRed) {
								parent->m_isRed = false;
								brother->m_isRed = true;
								break;
							} else {
								brother->m_isRed = true;
								node = parent;
							}
						} else if (brother->m_left!=nullptr && brother->m_left->m_isRed) {
							_leftParentLeftChild(parent);
							break;
						} else {
							_leftParentRightChild(parent);
							break;
						}
					}
				} else {
					break;
				}
			}
		}
		
		m_root->m_isRed = false;
	}
	
	e_void _fixAfterInsert(Node* node) {
		while (node!=m_root && node->m_parent->m_isRed) { // so grandPanret must exist and is black.
			Node* parent = node->m_parent;
			Node* grandParent = parent->m_parent;
			Node* uncle = nullptr;
			if (parent == grandParent->m_left) { // new node`s parent is left.
				uncle = grandParent->m_right; //uncle may not exist.
				if (uncle == nullptr) { //uncle is black
					if (node == parent->m_left)
						_leftParentLeftChild(grandParent);
					else 
						_leftParentRightChild(grandParent);
					break;
				} else if (!uncle->m_isRed) {
					if (node == parent->m_left)
						_leftParentLeftChild(grandParent);
					else 
						_leftParentRightChild(grandParent);
					break;
				} else { //uncle is red
					parent->m_isRed = false;
					uncle->m_isRed = false;
					grandParent->m_isRed = true;
					node = grandParent;
				}
			} else { // right parent
				uncle = grandParent->m_left;
				if (uncle == nullptr) { //uncle is black
					if (node == parent->m_left)
						_rightParentLeftChild(grandParent);
					else
						_rightParentRightChild(grandParent);
					break;
				} else if (!uncle->m_isRed) {
					if (node == parent->m_left)
						_rightParentLeftChild(grandParent);
					else
						_rightParentRightChild(grandParent);
					break;
				} else {
					parent->m_isRed = false;
					uncle->m_isRed = false;
					grandParent->m_isRed = true;
					node = grandParent;
				}
			}
		}
		
		m_root->m_isRed = false;
	}
	
	e_void _rightParentLeftChild(Node* imbalance) {
		_leftParentLeftChild(imbalance->m_right);
		imbalance->m_right->m_isRed = true;
		imbalance->m_right->m_right->m_isRed = true;
		_rightParentRightChild(imbalance);
	}
	
	e_void _leftParentRightChild(Node* imbalance) {
		_rightParentRightChild(imbalance->m_left);
		imbalance->m_left->m_isRed = true;
		imbalance->m_left->m_left->m_isRed = true;
		_leftParentLeftChild(imbalance);
	}
	
	e_void _leftParentLeftChild(Node* imbalance) {
		Node* tmp = imbalance->m_left;
		if (imbalance == m_root) m_root = tmp;
		imbalance->m_left = tmp->m_right;
		if (imbalance->m_left != nullptr) {
			imbalance->m_left->m_parent = imbalance;
		}
		tmp->m_parent = imbalance->m_parent;
		if (tmp->m_parent != nullptr) {
			if (imbalance == tmp->m_parent->m_left) {
				tmp->m_parent->m_left = tmp;
			} else {
				tmp->m_parent->m_right = tmp;
			}
		}
		tmp->m_right = imbalance;
		imbalance->m_parent = tmp;
		
		imbalance->m_isRed = true;
		tmp->m_isRed = false;
	}
	
	e_void _rightParentRightChild(Node* imbalance) {
		Node* tmp = imbalance->m_right;
		if (imbalance == m_root) m_root = tmp;
		imbalance->m_right = tmp->m_left;
		if (imbalance->m_right != nullptr) {
			imbalance->m_right->m_parent = imbalance;
		}
		tmp->m_parent = imbalance->m_parent;
		if (tmp->m_parent != nullptr) {
			if (imbalance == tmp->m_parent->m_left) {
				tmp->m_parent->m_left = tmp;
			} else {
				tmp->m_parent->m_right = tmp;
			}
		}
		tmp->m_left = imbalance;
		imbalance->m_parent = tmp;
		
		imbalance->m_isRed = true;
		tmp->m_isRed = false;
	}
	
}; // TreeMap
} // Easy

#endif // _EASY_TREEMAP_H_

