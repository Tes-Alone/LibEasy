/*
 * File Name:	PtrVector.hpp
 * Date/Time:
 * 	Start: 	2019-12-04 20:00 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_PTRVECTOR_H_
#define _EASY_PTRVECTOR_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"

#include "Console.hpp"

namespace Easy {

/**
 * \class PtrVector
 * 
 * \brief 动态指针数组实现.
 * 
 * 本类用于存储指针, 使用者须注意指针悬挂和 Double free 的问题.
*/

template<class E>
class PtrVector {
private:
	E**   m_buffer;
	e_int m_length;
	e_int m_capacity;
public:
	/**
	 * \brief 构造函数.
	 * 
	 * 如果 initCapa<16, initCapa 会被设置为 16.
	 * @param initCapa 初始化容量, 默认为 16.
	*/
	PtrVector(e_int initCapa=16) {
		if (initCapa < 16)
			initCapa = 16;
		
		m_capacity = initCapa;
		m_length   = 0;
		m_buffer   = new E*[m_capacity];
	}
	
	/**
	 * \brief 析构函数.
	 * 
	 * 函数不会 delete 当前所存储的指针引用的对象, 使用者须手动释放指针.
	*/
	~PtrVector() {
		for (e_int i=0; i<m_length; i++) {
			delete m_buffer[i];
		}
		delete [] m_buffer;
	}

	/**
	 * \brief copy 构造函数.
	 * 
	 * 刻意删除.
	*/
	PtrVector(const PtrVector& copy) =delete;
	
	/**
	 * \brief move 构造函数.
	 * 
	 * @param move
	*/
	PtrVector(PtrVector&& move) {
		m_capacity = move.m_capacity;
		m_length   = move.m_length;
		m_buffer   = move.m_buffer;
		move.m_capacity = 16;
		move.m_length   = 0;
		move.m_buffer   = new E*[16];
	}

	/**
	 * \brief copy 赋值.
	 * 
	 * 刻意删除.
	*/
	PtrVector& operator=(const PtrVector& copy) =delete;

	/**
	 * \brief move 赋值.
	 * 
	 * @param move
	*/
	PtrVector& operator=(PtrVector&& move) {
		delete [] m_buffer;
		m_capacity = move.m_capacity;
		m_length   = move.m_length;
		m_buffer   = move.m_buffer;
		move.m_capacity = 16;
		move.m_length   = 0;
		move.m_buffer   = new E*[16];
		return *this;
	}

	/**
	 * \brief 在结尾添加元素.
	 * 
	 * 函数仅仅复制指针.
	 * 
	 * @param item 要添加的元素.
	 * @return item.
	 * @throw 如果 item 为空指针, 抛出 NullPointerException.
	*/
	E* add(E* item) {
		if (item == nullptr) 
			throw NullPointerException{__func__, __LINE__};
		if (m_length == m_capacity) {
			_updateCapacity(m_capacity*2);
		}
		m_buffer[m_length++] = item;
		return item;
	}

	/**
	 * \brief 在指定位置插入元素.
	 * 
	 * @param index 指定的位置.
	 * @param item 元素.
	 * @return item.
	 * @throw 如果 item 为空指针, 抛出 NullPointerException.
	 * @throw 如果 index<0 或 index>PtrVector::size(); 抛出 IndexOutOfBoundsException.
	*/
	E* insert(e_int index, E* item) {
		if (index<0 || index>m_length)
			throw IndexOutOfBoundsException{__func__, __LINE__, L"PtrVector", m_length, index};
		if (item == nullptr) 
			throw NullPointerException{__func__, __LINE__};

		if (m_length == m_capacity) {
			E** newBuffer = _createNewBuffer(m_capacity*2);
			memmove_s(newBuffer, index*sizeof(E*), m_buffer, index*sizeof(E*));
			newBuffer[index] = item;
			memmove_s(&newBuffer[index+1], (m_length-index)*sizeof(E*), &m_buffer[index], (m_length-index)*sizeof(E*));
			delete [] m_buffer;
			m_buffer = newBuffer;			
		} else { 
			if (index == m_length) {
				m_buffer[m_length] = item;
			} else {
				memmove_s(&m_buffer[index+1], (m_length-index)*sizeof(E*), &m_buffer[index], (m_length-index)*sizeof(E*));
				m_buffer[index] = item;	
			}
		}
		m_length++;
		return item;
	}

