
#ifndef _EASY_BASICARRAY_TCC_H_
#define _EASY_BASICARRAY_TCC_H_

#include "BasicArray.hpp"
#include "StringBuffer.hpp"
#include "System.hpp"
#include "UInteger.hpp"
#include "String.hpp"
#include "Bytes.hpp"

namespace Easy {
	
template<class E>
inline e_void BasicArray<E>::setAt(e_int index, E item) {
	if (index<0 || index>=m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, index};
	m_arr[m_length++] = item;
}

template<class E>
inline E& BasicArray<E>::getAt(e_int index) const {
	if (index<0 || index>=m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, index};
	return m_arr[index];
}

template<class E>
e_int BasicArray<E>::lastIndexOf(E item, e_int offset) const {
	if (offset == -1) 
		offset = m_length - 1;
	if (offset<0 || offset>=m_length) 
		return -1;

	for (e_int i=offset; i>=0; i--)
	{
		if (equals(item, m_arr[i]))
			return i;
	}

	return -1;
}

template<class E>
BasicArray<E> BasicArray<E>::subArray(e_int start, e_int end) const {
	if (m_length == 0) return BasicArray<E>{0};	
	if (end == -1)	end = m_length;
	if (start<0 || start>=m_length) 
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, start};
	if (end<0 || end>m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
	BasicArray<E> sub{end-start};
	for (e_int i=start; i<end; i++) {
		sub[i-start] = m_arr[i];
	}
	return System::move(sub);
}

template<class E>
e_void BasicArray<E>::replace(const BasicArray<E>& arr, e_int start, e_int end) {
	if (end == -1)	end = m_length;
	if (start<0 || start>=m_length) 
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, start};
	if (end<0 || end>m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
	e_int replaceLen = end - start;
	if (arr.length() != replaceLen)
		throw IllegalArgumentException{__func__, __LINE__};
	for (e_int i=start; i<end; i++) {
		m_arr[i] = arr[i-start];
	}
}

template<class E>
BasicArray<e_ubyte> BasicArray<E>::toUBytes() const {
	UByteArray result{m_length*sizeof(E)};
	for (e_int i=0; i<m_length; i++) {
		result.replace(Bytes::toUBytes(m_arr[i]), i*sizeof(E), (i*sizeof(E))+sizeof(E));
	}
	return result;
}

template<class E>
e_void BasicArray<E>::sort(e_int start, e_int end) {
	if (m_length == 0) 
		return;

	if (end == -1)	
		end = m_length;
	if (start<0 || start>=m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, start};
	if (end<0 || end>m_length) 
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, end};
	if (start > end) 
		throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) 
		end++;

	_quickSort(m_arr, start, end-1);
}
	
template<class E>
e_int BasicArray<E>::search(E item, e_int start, e_int end) const	{
	if (m_length == 0) 
		return -1;

	if (end == -1) 
		end = m_length;

	if (start<0 || start>=m_length) 
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, start};
	if (end<0 || end>m_length) 
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"BasicArray"}, m_length, end};
	if (start > end) 
		throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) 
		end++;

	return _binarySearch(item, start, end-1);
}
	
template<class E>
String BasicArray<E>::toString() const {
	StringBuffer sb{};
	sb.append(String{L"["});
	for (e_int i=0; i<m_length; i++) {
		sb.append(toString(m_arr[i]));
		sb.append(String{L","});
	}
	if (sb.length() > 1) {
		return System::move(sb.removeTail(sb.length()-1).append(String{L"]"}).toString());
	}
	return System::move(sb.append(String{L"]"}).toString());
}

template<class E>
e_int BasicArray<E>::hashCode() const {
	return Long::hashCode(reinterpret_cast<e_long>(this));
}

template<class E>
e_bool BasicArray<E>::equals(e_uint num1, e_uint num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_int num1, e_int num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_ushort num1, e_ushort num2) const  {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_short num1, e_short num2) const  {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_byte num1, e_byte num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_ubyte num1, e_ubyte num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_long num1, e_long num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_ulong num1, e_ulong num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_char num1, e_char num2) const {
	return num1 == num2;
}

template<class E>
e_bool BasicArray<E>::equals(e_bool truth1, e_bool truth2) const {
	return truth1 == truth2;
}

template<class E>
e_bool BasicArray<E>::equals(e_float f1, e_float f2) const {
	return Float::equals(f1, f2);
}

template<class E>
e_bool BasicArray<E>::equals(e_double d1, e_double d2) const {
	return Double::equals(d1, d2);
}

template<class E>
e_int BasicArray<E>::compare(e_uint num1, e_uint num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_int num1, e_int num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_ushort num1, e_ushort num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_short num1, e_short num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_ubyte num1, e_ubyte num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_byte num1, e_byte num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_ulong num1, e_ulong num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_long num1, e_long num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_char num1, e_char num2) const {
	return num1==num2 ? 0 : (num1>num2 ? 1 : -1);
}

template<class E>
e_int BasicArray<E>::compare(e_bool truth1, e_bool truth2) const {
	return truth1==truth2 ? 0 : (truth1?1:-1);
}

template<class E>
e_int BasicArray<E>::compare(e_float f1, e_float f2) const {
	return Float::compare(f1, f2);
}

template<class E>
e_int BasicArray<E>::compare(e_double d1, e_double d2) const {
	return Double::compare(d1, d2);
}

template<class E>
String BasicArray<E>::toString(e_byte num) const {
	return Integer::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_ubyte num) const {
	return Integer::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_ushort num) const {
	return UInteger::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_short num) const {
	return Integer::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_uint num) const {
	return UInteger::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_int num) const {
	return Integer::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_ulong num) const {
	return ULong::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_long num) const {
	return Long::toString(num);
}

template<class E>
String BasicArray<E>::toString(e_bool b) const {
	return Bool::toString(b);
}

template<class E>
String BasicArray<E>::toString(e_char c) const {
	return Character::toString(c);
}

template<class E>
String BasicArray<E>::toString(e_float f) const {
	return Float::toString(f);
}

template<class E>
String BasicArray<E>::toString(e_double d) const {
	return Double::toString(d);
}

template<class E>
e_void BasicArray<E>::_quickSort(E* its, e_int start, e_int end)
{
	if ((end-start) <= 0) {
		return;
	}

	e_int center = (start+end) / 2;

	E tmp       = its[end];
	its[end]    = its[center];
	its[center] = tmp;

	e_int left  = start-1;
	e_int right = end;
	E pivot     = its[end];

	do
	{ // Move the bounds inward until they meet
		while ((left<end) && compare(its[++left],pivot) < 0); // Move l right and
		while ((right>start) && left<right && (compare(pivot,its[--right])<0)); // r left

		E tmp      = its[left];
		its[left]  = its[right];
		its[right] = tmp;// Swap out-of-place values

	} while (left < right); // Stop when they cross

	tmp       = its[end];
	its[end]  = its[left];
	its[left] = tmp;

	_quickSort(its, start, left-1);
	_quickSort(its, left+1, end);
}

template<class E>
e_int BasicArray<E>::_binarySearch(E item, e_int start, e_int end)  const
{
	if ((end - start) < 0) return -1;
	e_int center = (end + start) / 2;
	e_int state = compare(m_arr[center], item);
	if (state == 0) return center;					//注意这里的+1
	if (state < 0) return _binarySearch(item, center+1, end);
	if (state > 0) return _binarySearch(item, start, center-1);//注意这里的 -1

	return -1; //never arrive here
}
	
} // Easy

#endif // _EASY_BASICARRAY_TCC_H_