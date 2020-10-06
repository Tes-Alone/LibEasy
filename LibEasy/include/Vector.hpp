/*
 * File Name:	Vector.hpp
 * Date/Time:
 * 	Start: 	2019-12-04 14:18 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_VECTOR_H_
#define _EASY_VECTOR_H_

#include <cstring>
#include <cstdlib>

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "Container.hpp"
#include "System.hpp"


namespace Easy {

/**
 * \class Vector
 * 
 * \brief 动态数组实现.
 * 
 * 用于值类型.
 * 
 * @since 1.0
*/

template<class E>
class Vector {

class VcIterator : public Iterator<E> {
private:
	const Vector* m_vector;
	E*    m_buffer;
	e_int m_expectSize;
	e_int m_index;
public:
	VcIterator(const Vector* v, E* buffer, e_int length) : Iterator<E>() {
		m_vector = v;
		m_buffer = buffer;
		m_expectSize = length;
		m_index  = 0;
	}

	e_bool isEnd() const override {
		return m_index >= m_expectSize;
	}

	E& current() const override {
		if (isEnd())
			throw IteratorIsEndException{__func__, __LINE__};
		if (m_vector->size() != m_expectSize)
			throw ConcurrentModificationException(__func__, __LINE__, L"Vector");
		return m_buffer[m_index];
	}
	
	e_bool advance() override {
		if (m_index>=m_expectSize)
			return false;
		m_index++;
		return true;
	}
	
	~VcIterator() {}
private:
	VcIterator(const VcIterator& copy) = delete;
	VcIterator& operator=(const VcIterator& copy) = delete;
	VcIterator(VcIterator&& move) = delete;
	VcIterator& operator=(VcIterator&& move) = delete;
}; // VcIterator

private:
	/*
		所有的操作必须保证:
			m_buffer 最少有 16 个空间,
			m_capacity 最少为 16,
			m_length 必须 > 0, 且 < m_capacity.
	*/
	E*    m_buffer;
	e_int m_length; 
	e_int m_capacity;
public:
	/**
	 * \brief 构造函数.
	 * 
	 * 如果 initCapa 小于 16, 函数会将 initCapa 设置为 16.
	 *
	 * @param initCapa 初始化容量, 默认为 16.
	*/
	explicit Vector(e_int initCapa=16) {		
		if (initCapa < 16) {
			initCapa = 16;
		}
		m_capacity = initCapa;
		m_buffer   = new E[m_capacity];
		m_length   = 0;
	}

	/**
	 * \brief 析构函数.
	*/
	inline ~Vector() {delete [] m_buffer;}
	
	/**
	 * \brief copy 构造函数.
	 * 
	 * @param copy
	*/
	Vector(const Vector& copy) {
		m_length   = copy.m_length;
		m_capacity = (m_length<16) ? 16 : m_length;
		m_buffer   = new E[m_capacity];
		_copy(m_buffer, copy.m_buffer);
	}

	/**
	 * \brief move 构造函数.
	 * 
	 * @param move
	*/
	Vector(Vector&& move) {
		m_length   = move.m_length;
		m_capacity = move.m_capacity;
		m_buffer   = move.m_buffer;
		move.m_capacity = 16;
		move.m_buffer   = new E[m_capacity];
		move.m_length   = 0;
	}

	/**
	 * \brief copy 赋值.
	 * 
	 * @param copy
	 * @return copy 赋值后的接受者.
	*/
	Vector& operator=(const Vector& copy) {
		delete [] m_buffer;
		m_length   = copy.m_length;
		m_capacity = (m_length<16) ? 16 : m_length;
		m_buffer   = new E[m_capacity];
		_copy(m_buffer, copy.m_buffer);
		return *this;		
	}

	/**
	 * \brief move 赋值.
	 * 
	 * @param move
	 * @return move 赋值后的接受者.
	*/
	Vector& operator=(Vector&& move) {
		delete [] m_buffer;
		m_length   = move.m_length;
		m_capacity = move.m_capacity;
		m_buffer   = move.m_buffer;
		move.m_capacity = 16;
		move.m_buffer   = new E[m_capacity];
		move.m_length   = 0;
		return *this;	
	}

	/**
		\brief 等于函数.

		集合元素需实现 equals() 方法.

		@param other 一个集合.
		@return 如果接受者和 other 的元素对应相等, 返回真; 否则, 返回假.
	*/
	inline e_bool equals(const Vector<E>& other) const {
		return Container::equals<Vector<E>, Vector<E>, E>(*this, other);
	}

