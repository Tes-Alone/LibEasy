/*
 * File Name:	BasicArray.hpp
 * Date/Time:
 * 	Start: 	2019-12-06 17:34 week:5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_BASICARRAY_H_
#define _EASY_BASICARRAY_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "System.hpp"
#include "Math.hpp"
#include "EasyC.hpp"

namespace Easy {

/**
 * \class BasicArray
 * 
 * \brief %Array of basic types.
 *
 * <b>Note: </b> After moved, the receiver will become empty.<br/><br/>
 * 
 * @author Tes Alone
 * @since 1.0
*/

class String;

template<class E>
class BasicArray {
private:
	E* m_arr;
	e_int m_length;
public:
	/**
	 * \brief ctor.
	 * 
	 * If length is less than 0, length will be fixed to 0.
	 * 
	 * @param length array length, may be 0, defualt 0.
	*/
	explicit BasicArray(e_int length=0) {		
		if (length <= 0) {
			m_arr    = nullptr;
			m_length = 0;
		} else {
			m_length = length;
			m_arr    = new E[m_length];
			fill(0);
		}
	}

	/**
	 * \brief dtor.
	 */
	inline ~BasicArray() {
		delete [] m_arr;
	}

	/**
	 * \brief copy ctor.
	 * 
	 * @param copy
	*/
	BasicArray(const BasicArray& copy) {
		if (copy.m_length == 0) {
			m_length = 0;
			m_arr    = nullptr;
		} else {
			m_length = copy.m_length;
			m_arr    = new E[m_length];
			e_memcopy(m_arr, copy.m_arr, m_length*sizeof(E));
		}
	}

	/**
	 * \brief move ctor.
	 * 
	 * @param move
	*/
	BasicArray(BasicArray&& move) {
		m_length = move.m_length;
		m_arr    = move.m_arr;
		move.m_arr    = nullptr;
		move.m_length = 0;
	}

	/**
	 * \brief copy assignment.
	 * 
	 * @param copy
	 * @return copy's deeply copy.
	*/
	BasicArray& operator=(const BasicArray& copy) {
		delete [] m_arr;
		if (copy.m_length == 0) {
			m_length = 0;
			m_arr    = nullptr;
		} else {
			m_length = copy.m_length;
			m_arr    = new E[m_length];
			e_memcopy(m_arr, copy.m_arr, m_length*sizeof(E));
		}
		return *this;
	}

	/**
	 * \brief move assignment.
	 * 
	 * @param move
	 * @return move's movely copy.
	*/
	BasicArray& operator=(BasicArray&& move) {
		delete [] m_arr;
		m_length = move.m_length;
		m_arr    = move.m_arr;
		move.m_arr    = nullptr;
		move.m_length = 0;
		return *this;
	}
	
	/**
	 * \brief Check equality.
	 * 
	 * If receiver and other has the same length, then
	 * Compare receiver and other, one by one, if they are all equals, return true;
	 * otherwise return false.
	 * If their length not equal, return false.
	 * 
	 * @param other the object compared to receiver.
	 * @return result.
	 * */
	
