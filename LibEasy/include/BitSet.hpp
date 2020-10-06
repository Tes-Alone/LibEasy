/*
 * 文件名: BitSet.hpp 
 *
 * 日期/时间:
 *		开始: 2020年06月26日 22:50:37 周五
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_BITSET_H_
#define _EASY_BITSET_H_


#include "BaseTypes.hpp"
#include "String.hpp"
#include "BasicArray.hpp"
#include "Exception.hpp"
#include "Math.hpp"
#include "Bytes.hpp"

namespace Easy {

/**
 * \class BitSet
 * 
 * \brief A bits represent of the set in math.
 * 
 * @author Tes Alone
 * @since 1.0
 * */

class BitSet {
private:
	ULongArray _bits;
	e_int 	   _tail;
	e_int 	   _length;
public:
	/**
	 * \brief ctor.
	 * 
	 * If len is less than 0, len will be set to 0.
	 * 
	 * @param len bits length, can be 0, default 0.
	 * */
	BitSet(e_int len=0) {
		if (len < 0) len = 0;
		_tail = len % 64;
		_bits = ULongArray{(len/64) + ((_tail==0)?0:1)};
		_length = len;
	}
	
	/**
	 * \brief copy ctor.
	 * 
	 * @param copy .
	 * */
	BitSet(const BitSet& copy) {
		_bits = copy._bits;
		_tail = copy._tail;
		_length = copy._length;
	}
	
	/**
	 * \brief copy assignment.
	 * 
	 * @param copy .
	 * */
	BitSet& operator=(const BitSet& copy) {
		_bits = copy._bits;
		_tail = copy._tail;
		_length = copy._length;
		return *this;
	}
	
	/**
	 * \brief move ctor.
	 * 
	 * @param move .
	 * */
	BitSet(BitSet&& move) {
		_bits = System::move(move._bits);
		_tail = move._tail;
		_length = move._length;
		move._tail   = 0;
		move._length = 0;
	}
	
	/**
	 * \brief move assignment.
	 * 
	 * @param move .
	 * */
	BitSet& operator=(BitSet&& move) {
		_bits = System::move(move._bits);
		_tail = move._tail;
		_length = move._length;
		move._tail   = 0;
		move._length = 0;
		return *this;
	}
	
	/**
	 * \brief Check equality.
	 * 
	 * The method compare receiver and other, if receiver's bits equals 
	 * other's, one by one, return true, otherwise retunr false.
	 * 
	 * @param other .
	 * @return  compared result.
	 * */
	e_bool equals(const BitSet& other) const {
		if (_length == other._length) {
			e_int len = _bits.length() - 1;
			for (e_int i=0; i<len; i++) {
				if (_bits[i] != other._bits[i])
				return false;
			}
			e_ulong mask = 0;
			for (e_int i=0; i<_tail; i++) {
				mask |= (((e_ulong)0x0000000000000001) << i);
			}
			e_ulong l1 = _bits[len] & mask;
			e_ulong l2 = other._bits[len] & mask;
			return l1 == l2;
		}
		return false;
	}
	
	/**
	 * \brief Check order.
	 * 
	 * Method just compare receiver's length and other's length.
	 * If receiver's length is bigger return 1, both equals return 0, otherwise return -1;
	 * 
	 * @param other .
	 * @return compared result.
	 * */
	inline e_int compare(const BitSet& other) const {
		return Integer::compare(_length, other._length);
	}
	
	/**
	 * \brief Get hash code.
	 * 
	 * Get receiver`s hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	inline e_int hashCode() const {
		return Math::abs(_bits.hashCode() + Integer::hashCode(_tail + _length));
	}
	
	/**
	 * \brief To string represent．
	 * 
	 * @returns receiver`s string represent.
	*/
	String toString() const {
		StringBuffer sb;
		e_int len = _bits.length() - 1;
		for (e_int i=0; i<len; i++) {
			sb.append(String::format(L"%064s", ULong::toBinaryString(_bits[i])).reverse());
		}
		String tail = String::format(L"%064s", ULong::toBinaryString(_bits[len]));
		if (_tail != 0) {
			sb.append(tail.substring(64-_tail, 64).reverse());
		} else {
			sb.append(tail.reverse());
		}
		return System::move(sb.toString());
	}
	
