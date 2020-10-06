/*
 * File Name:	Array.hpp
 * Date/Time:
 * 	Start: 	2019-11-28 13:22 week:4
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_ARRAY_H_
#define _EASY_ARRAY_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "StringBuffer.hpp"
#include "System.hpp"

namespace Easy {

/**
 * \class Array
 * 
 * \brief A static %array implement.
 * 
 * This class can used as a container by a class who has equals(), compare() and toString();
 * and the class needs have a <b>zero argument (or equality) ctor</b>.
 * 
 * <b>Note: </b> although this class is static, after moving, the object will become empty.<br/>
 * 
 * For basic types %array, see BasicArray.
 * 
 * @author Tes Alone
 * @since  1.0
*/

template<class E>
class Array {
private:
	E*    m_arr;
	e_int m_length;
public:
	/**
	 * \brief ctor.
	 * 
	 * Create a static array object, whose length is len.
	 * <br/>
	 * If len is less than 0, method wil fix it as 0.
	 * 
	 * @param len array length, may be 0, and default is 0.
	*/
	explicit Array(e_int len=0) {
		if (len <= 0) {
			m_length = 0;
			m_arr = nullptr;
		} else {
			m_length = len;
			m_arr    = new E[m_length];
		}
	}

	/**
	 * \brief dtor.
	*/
	inline ~Array() {delete [] m_arr;}

	/**
	 * \brief copy ctor.
	 * 
	 * @param copy .
	 * 
	*/
	Array(const Array& copy) {
		if (copy.m_length == 0) {
			m_length = 0;
			m_arr    = nullptr;
		} else {
			m_length = copy.m_length;
			m_arr    = new E[m_length];
			_copy(m_arr, copy.m_arr);
		}
	}

	/**
	 * \brief move ctor.
	 * 
	 * After moving, move will be a zero length array.
	 * 
	 * @param move .
	*/
	Array(Array&& move) {
		m_length = move.m_length;
		m_arr    = move.m_arr;
		move.m_arr    = nullptr;
		move.m_length = 0;
	}
	
	/**
	 * \brief copy assignment.
	 * 
	 * @param copy .
	 * @return a copy of copy.
	*/
	Array& operator=(const Array& copy) {
		delete [] m_arr;
		if (copy.m_length == 0) {
			m_length = 0;
			m_arr    = nullptr;
		} else {
			m_length = copy.m_length;
			m_arr    = new E[m_length];
			_copy(m_arr, copy.m_arr);
		}
		return *this;
	}
	
	/**
	 * \brief move assignment.
	 * 
	 * After moving, move will be a zero length array.
	 * 
	 * @param move .
	 * @return the one as move.
	*/
	Array& operator=(Array&& move) {
		delete [] m_arr;
		m_length = move.m_length;
		m_arr    = move.m_arr;
		move.m_arr    = nullptr;
		move.m_length = 0;
		return *this;
	}
	
