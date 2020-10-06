/*
 * 文件名: Heap.hpp 
 *
 * 日期/时间:
 *		开始: 2020年05月23日 22:38:09 周六
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_HEAP_H_
#define _EASY_HEAP_H_

#include "Vector.hpp"
#include "Exception.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace Easy {

/**
 * \class Heap
 * 
 * \brief A max/min heap implement.
 * 
 * The heaps are not the heap memories used by a program, they are a priority queue.
 * They can do jobs of get max or min value in a list quickly.
 * */
template<class E>
class Heap {
private:
	Vector<E> m_heap;
	e_bool    m_isMax;
public:
	/**
	 * \brief ctor.
	 * 
	 * @param isMax indicate the object is a max heap or a min heap.
	 * */
	Heap(e_bool isMax=true) {
		m_isMax = isMax;
	}
	
	/**
	 * \brief ctor.
	 * 
	 * @param arr the array the heap object which build from.
	 * @param isMax indicate the object is a max heap or a min heap.
	 * */
	Heap(const Array<E>& arr, e_bool isMax=true) {
		m_isMax = isMax;
		_buildHeap(arr);
	}
	
	/**
	 * \brief add element.
	 * 
	 * Add a element to receiver.
	 * 
	 * @param item added element.
	 * @return receiver after added item.
	 * */
	Heap<E>& add(const E& item) {
		m_heap.add(item);
		// Now sift up until curr’s parent > curr
		e_int curr = m_heap.size() - 1;		
		while (curr > 0) {
			e_int parent = _parent(curr);
			e_int flag   = m_heap[curr].compare(m_heap[parent]);
			if (m_isMax) {
				if (flag > 0) {
					E temp         = System::move(m_heap[curr]);
					m_heap[curr]   = System::move(m_heap[parent]);
					m_heap[parent] = System::move(temp);
				}
			} else {
				if (flag < 0) {
					E temp         = System::move(m_heap[curr]);
					m_heap[curr]   = System::move(m_heap[parent]);
					m_heap[parent] = System::move(temp);
				}
			}
			curr = parent;
		}
		return *this;
	}
	
	/**
	 * \brief add element.
	 * 
	 * Add a element to receiver.
	 * 
	 * @param item added element.
	 * @return receiver after added item.
	 * */
	Heap<E>& add(E&& item) {
		m_heap.add(System::move(item));
		// Now sift up until curr’s parent > curr
		e_int curr = m_heap.size() - 1;		
		while (curr > 0) {
			e_int parent = _parent(curr);
			e_int flag   = m_heap[curr].compare(m_heap[parent]);
			if (m_isMax) {
				if (flag > 0) {
					E temp         = System::move(m_heap[curr]);
					m_heap[curr]   = System::move(m_heap[parent]);
					m_heap[parent] = System::move(temp);
				}
			} else {
				if (flag < 0) {
					E temp         = System::move(m_heap[curr]);
					m_heap[curr]   = System::move(m_heap[parent]);
					m_heap[parent] = System::move(temp);
				}
			}
			curr = parent;
		}
		return *this;
	}
	
	/**
	 * \brief get first element.
	 * 
	 * @return the first element.
	 * @throw if receiver is empty, throw EmptyContainerException.
	 * */
	inline E& getFirst() const {
		if (m_heap.empty()) {
			throw EmptyContainerException{__func__, __LINE__, L"Heap"};
		}
		return m_heap[0];
	}
	
	/**
	 * \brief get element.
	 * 
	 * Get element at given index.
	 * 
	 * @param index the index.
	 * @return element at index.
	 * @throw if index<0 or index>=Heap::size(), throw IndexOutOfBoundsException.
	 * */
	inline E& get(e_int index) const {
		return m_heap[index];
	}
	
	/**
	 * \brief get element.
	 * 
	 * Get element at given index.
	 * 
	 * @param index the index.
	 * @return element at index.
	 * @throw if index<0 or index>=Heap::size(), throw IndexOutOfBoundsException.
	 * */
	inline E& operator[](e_int index) const {
		return get(index);
	}
	
	/**
	 * \brief remove first element.
	 * 
	 * @return removed element.
	 * @throw if receiver is empty, throw EmptyContainerException.
	 * */
	E removeFirst() {
		if (m_heap.empty()) {
			throw EmptyContainerException{__func__, __LINE__, L"Heap"};
		}
		e_int n = m_heap.size() - 1;
		if (n == 0) {
			return System::move(m_heap.remove(0));
		}
		E temp    = System::move(m_heap[n]);
		m_heap[n] = System::move(m_heap[0]);
		m_heap[0] = System::move(temp);
		E result  = System::move(m_heap.remove(n));
		_siftdown(0);
		return System::move(result);
	}
	