	/**
		\brief 比较函数.

		集合元素需实现 compare() 方法.

		@param other 集合对象.
		@return 如果接受者的元素按顺序大于 other 的元素, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	inline e_int compare(const Vector<E>& other) const {
		return Container::compare<Vector<E>, Vector<E>, E>(*this, other);
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
		return System::move(Container::toString<Vector<E>, E>(*this));
	}
	
	/**
	 * \brief 在结尾添加元素.
	 * 
	 * 使用 copy 语义.
	 * 
	 * @param item 要添加的元素.
	 * @return 添加在接受者中的于 item 等价的元素.
	*/
	E& add(const E& item) {
		if (m_length==m_capacity) {
			_updateCapacity(m_capacity*2);
		}
		
		m_buffer[m_length++] = item;
		return m_buffer[m_length-1];
	}

	/**
	 * \brief 在结尾添加元素.
	 * 
	 * 使用 move 语义.
	 * 
	 * @param item 要添加的元素.
	 * @return 添加在接受者中的于 item 等价的元素
	*/
	E& add(E&& item) {
		if (m_length==m_capacity) {
			_updateCapacity(m_capacity*2);
		}
	
		m_buffer[m_length++] = System::move(item);
		return m_buffer[m_length-1];
	}

	/**
	 * \brief 在指定位置插入元素.
	 * 
	 * 使用 copy 语义.
	 * 
	 * @param index 指定的位置.
	 * @param item 元素.
	 * @return 插入的元素.
	 * @throw 如果 index<0 或 index>Vector::size(); 抛出 IndexOutOfBoundsException.
	*/
	E& insert(const E& item, e_int index) {
		if (index<0 || index>m_length) {
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, index};
		}
		