	/**
	 * \brief Check containity.
	 * 
	 * If receiver contains or equals other, return true, otherwise return false.
	 * 
	 * @param other .
	 * @return result.
	 * */
	e_bool isSuperOf(const BitSet& other) const {
		if (this == &other) return true;
		if (_length == other._length) {
			e_int len = _bits.length() - 1;
			for (e_int i=0; i<len; i++) {
				if ((_bits[i]&other._bits[i])!=other._bits[i]) {
					return false;
				}
			}
			e_ulong mask = 0;
			for (e_int i=0; i<_tail; i++) {
				mask |= (((e_ulong)0x0000000000000001) << i);
			}
			e_ulong l1 = _bits[len] & mask;
			e_ulong l2 = other._bits[len] & mask;
			return (l1&l2) == l2;
		}
		return false;
	}
	
	/**
	 * \brief Check containity.
	 * 
	 * If receiver contains but not equals other, return true, otherwise return false.
	 * 
	 * @param other .
	 * @return result.
	 * */
	e_bool isProperSuperOf(const BitSet& other) const {
		if (this == &other) return false;
		return this->isSuperOf(other) && (!this->equals(other));
	}
	
	/**
	 * \brief Check containity.
	 * 
	 * If other contains or equals receiver, return true, otherwise return false.
	 * 
	 * @param other .
	 * @return result.
	 * */
	e_bool isSubOf(const BitSet& other) const {
		return other.isSuperOf(*this);
	}
	
	/**
	 * \brief Check containity.
	 * 
	 * If other contains but not equals receiver, return true, otherwise return false.
	 * 
	 * @param other .
	 * @return result.
	 * */
	e_bool isProperSubOf(const BitSet& other) const {
		return other.isProperSuperOf(*this);
	}
	
	/**
	 * \brief Union two sets.
	 * 
	 * The result is in receiver, other not change.
	 * 
	 * @param other .
	 * @return receiver after union.
	 * @throw if receiver's length not equals other's, throws IllegalArgumentException.
	 * */
	BitSet& unions(const BitSet& other) {
		if (_length != other._length) {
			throw IllegalArgumentException(__func__, __LINE__);
		}
		if (this == &other) return *this;
		e_int time = _bits.length();
		for (e_int i=0; i<time; i++) {
			_bits[i] |= other._bits[i];
		}
		return *this;
	}
	
	/**
	 * \brief Join two sets.
	 * 
	 * The result is in receiver, other not change.
	 * 
	 * @param other .
	 * @return receiver after joining.
	 * @throw if receiver's length not equals other's, throws IllegalArgumentException.
	 * */
	BitSet& joins(const BitSet& other) {
		if (_length != other._length) {
			throw IllegalArgumentException(__func__, __LINE__);
		}
		if (this == &other) return *this;
		e_int time = _bits.length();
		for (e_int i=0; i<time; i++) {
			_bits[i] &= other._bits[i];
		}
		return *this;
	}
	
	/**
	 * \brief Complement set.
	 * 
	 * @return receiver after complement.
	 * */
	BitSet& complements() {
		e_int time = _bits.length();
		for (e_int i=0; i<time; i++) {
			_bits[i] = ~_bits[i];
		}
		return *this;
	}
	
	/**
	 * \brief Get difference of two sets.
	 * 
	 * The methon use receiver to differ other.
	 * The result is in receiver, other not change.
	 * 
	 * @param other .
	 * @return receiver after differing.
	 * @throw if receiver's length not equals other's, throws IllegalArgumentException.
	 * */
	BitSet& differs(const BitSet& other) {
		if (_length != other._length) {
			throw IllegalArgumentException(__func__, __LINE__);
		}
		if (this == &other) {
			return this->cleanAll();
		}
		e_int time = _bits.length();
		for (e_int i=0; i<time; i++) {
			_bits[i] |= other._bits[i];
			_bits[i] ^= other._bits[i];
		}
		return *this;
	}
	
