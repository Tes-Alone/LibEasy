/*
 * File Name:	BasicBuffer.hpp
 * Date/Time:
 * 	Start: 	2020年02月06日 19:03:33 周四
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_BASICBUFFER_H_
#define _EASY_BASICBUFFER_H_

#include "BaseTypes.hpp"
#include "BasicArray.hpp"
#include "Exception.hpp"
#include "System.hpp"
#include "EasyC.hpp"

namespace Easy {

/**
	\class BasicBuffer
	
	\brief Basic type buffer.
	
	<b>Note: </b> After moved, the receiver will become empty.<br/><br/>
  
    @author Tes Alone
    @since 1.0
*/

template<class E>
class BasicBuffer {
	E* m_buffer;
	e_int m_capacity;
	e_int m_length;
public:
	/**
	 * \brief ctor.
	 * 
	 * if initCapacity less than 16, initCapacity will be 16.
	 *
	 * @param initCapacity buffer capacity, default 16.
	*/
	explicit BasicBuffer(e_int initCapacity=16) {
		if (initCapacity<16)
			initCapacity = 16;
		_init(initCapacity);
	}
	
	/**
	 * \brief dtor.
	*/
	inline ~BasicBuffer() {delete [] m_buffer;}
	
	/**
	 * \brief copy ctor.
	 * 
	 * @param copy
	*/
	BasicBuffer(const BasicBuffer& copy) {
		m_length   = copy.m_length;
		m_capacity = (m_length<16) ? 16 : m_length;
		m_buffer   = new E[m_capacity];
		e_memcopy(m_buffer, copy.m_buffer, m_length*sizeof(E));
	}
	
	/**
	 * \brief move ctor.
	 * 
	 * @param move
	*/
	BasicBuffer(BasicBuffer&& move) {
		m_length   = move.m_length;
		m_capacity = move.m_capacity;
		m_buffer   = move.m_buffer;
		move.m_capacity = 16;
		move.m_buffer   = new E[m_capacity];
		move.m_length   = 0;
	}
	
	/**
	 * \brief copy assignment.
	 * 
	 * @param copy
	 * @return copy's deeply copy.
	*/
	BasicBuffer& operator=(const BasicBuffer& copy) {
		delete [] m_buffer;
		m_length   = copy.m_length;
		m_capacity = (m_length<16) ? 16 : m_length;
		m_buffer   = new E[m_capacity];
		e_memcopy(m_buffer, copy.m_buffer, m_length*sizeof(E));
		return *this;
	}
	