	e_bool equals(const BasicArray<E>& other) const {
		if (m_length != other.m_length) return false;
		for (e_int i=0; i<m_length; i++) {
			if (!equals(m_arr[i], other[i])) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * \brief Check order.
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
	e_int compare(const BasicArray<E>& other) const {
		e_int len = Math::min(this->m_length, other.m_length);

	    for (e_int i=0; i<len; i++){
	    	e_int flag = compare(m_arr[i], other[i]);
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
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	e_int hashCode() const;
	
	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	String toString() const;
	
	/**
	 * \brief Check whether is empty.
	 * 
	 * Check if receiver's length is 0. If so, return true, else return false.
	 * 
	 * @return result.
	*/
	e_bool empty() const {
		return m_length == 0;
	}

	/**
	 * \brief Fill array.
	 * 
	 * Use item filling receiver.
	 * Will erase old values.
	 * 
	 * @param item .
	*/
	e_void fill(E item) {
		for (e_int i=0; i<m_length; i++) {
			m_arr[i] = item;
		}
	}

	/**
	 * \brief Set value.
	 * 
	 * Set value at the index-th spot of receiver to item.
	 * 
	 * @param index setting location.
	 * @param item .
	 * @throw if index<0 or index>=BasicArray::length(), throws IndexOutOfBoundsException.
	*/
	e_void setAt(e_int index, E item);

	/**
	 * \brief Get value.
	 * 
	 * Get value at the index-th spot of receiver.
	 * 
	 * @param index getting location.
	 * @return the index-th value of receiver.
	 * @throw if index<0 or index>=BasicArray::length(), throws IndexOutOfBoundsException.
	*/
	E& getAt(e_int index) const;

	/**
	 * \brief Get value.
	 * 
	 * Get value at the index-th spot.
	 * 
	 * @param index getting location.
	 * @return the index-th value of receiver.
	 * @throw if index<0 or index>=BasicArray::length(), throws IndexOutOfBoundsException.
	*/
	inline E& operator[] (e_int index) const {
		return getAt(index);
	}

	/**
	 * \brief Get C array.
	 * 
	 * DO NOT DELETE RETURNED VALUE.
	 *
	 * @return receiver's internal memory buffer address.
	*/
	inline E* toCArray() const {
		return m_arr;
	}

	/**
	 * \brief Get length.
	 * 
	 * @return receiver's length.
	*/
	inline e_int length() const {
		return m_length;
	}

	/**
		\brief Find element.

		From offset, check whether there is a element of receiver equals item, 
		one by one, if so, return the element's index, else return -1.
		
		If offset < 0, return -1;
		
		@param item .
		@param offset finding start, defualt 0, range betweens 0 ~ Integer::MAX.
		@return result.
	*/
	e_int indexOf(E item, e_int offset=0) const {
		if (offset < 0) 
			return -1;
		for (e_int i=offset; i<m_length; i++) {
			if (equals(item, m_arr[i]))
				return i;
		}
		return -1;
	}

	/**
		\brief Finding element forward.

		From offset, forward, check whether there is a element of receiver equals item, 
		one by one, if so, return the element's index, else return -1.
		If offset < 0 or offset >= BasicArray::length() return -1.
		@param item .
		@param offset findind start, defualt -1, indicates BasicArray::length() -1, range betweens 0 ~ BasicArray::length() -1.
		@return result.
	*/
	e_int lastIndexOf(E item, e_int offset=-1) const;

	/**
		\brief Check whether contains some element.

		This is a linear search, for binary search, try this:
		<pre><code>
		...
		arr.sort();
		if (arr.search(item)==-1) ...
		else ...
		...
		</code></pre>

		@param item .
		@return if receiver contains item, return true; otherwise, return false.
	*/
	e_bool contains(E item) const {
		return indexOf(item) != -1;
	}
	
	/**
		\brief Get subarray.
		Subarray range is from start to end, exclude end.
		If start equals end, end increases one.
		<br/>

		@param start start of searching, default is 0, between 0 ~ BasicArray::length()-1;
		@param end   end of searching, default is -1, indicates BasicArray::length() , between 0 ~ BasicArray::length().
		@return receiver's subarray from start to end(exclude).
		
		@throw if start < 0 or start >= BasicArray::length(), throw an IndexOutOfBoundsException.
	 	@throw if end < 0 or end > BasicArray::length(), throw an IndexOutOfBoundsException.
	 	@throw if start > end, throw an IllegalArgumentException.
	*/
	BasicArray<E> subArray(e_int start=0, e_int end=-1) const;
	
	
	/**
		\brief Replace subarray.
		Replace range is from start to end, exclude end.
		If start equals end, end increases one.
		<br/>

		@param arr   new subarray.
		@param start start of searching, default is 0, between 0 ~ BasicArray::length()-1;
		@param end   end of searching, default is -1, indicates BasicArray::length() , between 0 ~ BasicArray::length().
		
		@throw if start < 0 or start >= BasicArray::length(), throw an IndexOutOfBoundsException.
	 	@throw if end < 0 or end > BasicArray::length(), throw an IndexOutOfBoundsException.
	 	@throw if start > end, throw an IllegalArgumentException.
	*/
	e_void replace(const BasicArray<E>& arr, e_int start=0, e_int end=-1);

	/**
	 * \brief sort.
	 * 
	 * This method is a simple quicksort implement.
	 * <br/>
	 * If receiver is empty, the method do nothing.
	 * <br/>
	 * The sorted range include start, exclude end, if start equals end, end will 
	 * be added one.
	 *
	 *	@param start start of sorted range, default is 0, can be a number between 0 and BasicArray::length()-1;
	 *	@param end  end of sorted range, default is -1, indicates the BasicArray::length() , can be a number between 0 and BasicArray::length().
	 *	@throw if start < 0 or start >= BasicArray::length(), throw an IndexOutOfBoundsException.
	 *	@throw if end < 0 or end > BasicArray::length(), throw an IndexOutOfBoundsException.
	 *	@throw if start > end, throw an IllegalArgumentException.
	*/
	e_void sort(e_int start=0, e_int end=-1) ;

	/**
		\brief binary search.

		This is the binary search implement.
		<br/>
		
		Before calling this method, first call BasicArray::sort(), or you can be sure
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
		@param start start of searching, default is 0, between 0 ~ BasicArray::length()-1;
		@param end   end of searching, default is -1, indicates BasicArray::length() , between 0 ~ BasicArray::length().
		@return searched result.
		@throw if start < 0 or start >= BasicArray::length(), throw an IndexOutOfBoundsException.
	 	@throw if end < 0 or end > BasicArray::length(), throw an IndexOutOfBoundsException.
	 	@throw if start > end, throw an IllegalArgumentException.
	*/
	e_int search(E item, e_int start=0, e_int end=-1) const;
	
	/**
	 * \brief Get data.
	 * 
	 * Get data of receiver, the result independs with receiver.
	 * 
	 * @return the data of receiver.
	 * */
	BasicArray<e_ubyte> toUBytes() const;
	
private:
	e_bool equals(e_uint num1, e_uint num2) const ;
	e_bool equals(e_int num1, e_int num2) const ;
	
	e_bool equals(e_ushort num1, e_ushort num2) const  ;
	e_bool equals(e_short num1, e_short num2) const  ;
	
	e_bool equals(e_ubyte num1, e_ubyte num2) const;
	
	e_bool equals(e_byte num1, e_byte num2) const;
	
	e_bool equals(e_ulong num1, e_ulong num2) const ;
	e_bool equals(e_long num1, e_long num2) const ;
	
	e_bool equals(e_char num1, e_char num2) const ;
	
	e_bool equals(e_bool truth1, e_bool truth2) const;
	
	e_bool equals(e_float f1, e_float f2) const ;
	
	e_bool equals(e_double d1, e_double d2) const ;
	
	e_int compare(e_uint num1, e_uint num2) const ;
	e_int compare(e_int num1, e_int num2) const ;
	
	e_int compare(e_ushort num1, e_ushort num2) const;
	e_int compare(e_short num1, e_short num2) const;
	
	e_int compare(e_ubyte num1, e_ubyte num2) const ;
	
	e_int compare(e_byte num1, e_byte num2) const ;
	
	e_int compare(e_ulong num1, e_ulong num2) const ;
	e_int compare(e_long num1, e_long num2) const ;
	
	e_int compare(e_char num1, e_char num2) const ;
	
	e_int compare(e_bool truth1, e_bool truth2) const;
	
	e_int compare(e_float f1, e_float f2) const ;
	
	e_int compare(e_double d1, e_double d2) const ;
	
	String toString(e_ubyte num) const ;
	
	String toString(e_byte num) const ;
	
	String toString(e_ushort num) const ;
	
	String toString(e_short num) const ;
	
	String toString(e_uint num) const ;
	
	String toString(e_int num) const ;
	
	String toString(e_ulong num) const ;
	
	String toString(e_long num) const ;
	
	String toString(e_bool b) const ;
	
	String toString(e_char c) const ;
	
	String toString(e_float f) const ;
	
	String toString(e_double d) const ;
	
	e_void _quickSort(E* its, e_int start, e_int end) ;

	e_int _binarySearch(E item, e_int start, e_int end)  const;
}; // BasicArray

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_bool.
 * */
typedef BasicArray<e_bool>    BoolArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_char.
 * */
typedef BasicArray<e_char>    CharArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_ubyte.
 * */
typedef BasicArray<e_ubyte>   UByteArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_byte.
 * */
typedef BasicArray<e_byte>    ByteArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_ushort.
 * */
typedef BasicArray<e_short>   UShortArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_short.
 * */
typedef BasicArray<e_short>   ShortArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_uint.
 * */
typedef BasicArray<e_uint>      UIntArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_int.
 * */
typedef BasicArray<e_int>       IntArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_ulong.
 * */
typedef BasicArray<e_ulong>     ULongArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_long.
 * */
typedef BasicArray<e_long>     LongArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_float.
 * */
typedef BasicArray<e_float>   FloatArray;

/**
 * \brief BasicArray template instances.
 * 
 * Basic types array template instance for e_double.
 * */
typedef BasicArray<e_double> DoubleArray;

} // Easy

#endif // _EASY_BASICARRAY_H_