	/**
	 * \brief 获取指定位置的元素.
	 * 
	 * @param index 指定位置.
	 * @return 接受者 index 位置的元素.
	 * @throw 如果 index<0 || index>=Vector::size(), 抛出 IndexOutOfBoundsExeption.
	*/
	E* get(e_int index) {
		if (index<0 || index>=m_length) 
			throw IndexOutOfBoundsException{__func__, __LINE__, L"PtrVector", m_length, index};
		return m_buffer[index];
	}
	
	/**
	 * \brief 获取指定位置的元素.
	 * 
	 * @param index 指定位置.
	 * @return 接受者 index 位置的元素.
	 * @throw 如果 index<0 || index>=Vector::size(), 抛出 IndexOutOfBoundsExeption.
	*/
	E* operator[] (e_int index) {
		return get(index);
	}

	/**
	 * \brief 删除指定位置的元素.
	 * 
	 * @param index 指定的位置.
	 * @return index 位置的元素.
	 * @throw 如果 index<0 或者 index>=PtrVector::size(), 抛出 IndexOutOfBoundsException.
	*/
	E* remove(e_int index) {
		if (index<0 || index>=m_length)
			throw IndexOutOfBoundsException{__func__, __LINE__, L"PtrVector", m_length, index};
		
		if (index == m_length-1) {
			return m_buffer[--m_length];
		}

		E* item = m_buffer[index];
		memmove_s(&m_buffer[index], (m_length-index)*sizeof(E*), &m_buffer[index+1], (m_length-index)*sizeof(E*));
		m_length--;
		return item;
	}
	
	/**
		\brief 删除元素.
		
		@param item 欲删除的元素.
		@return 存在于接受者中的 item.
		@throw 如果 item 为空指针, 抛出 Error::NullPointerException.
		@throw 如果接受者不包含 item, 抛出 Error::HasNoSuchElementException.
	*/
	E* remove(E* item) {
		if (item == nullptr) 
			throw NullPointerException{__func__, __LINE__};
		
		e_int index = indexOf(item);
		if (index == -1) 
			throw HasNoSuchElementException{__func__, __LINE__, L"PtrVector"};
		
		return remove(index);
	}
	
	/**
	 * \brief 获取指定元素位置.
	 * 
	 * @param item 用于查找的对象.
	 * @return 接受者中与 item 地址相等的元素位置, 如果没有, 返回 -1.
	 * @throw 如果 item 为空指针, 抛出 NullPointerException.
	*/
	e_int indexOf(E* item) {
		if (item == nullptr) 
			return -1;

		for (e_int i=0; i<m_length; i++) {
			if (item == m_buffer[i])
				return i;
		}
		return -1;
	}
	
	/**
	 * \brief 获取元素个数.
	 * 
	 * @return 接受者包含的元素个数.
	*/
	inline e_int size() const {
		return m_length;
	}

	/**
	 * \brief 查看当前使用内存, 以指针大小计.
	 * 
	 * @return 接受者当前使用内存大小.
	*/
	inline e_int capacity() const {
		return m_capacity;
	}
	
	/**
	 * \brief 缩减内存到正好存储元素.
	 * 
	 * @return 缩减内存后的接受者.
	*/
	PtrVector& trimToLength() {
		if (m_capacity == 16) {
			return *this;
		}
		if (m_capacity != m_length) {
			_updateCapacity(m_length);
		}
		return *this;
	}

	/**
	 * \brief 清空容器.
	 * 
	 * 函数会 delete 当前内存中的指针所引用的对象.
	 * 
	 * 清空内部缓存区并重新初始化.
	*/
	e_void clean() {
		for (e_int i=0; i<m_length; i++) {
			delete m_buffer[i];
		}
		delete [] m_buffer;
		m_capacity = 16;
		m_length   = 0;
		m_buffer   = new E*[m_capacity];
	}

private:
	E** _createNewBuffer(e_int newCapacity) {
		m_capacity = newCapacity;
		E** newBuffer = new E*[m_capacity];
		return newBuffer;
	}

	e_void _updateCapacity(e_int newCapacity) {
		E** newBuffer = _createNewBuffer(newCapacity);
		memmove_s(newBuffer, m_length*sizeof(E*), m_buffer, m_length*sizeof(E*));
		delete [] m_buffer;
		m_buffer = newBuffer;
	}
}; // PtrVector
} // Easy

#endif // _EASY_PTRVECTOR_H_

