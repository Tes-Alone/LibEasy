/*
 * File Name:	TreeSet.hpp
 * Date/Time:
 * 	Start: 	2019-12-08 12:42 week:0
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_TREESET_H_
#define _EASY_TREESET_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "AutoPointer.hpp"
#include "Math.hpp"
#include "Container.hpp"
#include "System.hpp"

#include "Console.hpp"

namespace Easy {

/**
 * \class TreeSet
 * 
 * \brief 红黑树实现.
*/

template<class E>
class TreeSet {

class Node {
public:
	E      m_item;
	Node*  m_parent;
	Node*  m_left;
	Node*  m_right;
	e_bool m_isRed;
	
	Node(const E& item, Node* parent, e_bool isRed) {
		m_item   = item;
		m_parent = parent;
		m_left   = nullptr;
		m_right  = nullptr;
		m_isRed  = isRed;
	}
	
	Node(E&& item, Node* parent, e_bool isRed) {
		m_item   = System::move(item);
		m_parent = parent;
		m_left   = nullptr;
		m_right  = nullptr;
		m_isRed  = isRed;
	}

	e_bool isLeft() {
		return m_parent==nullptr ? true : (m_parent->m_left==this);
	}
}; //Node

class TsIterator : public Iterator<E> {
private:
	const TreeSet* m_set;
	e_int m_expectSize;
public:
	TsIterator(const TreeSet* set, Node* min, e_int length) {
		m_set = set;
		m_cur = min;
		m_expectSize = length;
	}
	
	~TsIterator() {}
	
	e_bool isEnd() const override{
		return m_cur == nullptr; 
	}

	E& current() const override {
		if (isEnd()) 
			throw IteratorIsEndException{__func__, __LINE__};
		if (m_set->size() != m_expectSize)
			throw ConcurrentModificationException(__func__, __LINE__, L"TreeSet");
		return m_cur->m_item;
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
	
	TsIterator(TsIterator& copy) =delete;
	TsIterator(TsIterator&& move) =delete;
	TsIterator& operator=(TsIterator& copy) =delete;
	TsIterator& operator=(TsIterator&& move) =delete;
};

public:
	/**
	 * \brief 构造函数.
	*/
	inline TreeSet() {
		_init();
	}

	/**
	 * \brief 析构函数.
	*/
	inline ~TreeSet() {
		_dispose(m_root);
	}
	
	/**
	 * \brief copy 构造函数.
	 * 
	 * @param copy
	*/
	inline TreeSet(const TreeSet& copy) {
		_init();
		_copy(copy);
	}

	/**
	 * \brief move 构造函数.
	 * 
	 * @param move
	*/
	TreeSet(TreeSet&& move) {
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
	TreeSet& operator=(const TreeSet& copy) {
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
	TreeSet& operator=(TreeSet&& move) {
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
	inline e_bool equals(const TreeSet<E>& other) const {
		return Container::equals<TreeSet<E>, TreeSet<E>, E>(*this, other);
	}

	/**
		\brief 比较函数.

		集合元素需实现 compare() 方法.

		@param other 集合对象.
		@return 如果接受者的元素按顺序大于 other 的元素, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	inline e_int compare(const TreeSet<E>& other) const {
		return Container::compare<TreeSet<E>, TreeSet<E>, E>(*this, other);
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
		return System::move(Container::toString<TreeSet<E>, E>(*this));
	}
	
	/**
		\brief 添加元素.
		
		如果接受者中包含等价元素, 函数不会再次添加元素.

		@param item 要添加的元素.
		@return 添加在接受者中的于 item 等价的元素
	*/
	E& add(const E& item) {
		if (m_root == nullptr) {
			m_root = new Node(item, nullptr, false);
			m_size++;
			return m_root->m_item;
		}
		
		Node* cur  = m_root;
		Node* prev = cur;
		e_int state = 0;
		while (cur) {
			state = cur->m_item.compare(item);
			if (state == 0) {
				return cur->m_item;
			}
			if (state > 0) {
				prev = cur;
				cur = cur->m_left;
			} else {
				prev = cur;
				cur = cur->m_right;
			}
		}
		
		cur = new Node(item, prev, true);
		if (state > 0) {
			prev->m_left = cur;
		} else {
			prev->m_right = cur;
		}
		
		_fixAfterInsert(cur);
		m_size++;
		return cur->m_item;
	}

	/**
		\brief 添加元素.
		
		如果接受者中包含等价元素, 函数不会再次添加元素.

		@param item 要添加的元素.
		@return 添加在接受者中的于 item 等价的元素
	*/
	E& add(E&& item)
	{
		if (m_root == nullptr) {
			m_root = new Node(System::move(item), nullptr, false);
			m_size++;
			return m_root->m_item;
		}
		
		Node* cur  = m_root;
		Node* prev = cur;
		e_int state = 0;
		while (cur) {
			state = cur->m_item.compare(item);
			if (state == 0) {
				return cur->m_item;
			}
			if (state > 0) {
				prev = cur;
				cur = cur->m_left;
			} else {
				prev = cur;
				cur = cur->m_right;
			}
		}
		
		cur = new Node(System::move(item), prev, true);
		if (state > 0) {
			prev->m_left = cur;
		} else {
			prev->m_right = cur;
		}
		
		_fixAfterInsert(cur);
		m_size++;
		return cur->m_item;
	}

	/**
	 * \brief 删除元素.
	 * 
	 * @param item 与要删除的元素等价的元素.
	 * @return 被删除的元素.
	 * @throw 如果接受者不包含元素 item, 抛出 HasNoSuchElementException.
	*/
	E remove(const E& item) {
		Node* _move = _findHelp(m_root, item);
		if (_move == nullptr) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"TreeSet"}};
		}
		E _moveItem = System::move(_move->m_item);
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
			_move->m_item = System::move(_rightMin->m_item);
			_move = _rightMin;
			goto label;
		}
		