	/**
	 * \brief check equality.
	 * 
	 * The method compare receiver and other, if receiver's elements equals 
	 * other's, one by one, return true, otherwise retunr false.
	 * <br/>
	 * Element types need implement equals().
	 * 
	 * @param other .
	 * @return  compared result.
	 * */
	e_bool equals(const Array<E>& other) const {
		if (m_length != other.m_length) return false;
		for (e_int i=0; i<m_length; i++) {
			if (!m_arr[i].equals(other[i])) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * \brief check order.
	 * 
	 * The method compare receiver and other, if receiver's
	 * elements is bigger than other's, one by one, return 1, 
	 * equals return 0, otherwise return -1.
	 * <br/>
	 * Element types need implement compare().
	 * 
	 * @param other .
	 * @return compared result.
	 * */
	e_int compare(const Array<E>& other) const {
		e_int len = Math::min(this->m_length, other.m_length);

	    for (e_int i=0; i<len; i++){
	    	e_int flag = this->m_arr[i].compare(other[i]);
	        if (flag > 0)
	            return 1;
	        if (flag < 0)
	            return -1;
	    }
	
	    if (this->m_length == other.m_length)
	        return 0;
	
	    return (this->m_length > other.m_length) ? 1 : -1;
	}
	
	/**
	 * \brief get hash code.
	 * 
	 * Get receiver`s hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	e_int hashCode() const {
		return Long::hashCode(reinterpret_cast<e_long>(this));
	}

	/**
	 * \brief To string represent．
	 * 
	 * Element types need implement toString().
	 * 
	 * @return receiver`s string represent.
	*/
	String toString() const {
		StringBuffer sb{};
		sb.append(String{L"["});
		e_int i = 0;
		for (; i<m_length-1; i++) {
			sb.append(m_arr[i].toString());
			sb.append(String{L","});
		}
		if (m_length > 0) {
			sb.append(m_arr[i].toString());
		}
		return System::move(sb.append(String{L"]"}).toString());
	}

	/**
	 * \brief set item.
	 * 
	 * Set index-th element of receiver to item, use copy ctor.
	 * 
	 * @param index setting position.
	 * @param item setting value.
	 * @throw if index<0 or index>=Array::length(), throw an IndexOutOfBoundsException.
	*/
	e_void setAt(e_int index, const E& item) {
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, index};

		m_arr[index] = item;
	}

	/**
	 * \brief set item.
	 * 
	 * Set index-th element of receiver to item, use move ctor.
	 * 
	 * @param index setting position.
	 * @param item setting value.
	 * @throw if index<0 or index>=Array::length(), throw an IndexOutOfBoundsException.
	*/
	e_void setAt(e_int index, E&& item) {
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, index};

		m_arr[index] = System::move(item);
	}

	/**
	 * \brief get element.
	 * 
	 * Get the index-th element of receiver.
	 * 
	 * @param index position of wanted element.
	 * @return the index-th element of receiver.
	 * @throw if index<0 or index>=Array::length(), throw an IndexOutOfBoundsException.
	*/
	E& getAt(e_int index) const {
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, index};