	/**
	 * \brief Get symmetry difference of two sets.
	 * 
	 * The result is in receiver, other not change.
	 * 
	 * @param other .
	 * @return receiver after differing.
	 * @throw if receiver's length not equals other's, throws IllegalArgumentException.
	 * */
	BitSet& symDiffers(const BitSet& other) {
		if (_length != other._length) {
			throw IllegalArgumentException(__func__, __LINE__);
		}
		if (this == &other) {
			return this->cleanAll();
		}
		e_int time = _bits.length();
		BitSet tmp{*this};
		for (e_int i=0; i<time; i++) {
			tmp._bits[i] &= other._bits[i];
			_bits[i] |= other._bits[i];
			_bits[i] |= tmp._bits[i];
			_bits[i] ^= tmp._bits[i];
		}
		return *this;
	}
	
	/**
	 * \brief Set i-th bit.
	 * 
	 * Set the i-th bit in receiver.
	 * 
	 * @param i .
	 * @return receiver after setting.
	 * @throw if i<0 or i>= BitSet::length(), throws IndexOutOfBoundsException.
	 * */
	BitSet& setBit(e_int i) {
		if (i<0 || i>=_length) {
			throw IndexOutOfBoundsException(__func__, __LINE__, L"BitSet", _length, i);
		}
		e_int slot   = i / 64;
		e_int offset = i % 64;
		_bits[slot] |= (((e_ulong)0x0000000000000001) << offset);		
		return *this;
	}
	
	/**
	 * \brief Clean i-th bit.
	 * 
	 * Clean the i-th bit in receiver.
	 * 
	 * @param i .
	 * @return receiver after cleaning.
	 * @throw if i<0 or i>= BitSet::length(), throws IndexOutOfBoundsException.
	 * */
	BitSet& cleanBit(e_int i) {
		if (i<0 || i>=_length) {
			throw IndexOutOfBoundsException(__func__, __LINE__, L"BitSet", _length, i);
		}
		e_int slot   = i / 64;
		e_int offset = i % 64;
		_bits[slot] &= (~(((e_ulong)0x0000000000000001) << offset));
		return *this;
	}
	
	/**
	 * \brief Check whether bit set.
	 * 
	 * Check whether the i-th bit in receiver is set, if so return true, otherwise return false.
	 * 
	 * @param i .
	 * @return result.
	 * */
	e_bool isSet(e_int i) const {
		if (i<0 || i>=_length) {
			return false;
		}
		e_int slot   = i / 64;
		e_int offset = i % 64;
		return _bits[slot]&(((e_ulong)0x0000000000000001) << offset);
	}
	
	/**
	 * \brief Clean all bits.
	 * 
	 * @return receiver after cleaning.
	 * */
	BitSet& cleanAll() {
		e_int len = _bits.length();
		for (e_int i=0; i<len; i++) {
			_bits[i] &= 0;
		}
		return *this;
	}
	
	/**
	 * \brief Set all bits.
	 * 
	 * @return receiver after setting.
	 * */
	BitSet& setAll() {
		e_int len = _bits.length();
		for (e_int i=0; i<len; i++) {
			_bits[i] |= ((e_ulong)0xFFFFFFFFFFFFFFFF);
		}
		return *this;
	}
	
	/**
	 * \brief Get bits length.
	 * 
	 * Get length of receiver, by bit.
	 * 
	 * @return length of receiver.
	*/
	inline e_int length() const {
		return _length;
	}
	
	/**
	 * \brief Get data.
	 * 
	 * Get data of receiver, the result independs with receiver.
	 * 
	 * @return the data of receiver.
	 * */
	UByteArray toUBytes() const {
		UByteBuffer result;
		e_int len = _bits.length()-1;
		for (e_int i=0; i<len; i++) {
			result.append(Bytes::toUBytes(_bits[i]));
		}
		if (_tail != 0) {
			e_ulong mask = 0;
			for (e_int i=0; i<_tail; i++) {
				mask |= (((e_ulong)0x0000000000000001) << i);
			}
			result.append(Bytes::toUBytes(_bits[len] & mask));
		} else {
			result.append(Bytes::toUBytes(_bits[len]));
		}
		return result.subArray();
	}
}; // BitSet

} // namespace Easy



#endif // _EASY_BITSET_H_