	/**
	 * \brief remove element.
	 * 
	 * Remove element at given index.
	 * 
	 * @param index the index.
	 * @return removed element.
	 * @throw if receiver is empty, throw EmptyContainerException.
	 * */
	E remove(e_int index) {
		if (m_heap.empty()) {
			throw EmptyContainerException{__func__, __LINE__, L"Heap"};
		}
		e_int n = m_heap.size() - 1;
		if (n == 0) {
			return System::move(m_heap.remove(0));
		}
		E temp        = System::move(m_heap[index]);
		m_heap[index] = System::move(m_heap[n]);
		m_heap[n]     = System::move(temp);
		
		temp = System::move(m_heap.remove(n));
		
		while (index != 0) {
			e_int parent = _parent(index);
			e_int flag   = m_heap[index].compare(m_heap[parent]);
			if (m_isMax) {
				if (flag > 0) {
					E temp         = System::move(m_heap[index]);
					m_heap[index]  = System::move(m_heap[parent]);
					m_heap[parent] = System::move(temp);
				}
			} else {
				if (flag < 0) {
					E temp         = System::move(m_heap[index]);
					m_heap[index]  = System::move(m_heap[parent]);
					m_heap[parent] = System::move(temp);
				}
			}
			index = parent;
		}
		_siftdown(index); // Push down small key
		return System::move(temp);
	}
	
	/**
	 * \brief get size.
	 * 
	 * Get receiver`s size.
	 * 
	 * @return receiver`s size.
	 * */
	inline e_int size() const {
		 return m_heap.size(); 
	}
	
	/**
	 * \brief check whether empty.
	 * 
	 * @return if receiver is empty return true, otherwise return false.
	 * */
	inline e_bool empty() {
		return m_heap.empty();
	}
	
	/**
	 * \brief trim container.
	 * 
	 * Trim receiver to fit its content.
	 * 
	 * @return trimmed receiver.
	 * */
	inline Heap<E>& trimToLength() {
		m_heap.trimToLength();
		return *this;
	}
	
	/**
	 * \brief clean container.
	 * 
	 * Clean receiver.
	 * 
	 * @return cleaned receiver.
	 * */
	inline Heap<E>& clean() {
		m_heap.clean();
		return *this;
	}
	
	/**
	 * \brief get string represent.
	 * 
	 * Get string represent.
	 * 
	 * @return receiver`s string represent.
	 * */
	inline String toString() const {
		return m_heap.toString();
	}
private:
	e_void _buildHeap(const Array<E>& arr) {
		for (e_int i=0; i<arr.length(); i++) {
			m_heap.add(arr[i]);
		}
		e_int i = arr.length() / 2 - 1;
		for (; i>=0; i--) {
			_siftdown(i);
		}
	}
	
	e_bool _isLeaf(int pos) const {
		e_int n = m_heap.size();
		return (pos >= n/2) && (pos < n); 
	}
	
	e_int _leftchild(int pos) const { 
		return 2 * pos + 1; 
	}
	
	e_int _rightchild(int pos) const {
		return 2 * pos + 2; 
	}
	
	e_int _parent(int pos) const { 
		return (pos-1) / 2; 
	}
	
	// Helper function to put element in its correct place
	e_void _siftdown(e_int pos) {
		while (!_isLeaf(pos)) { // Stop if pos is a leaf
			e_int j  = _leftchild(pos); 
			e_int rc = _rightchild(pos);			
			if (rc < m_heap.size()) {
				e_int flag = m_heap[rc].compare(m_heap[j]);
				if (m_isMax) {
					if (flag > 0) {
						j = rc;
					}
				} else {
					if (flag < 0) {
						j = rc;
					}
				}
			}
			
			e_int flag = m_heap[pos].compare(m_heap[j]);
			if (m_isMax) {
				if (flag >= 0) return;
			} else {
				if (flag <= 0) return;
			}
			E temp      = System::move(m_heap[pos]);
			m_heap[pos] = System::move(m_heap[j]);
			m_heap[j]   = System::move(temp);
			pos = j; // Move down
		}
	}
}; // Heap

} //Easy


#endif // _EASY_HEAP_H_