		if (m_length == m_capacity) {
			E* newBuffer = _createNewBuffer(m_capacity*2);
			_move(newBuffer, m_buffer, index);
			newBuffer[index] = item;
			_move(&newBuffer[index+1], &m_buffer[index], m_length-index);
			delete [] m_buffer;
			m_buffer = newBuffer;			
		} else { 
			if (index == m_length) {
				m_buffer[m_length] = item;
			} else {
				_moveBack(&m_buffer[index+1], &m_buffer[index], m_length-index);
				m_buffer[index] = item;	
			}
		}
		m_length++;
		return m_buffer[index];
	}

	/**
	 * \brief 在指定位置插入元素.
	 * 
	 * 使用 move 语义.
	 * 
	 * @param index 指定的位置.
	 * @param item 元素.
	 * @return 插入的元素.
	 * @throw 如果 index<0 或 index>Vector::size(); 抛出 IndexOutOfBoundsException.
	*/
	E& insert(E&& item, e_int index) {
		if (index<0 || index>m_length) {
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, index};
		}
		
		if (m_length == m_capacity) {
			E* newBuffer = _createNewBuffer(m_capacity*2);
			_move(newBuffer, m_buffer, index);
			newBuffer[index] = System::move(item);
			_move(&newBuffer[index+1], &m_buffer[index], m_length-index);
			delete [] m_buffer;
			m_buffer = newBuffer;			
		} else { 
			if (index == m_length) {
				m_buffer[m_length] = System::move(item);
			} else {
				_moveBack(&m_buffer[index+1], &m_buffer[index], m_length-index);
				m_buffer[index] = System::move(item);	
			}
		}
		m_length++;
		return m_buffer[index];
	}

	/**
	 * \brief 获取指定位置的元素.
	 * 
	 * @param index 指定位置.
	 * @return 接受者 index 位置的元素.
	 * @throw 如果 index<0 || index>=Vector::size(), 抛出 IndexOutOfBoundsExeption.
	*/
	inline E& get(e_int index) const {
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, index};
		return m_buffer[index];
	}
	
	/**
		\brief 获取指定元素.
		
		@param item 与欲获取的元素等价的元素.
		@return 接受者中与 item 第一个相等的元素.
		@throw 如果接受者不包含和 item 等价的元素, 抛出 HasNoSuchElementException.
	*/
	E& get(const E& item) const {
		e_int index = this->indexOf(item);
		if (index == -1) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"Vector"}};
		}
		return get(index);
	}

	/**
	 * \brief 获取指定位置的元素.
	 * 
	 * @param index 指定位置.
	 * @return 接受者 index 位置的元素.
	 * @throw 如果 index<0 || index>=Vector::size(), 抛出 IndexOutOfBoundsExeption.
	*/
	inline E& operator[] (e_int index) const {
		return get(index);
	}

	/**
	 * \brief 删除指定位置的元素.
	 * 
	 * @param index 指定的位置.
	 * @return index 位置的元素.
	 * @throw 如果 index<0 或者 index>=Vector::size(), 抛出 IndexOutOfBoundsException.
	*/
	E remove(e_int index) {
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, index};
		
		if (index == m_length-1) {
			return System::move(m_buffer[--m_length]);
		}

		E item = System::move(m_buffer[index]);
		_move(&m_buffer[index], &m_buffer[index+1], m_length-index);
		m_length--;
		return System::move(item);
	}
	
	/**
		\brief 移除指定元素.
		
		@param item 与欲获取的元素等价的元素.
		@return 接受者中与 item 第一个相等的元素.
		@throw 如果接受者不包含和 item 等价的元素, 抛出 HasNoSuchElementException.
	*/
	E remove(const E& item) {
		e_int index = this->indexOf(item);
		if (index == -1) {
			throw  HasNoSuchElementException{__func__, __LINE__, String{L"Vector"}};
		}
		return System::move(remove(index));
	}
	
	/**
		\brief 判断接受者是否包含指定元素.

		@param item 欲查找的项.
		@return 如果接受者包含 item, 返回真; 否则, 返回假.
	*/
	inline e_bool contains(const E& item) const {
		return this->indexOf(item) != -1;
	}

	/**
		\brief 查找元素.

		@param item 欲查找的元素.
		@param offset 查找起始位置, 默认为0, 范围为 0 ~ Vector::size() -1.
		@return 从 offset 开始, 与 item 第一个相等的元素的位置, 如果没有相等元素, 返回 -1.

		@throw 如果 offset < 0, 抛出 IllegalArgumentException.
	*/
	e_int indexOf(const E& item, e_int offset=0) const {
		if (offset < 0) return -1;
		for (e_int i=offset; i<m_length; i++) {
			if (item.equals(m_buffer[i]))
				return i;
		}
		return -1;
	}

	/**
		\brief 倒序查找元素.

		@param item 欲查找的元素.
		@param offset 查找起始位置, 默认为-1, 代表 Vector::size() -1, 范围为 0 ~ Vector::size() -1.
		@return 在 offset 之前, 与 item 相等的最后一个元素的位置, 如果没有相等元素, 返回 -1

		@throw 如果 offset < 0 或 offset >= Vector::size() , 抛出 IndexOutOfBoundsException.
	*/
	e_int lastIndexOf(const E& item, e_int offset=-1) const {
		if (offset == -1) offset = m_length - 1;
		if (offset<0 || offset>=m_length) 
			return -1;

		for (e_int i=offset; i>=0; i--) {
			if (item.equals(m_buffer[i]))
				return i;
		}

		return -1;
	}
	
	/**
	 	\brief 快速排序.
	 	
		如果接受者长度为0, 方法什么都不做.
		<br/>
		使用的位置包括 start 但不包括 end.
		<br/>
		如果 start 等于 end, end 会被加 1.
		<br/>
		元素类型需实现 compare() 方法.
		
		@param start 排序开始位置, 默认为 0, 范围在 0~( Vector::size() -1);
		@param end  排序结尾位置, 默认为 -1, 代表 Vector::size() , 范围在 (0 ~ Vector::size()).
		@throw 如果 start < 0或 start >= Vector::size(), 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > Vector::size(), 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.
	*/
	e_void sort(e_int start=0, e_int end=-1) {
		if (m_length == 0) 
			return;
		if (end == -1)
			end = m_length;
		if (start<0 || start>=m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, start};
		if (end<0 || end>m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, end};
		if (start > end) 
			throw  IllegalArgumentException{__func__, __LINE__};
		if (start == end)
			end++;

		_quickSort(m_buffer, start, end-1);
	}

	/**
		\brief 二叉搜索.
		
		在调用此方法前, 需调用 Vector::sort(), 否则结果无意义.
		<br/>
		如果数组长度为0, 或接受者中不包括与 item 相等的元素, 返回 -1.
		<br/>
		搜索的位置包括 start 但不包括 end.
		<br/>
		如果 start 等于 end, end 会被加 1.
		<br/>
		元素类型需实现 compare() 方法.
		
		@param item 欲查找的元素.
		@param start 填充开始位置, 默认为0, 范围在 0~( Array::length() -1);
		@param end  填充结尾位置, 默认为 -1, 代表 Array::length() , 范围在 (0 ~ Array::length()).
		@return 接受者的 start 到 end-1 之间的第一个与 item 相等的元素位置.
		@throw 如果 start < 0 或 start >= Array::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > Array::length() , 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.
	*/
	e_int search(const E& item, e_int start=0, e_int end=-1) const
	{
		if (m_length == 0) return -1;

		if (end == -1) end = m_length;

		if (start<0 || start>=m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, start};
		if (end<0 || end>m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Vector"}, m_length, end};
		if (start > end) 
			throw IllegalArgumentException{__func__, __LINE__};
		if (start == end) end++;

		return _binarySearch(item, start, end-1);
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
	 * \brief 检查集合是否为空.
	 * 
	 * @return 如果集合为空, 返回真; 否则, 返回假.
	*/
	inline e_bool empty() const {
		return m_length == 0;
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
	 * \brief 缩减内存到正好存储元素.
	 * 
	 * @return 缩减内存后的接受者.
	*/
	Vector& trimToLength() {
		if (m_capacity == 16)
			return *this;
		
		if (m_capacity == m_length)
			return *this;
		
		_updateCapacity((m_length<16)?16:m_length);
		return *this;
	}
	
	/**
	 * \brief 清空容器.
	 * 
	 * 清空内部缓存区并重新初始化.
	*/
	e_void clean() {
		delete [] m_buffer;
		m_capacity = 16;
		m_length   = 0;
		m_buffer   = new E[m_capacity];
	}
	
	/**
	 * \brief 获取迭代器.
	 * 
	 * @return 接受者的迭代器.
	*/
	inline Iterator<E>* iterator() const {
		return new VcIterator{this, m_buffer, m_length};
	}
private:
	e_void _updateCapacity(e_int newCapacity) {
		E* newBuffer = _createNewBuffer(newCapacity);
		_move(newBuffer, m_buffer, m_length);
		delete [] m_buffer;
		m_buffer = newBuffer;
	}

	E* _createNewBuffer(e_int newCapacity) {
		m_capacity   = newCapacity;
		E* newBuffer = new E[m_capacity];
		return newBuffer;
	}

	inline e_void _copy(E* des, E* src) {
		for (e_int i=0; i<m_length; i++) {
			des[i] = src[i];
		}
	}

	inline e_void _move(E* des, E* src, e_int length) {
		for (e_int i=0; i<length; i++) {
			des[i] = System::move(src[i]);
		}
	}

	inline e_void _moveBack(E* des, E*src, e_int length) {
		for (e_int i=length-1; i>=0; i--) {
			des[i] = System::move(src[i]);
		}
	}

	e_void _quickSort(E* its, e_int start, e_int end) {
		if ((end-start) <= 0) {
			return;
		}

		e_int center = (start+end) / 2;

		E tmp    	= System::move(its[end]);
		its[end]    = System::move(its[center]);
		its[center] = System::move(tmp);

		e_int left  = start-1;
		e_int right = end;
		E& pivot    = its[end];

		do
		{ // Move the bounds inward until they meet
			while ((left<end) && its[++left].compare(pivot)==-1); // Move l right and
			while ((right>start) && (left<right) && (pivot.compare(its[--right])==-1)); // r left

			E tmp      = System::move(its[left]);
			its[left]  = System::move(its[right]);
			its[right] = System::move(tmp);// Swap out-of-place values

		} while (left < right); // Stop when they cross

		tmp 	  = System::move(its[end]);
		its[end]  = System::move(its[left]);
		its[left] = System::move(tmp);

		_quickSort(its, start, left-1);
		_quickSort(its, left+1, end);
	}

	e_int _binarySearch(const E& item, e_int start, e_int end) const
	{
		if ((end - start) < 0) return -1;
		e_int center = (end + start) / 2;
		e_int state = m_buffer[center].compare(item);
		if (state == 0)  return center;					//注意这里的+1
		if (state == -1) return _binarySearch(item, center+1, end);
		if (state == 1)  return _binarySearch(item, start, center-1);//注意这里的 -1

		return -1; //never arrive here
	}
}; // Vector
} // Easy

#endif // _EASY_VECTOR_H_