		m_size--;
		delete _move;
		return System::move(_moveItem);
	}

	/**
	 * \brief 获取元素.
	 * 
	 * @param item 与欲获取元素等价的元素.
	 * @return 接受者中与 item 等价的元素.
	 * @throw 如果接受者空或不包含 item, 抛出 HasNoSuchElementException.
	*/
	E& get(const E& item) const {
		Node* node = _findHelp(m_root, item);
		if (node == nullptr)
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"TreeSet"}};
		return node->m_item;
	}
	
	/**
		\brief 判断接受者是否包含指定元素.

		@param item 欲查找的项.
		@return 如果接受者包含 item, 返回真; 否则, 返回假.
	*/
	inline e_bool contains(const E& item) const {
		return _findHelp(m_root, item) != nullptr;
	}

	/**
	 * \brief 获取最大值.
	 * 
	 * @return 接受者中的最大元素.
	 * @throw 如果接受者空, 抛出 EmptyContainerException.
	*/
	E& max() const {
		Node* _max =  _getMax(m_root);
		if (_max == nullptr) {
			throw  EmptyContainerException{__func__, __LINE__, String{L"TreeSet"}};
		}
		return _max->m_item;
	}

	/**
	 * \brief 获取最小值.
	 * 
	 * @return 接受者中的最小元素.
	 * @throw 如果接受者空, 抛出 EmptyContainerException.
	*/
	E& min() const {
		Node* _min =  _getMin(m_root);
		if (_min == nullptr) {
			throw  EmptyContainerException{__func__, __LINE__, String{L"TreeSet"}};
		}
		return _min->m_item;
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
	inline Iterator<E>* iterator() const {
		return new TsIterator{this, _getMin(m_root), m_size};
	}
private:
	Node* m_root;
	e_int m_size;
	
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
	
	e_void _copy(const TreeSet& copy) {
		AutoPointer<Iterator<E>> iter{copy.iterator()};
		while (!iter->isEnd()) {
			this->add(iter->current());
			iter->advance();
		}
	}
	
	Node* _findHelp(Node* root, const E& item) const {
		if (root == nullptr) return nullptr;
		e_int state = root->m_item.compare(item);
		if (state == 0) return root;
		if (state < 0) {
			return _findHelp(root->m_right, item);
		} else {
			return _findHelp(root->m_left, item);
		}
	}

	Node* _findHelp(Node* root, const E&& item) const {
		if (root == nullptr) return nullptr;
		e_int state = root->m_item.compare(item);
		if (state == 0) return root;
		if (state < 0) {
			return _findHelp(root->m_right, item);
		} else {
			return _findHelp(root->m_left, item);
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
}; // TreeSet
} // Easy

#endif // _EASY_TREESET_H_