		return m_arr[index];
	}

	/**
	 * \brief get element.
	 * 
	 * Get the index-th element of receiver.
	 * 
	 * @param index position of wanted element.
	 * @return the index-th element of receiver.
	 * @throw if index<0 or index>=Array::length(), throw an IndexOutOfBoundsException.
	*/
	inline E& operator[] (e_int index) const {
		return getAt(index);
	}

	/**
	 * \brief get array length.
	 * 
	 * Get length of receiver.
	 * 
	 * @return length of receiver.
	*/
	inline e_int length() const {
		return m_length;
	}

	/**
	 * \brief sort.
	 * 
	 * This method is a simple quicksort implement.
	 * <br/>
	 * Element types need implement compare().
	 * <br/>
	 * If receiver is empty, the method do nothing.
	 * <br/>
	 * The sorted range include start, exclude end, if start equals end, end will 
	 * be added one.
	 *
	 *	@param start start of sorted range, default is 0, can be a number between 0 and Array::length()-1;
	 *	@param end  end of sorted range, default is -1, indicates the Array::length() , can be a number between 0 and Array::length().
	 *	@throw if start < 0 or start >= Array::length(), throw an IndexOutOfBoundsException.
	 *	@throw if end < 0 or end > Array::length(), throw an IndexOutOfBoundsException.
	 *	@throw if start > end, throw an IllegalArgumentException.
	*/
	e_void sort(e_int start=0, e_int end=-1) {
		if (m_length == 0) 
			return;
		if (end == -1)
			end = m_length;
		if (start<0 || start>=m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, start};
		if (end<0 || end>m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, end};
		if (start > end) 
			throw  IllegalArgumentException{__func__, __LINE__};
		if (start == end)
			end++;

		_quickSort(m_arr, start, end-1);
	}

	/**
		\brief find element.
		
		This method return the index of the first element equals item after offset-th element,
		in receiver. If there is no such element, or offset is less than 0 or not less than Array::length(), return -1.
		<br/>
		Element types need implement equals().

		@param item .
		@param offset start finding position, default is 0, between 0 and Integer::MAX
		@return result.
	*/
	e_int indexOf(const E& item, e_int offset=0) const {
		if (offset < 0) 
			return -1;
		for (e_int i=offset; i<m_length; i++) {
			if (item.equals(m_arr[i]))
				return i;
		}
		return -1;
	}

	/**
		\brief find element forward.
		
		This method return the index of the last element which equals item before offset,
		in receiver.
		If there is no such element, return -1.
		If offset<0 || offset>=Array::length(), return -1;
		<br/>
		Element types need implement equals().

		@param item .
		@param offset start finding position, default is 0, between 0 and Array::length() -1.
		@return resutl.
	*/
	e_int lastIndexOf(const E& item, e_int offset=-1) const {
		if (offset == -1) 
			offset = m_length - 1;
		if (offset<0 || offset>=m_length) 
			return -1;

		for (e_int i=offset; i>=0; i--) {
			if (item.equals(m_arr[i]))
				return i;
		}

		return -1;
	}

	/**
		\brief check whether has some element.

		Check whether receiver contains a element which equals item. If so, return true,
		otherwise return false.
		<br/>
		Element types need implement equals().

		This method is a linear search, use binary search, do as following:
		<pre><code>
		...
		arr.sort();
		if (arr.search(item)==-1) ...
		else ...
		...
		</code></pre>

		@param item .
		@return checked resutl.
	*/
	e_bool contains(const E& item) const {
		return this->indexOf(item) != -1;
	}

	/**
	 * \brief check whether empty.
	 * 
	 * Check whether receiver is empty.
	 * 
	 * @return if is empty, return true, otherwise return false.
	*/
	e_bool empty() const {
		return m_length == 0;
	}

	/**
		\brief binary search.

		This is the binary search implement.
		<br/>
		Element types need implement equals().
		<br/>
		
		Before calling this method, first call Array::sort(), or you can be sure
		the receiver stores its element in increase.
		<br/>
		
		The method return a element which equals item, may not be the first of last,
		in receiver.
		If there is no such element , return -1.
		<br/>
		Searching range from start to end, exclude end.
		If start equals end, end increases one.
		<br/>
		

		@param item .
		@param start start of searching, default is 0, between 0 ~ Array::length()-1;
		@param end   end of searching, default is -1, indicates Array::length() , between 0 ~ Array::length().
		@return searched result.
		@throw if start < 0 or start >= Array::length(), throw an IndexOutOfBoundsException.
	 	@throw if end < 0 or end > Array::length(), throw an IndexOutOfBoundsException.
	 	@throw if start > end, throw an IllegalArgumentException.
	*/
	e_int search(const E& item, e_int start=0, e_int end=-1) const {
		if (m_length == 0) 
			return -1;

		if (end == -1) 
			end = m_length;

		if (start<0 || start>=m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, start};
		if (end<0 || end>m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"Array"}, m_length, end};
		if (start > end) 
			throw  IllegalArgumentException{__func__, __LINE__};
		if (start == end) 
			end++;

		return _binarySearch(item, start, end-1);
	}
private:
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

	e_int _binarySearch(const E& item, e_int start, e_int end) const {
		if ((end - start) < 0) return -1;
		e_int center = (end + start) / 2;
		e_int state = m_arr[center].compare(item);
		if (state == 0) return center;					//注意这里的+1
		if (state == -1) return _binarySearch(item, center+1, end);
		if (state == 1) return _binarySearch(item, start, center-1);//注意这里的 -1

		return -1; //never arrive here
	}

	e_void _copy(E* des, E* src) {
		for (e_int i=0; i<m_length; i++) {
			des[i] = src[i];
		}
	}
}; // Array
} // Easy

#endif // _EASY_ARRAY_H_