	/**
	 * \brief move assignment.
	 * 
	 * @param move
	 * @return move's movely copy.
	*/
	BasicBuffer& operator=(BasicBuffer&& move) {
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
	 * \brief Check equality.
	 * 
	 * If receiver and other has the same length, then
	 * compare receiver and other, one by one, if they are all equals, return true;
	 * otherwise return false.
	 * If their length not equal, return false.
	 * 
	 * @param other the object compared to receiver.
	 * @return result.
	 * */
	e_bool equals(const BasicBuffer<E>& other) const {
		if (m_length != other.m_length) return false;
		for (e_int i=0; i<m_length; i++) {
			if (!equals(m_buffer[i], other[i])) {
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
	e_int compare(const BasicBuffer<E>& other) const {
		e_int len = Math::min(this->m_length, other.m_length);

	    for (e_int i=0; i<len; i++){
	    	e_int flag = compare(m_buffer[i], other[i]);
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
	e_int hashCode() const {
		return Long::hashCode(reinterpret_cast<e_long>(this));
	}
	
	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	String toString() const;
	
	/**
	 * \brief Append element.
	 * 
	 * @param item added element.
	 * @return receiver after adding.
	*/
	BasicBuffer& append(E item) {
		e_int resultLen = m_length + 1;
		if (resultLen > m_capacity) {
			_updateCapacity(m_capacity*2);
		}
		m_buffer[m_length++] = item;
		return *this;
	}
	
	/**
		\brief Append BasicArray.
		
		if start equals end, end incs  1.
		start include, end exclude.
		
		@param arr appended array, mey be zero array.
		@param start arr's start pos of appending, default 0, range 0 ~ arr's length-1.
		@param end  arr's end pos of appending, default -1, indicates  arr's length; range 0 ~ arr's length.
		@return receiver after appending.
		
		@throw if start<0 or start >= arr's length, throws IndexOutOfBoundsException.
		@throw if end<0 or end>arr's length, throws IndexOutOfBoundsException.
		@throw if start > end, throws IllegalArgumentException. 
	*/
	BasicBuffer& append(const BasicArray<E>& arr, e_int start=0, e_int end=-1) {
		if (arr.length() == 0) return *this;
	
		if (end == -1)	end = arr.length();
		if (start<0 || start>=arr.length()) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, arr.length(), start};
		if (end<0 || end>arr.length()) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, arr.length(), end};
		if (start > end) throw IllegalArgumentException{__func__, __LINE__};
		if (start == end) end++;
		
		_append(arr.toCArray(), arr.length(), start, end);
		
		return *this;
	}
	
	/**
	 * \brief Insert element.
	 * 
	 * @param offset pos of inserting.
	 * @param item inserted value.
	 * @return receiver after inserting.
	 * @throw if offset<0 ||index>BasicBuffer::length(); throws IndexOutOfBoundsException.
	*/
	BasicBuffer& insert(E item, e_int offset) {
		if (offset<0 || offset>m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, offset};
		
		e_int resultLen = m_length + 1;
		if (resultLen > m_capacity) {
			_updateCapacity(m_capacity*2);
		}
		
		//memmove_s(&m_buffer[offset+1], (m_length-offset)*sizeof(E), &m_buffer[offset], (m_length-offset)*sizeof(E));
		e_memmove(&m_buffer[offset+1], &m_buffer[offset], (m_length-offset)*sizeof(E));
		m_buffer[offset] = item;
		m_length++;
		return *this;
	}
	
	/**
		\brief Insert BasicArray.
		
		@param arr inserted basic type array, may be zero array.
		@param offset pos of inserting,range 0 ~ BasicBuffer::length().
		@return receiver after inserting.
		
		@throw if offset<0 or offset>BasicBuffer::length(), throws IndexOutOfBoundsException.
	*/
	BasicBuffer& insert(const BasicArray<E>& arr, e_int offset) {
		if (offset<0 || offset>m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, offset};
		
		if (arr.length() == 0)
			return *this;
		
		_insert(arr.toCArray(), arr.length(), offset);

		return *this;
	}
	
	/**
		\brief Remove subarray.
		
		if start equals end, end incs 1.
		start include, end exclude.
		
		@param start start pos of removing, default 0, range 0 ~ BasicBuffer::length()-1.
		@param end   end pos of removing, default -1, indicate BasicBuffer::length(), range 为 0 ~ BasicBuffer::length().
		@return receiver after removing.
		
		@throw if start<0 or start>=BasicBuffer::length(), throws IndexOutOfBoundsException.
		@throw if end<0 or end>BasicBuffer::length(), throws IndexOutOfBoundsException.
		@throw if start>end, throws IllegalArgumentException.
	*/
	BasicBuffer& remove(e_int start=0, e_int end=-1) {
		if (end == -1)	end = m_length;
		if (start<0 || start>=m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, start};
		if (end<0 || end>m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, end};
		if (start > end) throw IllegalArgumentException{__func__, __LINE__};
		if (start == end) end++;
		
		e_int resultLen = m_length - (end-start);
		e_int moveLen   = m_length - end;
		
		_remove(resultLen, moveLen, start, end);
		
		return *this;
	}
	
	/**
		\brief Get subarray.
		
		if start equals end, end incs 1.
		start include, end exclude.
		
		@param start start pos of getting, default 0, range 0 ~ BasicBuffer::length()-1.
		@param end   end pos of getting, default -1, indicate BasicBuffer::length(), range 为 0 ~ BasicBuffer::length().
		@return sub array.
		
		@throw if start<0 or start>=BasicBuffer::length(), throws IndexOutOfBoundsException.
		@throw if end<0 or end>BasicBuffer::length(), throws IndexOutOfBoundsException.
		@throw if start>end, throws IllegalArgumentException.
	*/
	BasicArray<E> subArray(e_int start=0, e_int end=-1) {
		if (m_length == 0) return BasicArray<E>{0};		
		if (end == -1)	end = m_length;
		if (start<0 || start>=m_length) 
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, start};
		if (end<0 || end>m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, end};
		if (start > end) throw IllegalArgumentException{__func__, __LINE__};
		if (start == end) end++;
		BasicArray<E> sub{end-start};
		for (e_int i=start; i<end; i++) {
			sub[i-start] = m_buffer[i];
		}
		return System::move(sub);
	}
	
	/**
	 * \brief Get C array.
	 * 
	 * DO NOT DELETE RETURNED VALUE.
	 *
	 * @return receiver's internal memory buffer address.
	*/
	inline const E* toCArray() const {
		return m_buffer;
	}
	
	/**
	 * \brief Get length.
	 * 
	 * @return the number of receiver's elements.
	*/
	inline e_int length() const {
		return m_length;
	}
	
	/**
	 * \brief Check whether is empty.
	 * 
	 * @return receiver's length == 0.
	*/
	inline e_bool empty() const {
		return m_length == 0;
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
		\brief Check whether contains some element.

		This is a linear search.

		@param item .
		@return if receiver contains item, return true; otherwise, return false.
	*/
	inline e_bool contains(const E item) const {
		return this->indexOf(item) != -1;
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
	e_int indexOf(const E item, e_int offset=0) const {
		if (offset < 0) return -1;
		for (e_int i=offset; i<m_length; i++) {
			if (equals(item, m_buffer[i]))
				return i;
		}
		return -1;
	}
	
	/**
		\brief Finding element forward.

		From offset, forward, check whether there is a element of receiver equals item, 
		one by one, if so, return the element's index, else return -1.
		
		If offset < 0 or offset >= BasicBuffer::length() return -1.
		
		@param item .
		@param offset findind start, defualt -1, indicates BasicBuffer::length() -1, range betweens 0 ~ BasicBuffer::length() -1.
		@return result.
	*/
	e_int lastIndexOf(const E item, e_int offset=-1) const {
		if (offset == -1) offset = m_length - 1;
		if (offset<0 || offset>=m_length) 
			return -1;

		for (e_int i=offset; i>=0; i--) {
			if (equals(item, m_buffer[i]))
				return i;
		}

		return -1;
	}
	
	/**
	 * \brief Get element.
	 * 
	 * Get the index-th element of receiver.
	 * 
	 * @param index pos of getting.
	 * @return the index-th element .
	 * @throw if index<0 || index>=BasicBuffer::length(), throws IndexOutOfBoundsExeption.
	*/
	inline E& valueAt(e_int index) const {
		if (index<0 || index>=m_length) {
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicBuffer"}, m_length, index};
		}
		return m_buffer[index];
	}
	
	/**
	 * \brief Get element.
	 * 
	 * Get the index-th element of receiver.
	 * 
	 * @param index pos of getting.
	 * @return the index-th element .
	 * @throw if index<0 || index>=BasicBuffer::length(), throws IndexOutOfBoundsExeption.
	*/
	inline E& operator[](e_int index) const {
		return valueAt(index);
	}
	
	/**
	 * \brief Trim capacity to fix receiver's length.
	 * 
	 * @return receiver after trimming.
	*/
	BasicBuffer& trimToLength() {
		if (m_capacity == 16)
			return *this;
		
		if (m_capacity == m_length)
			return *this;
		
		_updateCapacity((m_length<16)?16:m_length);
		return *this;
	}
	
	/**
	 * \brief Clean buffer.
	 * 
	 * Clean buffer and reinitialize.
	*/
	e_void clean() {
		delete [] m_buffer;
		m_capacity = 16;
		m_length   = 0;
		m_buffer   = new E[m_capacity];
	}
	
	/**
	 * \brief Change size.
	 * 
	 * Change receiver's length.
	 * If newSize bigger than old length, change receiver's length to newSize,
	 * add fill the gaps with 0. If newSize less than old length, change receiver's
	 * length to newSize, but do not tirm receiver's capacity.
	 * If newSize less than or equal 0, then method do the things like clean().
	 * 
	 * if newSize bigger than receiver's capacity, method will allocate memory.
	 * 
	 * @param newSize new length.
	 * @return receiver after resizing.
	 * */
	BasicBuffer& resize(e_int newSize) {
		if (newSize <= 0) {
			delete [] m_buffer;
			m_capacity = 16;
			m_length   = 0;
			m_buffer   = new E[m_capacity];
			return *this;
		}
		if (newSize > m_capacity) {
			_updateCapacity(newSize);
			for (e_int i=m_length; i<newSize; i++) {
				m_buffer[i] = 0;
			}
			m_length = newSize;
			return *this;
		} else {
			if (newSize > m_length) {
				for (e_int i=m_length; i<newSize; i++) {
					m_buffer[i] = 0;
				}
				m_length = newSize;
				return *this;
			} else {
				m_length = newSize;
				return *this;
			}
 		}
	}
	
private:
	e_void _init(e_int capa) {
		m_capacity = capa;
		m_buffer = new E[m_capacity];
		m_length = 0;
	}
	
	e_void _append(E* carr, e_int len, e_int start, e_int end) {
		e_int addLen    = end - start;
		e_int resultLen = len + m_length;

		if (resultLen>m_capacity && resultLen<m_capacity*2)	{
			_updateCapacity(m_capacity * 2);
		} else if (resultLen >= m_capacity*2) {
			_updateCapacity(resultLen + 1);
		}
		
		if (addLen == 1) {
			m_buffer[m_length] = carr[start];
			m_length++;
		} else {
			//memmove_s(&(m_buffer[m_length]), addLen*sizeof(E), &carr[start], addLen*sizeof(E));
			e_memcopy(&(m_buffer[m_length]), &carr[start], addLen*sizeof(E));
			m_length = resultLen;
		}
	}
	
	e_void _updateCapacity(e_int newCapa) {
		m_capacity = newCapa;
		E* newBuf  = new E[m_capacity];	
		
		//memmove_s(newBuf, m_length*sizeof(E), m_buffer, m_length*sizeof(E));
		e_memcopy(newBuf, m_buffer, m_length*sizeof(E));
		delete [] m_buffer;
		m_buffer = newBuf;
	}
	
	e_void _insert(E* carr, e_int len, e_int offset) {
		e_int resultLen = m_length + len;
		
		if (resultLen>m_capacity && resultLen<m_capacity*2) {
			_updateCapacity(m_capacity * 2);
		} else if (resultLen >= m_capacity*2) {
			_updateCapacity(resultLen + 1);
		}

		//memmove_s(&m_buffer[offset+len], (m_length-offset)*sizeof(E), &m_buffer[offset], (m_length-offset)*sizeof(E));
		e_memmove(&m_buffer[offset+len], &m_buffer[offset], (m_length-offset)*sizeof(E));
		//memmove_s(&m_buffer[offset], len*sizeof(E), carr, len*sizeof(E));
		e_memcopy(&m_buffer[offset], carr, len*sizeof(E));
		m_length = resultLen;
	}
	
	e_void _remove(e_int resultLen, e_int moveLen, e_int start, e_int end) {
		if (resultLen == 0) {
			m_length = 0;
		} else {
			e_memmove(&m_buffer[start], &m_buffer[end], moveLen*sizeof(E));
			//memmove_s(&m_buffer[start], moveLen*sizeof(E), &m_buffer[end], moveLen*sizeof(E));
			m_length = resultLen;
		}
	}
	
	e_bool equals(e_uint num1, e_uint num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_int num1, e_int num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_ushort num1, e_ushort num2) const  {
		return num1 == num2;
	}
	
	e_bool equals(e_short num1, e_short num2) const  {
		return num1 == num2;
	}
	
	e_bool equals(e_ubyte num1, e_ubyte num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_byte num1, e_byte num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_ulong num1, e_ulong num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_long num1, e_long num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_char num1, e_char num2) const {
		return num1 == num2;
	}
	
	e_bool equals(e_bool truth1, e_bool truth2) const {
		return truth1 == truth2;
	}
	
	e_bool equals(e_float f1, e_float f2) const {
		return Float::equals(f1, f2);
	}
	
	e_bool equals(e_double d1, e_double d2) const {
		return Double::equals(d1, d2);
	}
	
	e_int compare(e_uint num1, e_uint num2) const {
		return UInteger::compare(num1, num2);
	}
	
	e_int compare(e_int num1, e_int num2) const {
		return Integer::compare(num1, num2);
	}
	
	e_int compare(e_ushort num1, e_ushort num2) const {
		return UInteger::compare(num1, num2);
	}
	
	e_int compare(e_short num1, e_short num2) const {
		return Integer::compare(num1, num2);
	}
	
	e_int compare(e_ubyte num1, e_ubyte num2) const {
		return Integer::compare(num1, num2);
	}
	
	e_int compare(e_byte num1, e_byte num2) const {
		return Integer::compare(num1, num2);
	}
	
	e_int compare(e_ulong num1, e_ulong num2) const {
		return ULong::compare(num1, num2);
	}
	
	e_int compare(e_long num1, e_long num2) const {
		return Long::compare(num1, num2);
	}
	
	
	e_int compare(e_char num1, e_char num2) const {
		return num1 - num2;
	}
	
	e_int compare(e_bool truth1, e_bool truth2) const {
		return truth1==truth2 ? 0 : (truth1?1:-1);
	}
	
	
	e_int compare(e_float f1, e_float f2) const {
		return Float::compare(f1, f2);
	}
	
	
	e_int compare(e_double d1, e_double d2) const {
		return Double::compare(d1, d2);
	}
	
	String toString(e_byte num) const {
		return Integer::toString(num);
	}
	
	String toString(e_ubyte num) const {
		return Integer::toString(num);
	}
	
	String toString(e_ushort num) const {
		return UInteger::toString(num);
	}
	
	String toString(e_short num) const {
		return Integer::toString(num);
	}
	
	String toString(e_uint num) const {
		return UInteger::toString(num);
	}
	
	String toString(e_int num) const {
		return Integer::toString(num);
	}
	
	String toString(e_long num) const {
		return Long::toString(num);
	}
	
	String toString(e_ulong num) const {
		return ULong::toString(num);
	}
	
	String toString(e_bool b) const {
		return Bool::toString(b);
	}
	
	String toString(e_char c) const {
		return Character::toString(c);
	}
	
	String toString(e_float f) const {
		return Float::toString(f);
	}
	
	String toString(e_double d) const {
		return Double::toString(d);
	}
}; // BasicBuffer

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_bool.
 * */
typedef BasicBuffer<e_bool> BoolBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_char.
 * */
typedef BasicBuffer<e_char> CharBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_byte.
 * */
typedef BasicBuffer<e_byte> ByteBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_ubyte.
 * */
typedef BasicBuffer<e_ubyte> UByteBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_short.
 * */
typedef BasicBuffer<e_short> ShortBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_ushort.
 * */
typedef BasicBuffer<e_ushort> UShortBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_int.
 * */
typedef BasicBuffer<e_int> IntBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_uint.
 * */
typedef BasicBuffer<e_uint> UIntBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_long.
 * */
typedef BasicBuffer<e_long> LongBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_ulong.
 * */
typedef BasicBuffer<e_ulong> ULongBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_float.
 * */
typedef BasicBuffer<e_float> FloatBuffer;

/**
 * \brief BasicBuffer template instances.
 * 
 * Basic types buffer template instance for e_double.
 * */
typedef BasicBuffer<e_double> DoubleBuffer;

} // Easy

#endif // _EASY_BASICBUFFER_H_

