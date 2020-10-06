
/*
 * File name: UInteger.hpp 
 *
 * Date/Time:
 *		start: 2019年10月03日 18时17分07秒 周4
 *		end:
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_UINTEGER_H_
#define LEC_UINTEGER_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"
#include "Math.hpp"
#include "Integer.hpp"

namespace Easy {
	
/**
	\class UInteger
	
	\brief 无符号 32 位整数包装类.
	
	@since 1.0
*/
class UInteger {
public:

	/**
		\brief 最小 32 位无符号整数值.
	*/
	static const e_uint MIN = 0;

	/**
		\brief 最大 32 位无符号整数值.
	*/
	static const e_uint MAX = UINT32_MAX;

	/**
		\brief 构造函数.
		
		@param value 无符号 32 位整数值, 默认为 0.
	*/
	UInteger(e_uint value=0):m_value(value){}
	
	/**
		\brief 获取值.
		
		@return 接受者的值.
	*/
	inline e_uint getValue() const {return m_value;}
	
	/**
		\brief 等于函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
	*/
	inline e_bool equals(const UInteger& other) const {
		return equals(m_value, other.m_value);
	}

	/**
		\brief 比较函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值大于 other 的值, 返回 1; 等于返回 0; 小于返回 -1.
	*/		
	inline e_int compare(const UInteger& other) const {
		return compare(m_value, other.m_value);
	}
	
	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	inline e_int hashCode() const {return UInteger::hashCode(m_value);}
	
	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	inline String toString() const {return System::move(UInteger::toString(m_value));}

	/**
		\brief 等于函数.
		
		@param i1 无符号 32 位整数值.
		@param i2 无符号 32 位整数值.
		@return i1 == i2
	*/
	inline static e_bool equals(e_uint i1, e_uint i2) {return i1==i2;}
	
	/**
		\brief 比较函数.
		
		@param i1 无符号 32 位整数值.
		@param i2 无符号 32 位整数值.
		@return i1==i2 ? 0 : (i1>i2 ? 1 : -1)
	*/
	inline static e_int compare(e_uint i1, e_uint i2) {return i1==i2?0:(i1>i2?1:-1);}

	/**
	 * \brief 哈希函数.
	 * 
	 * @param num 求哈希值的数.
	 * @return num 的哈希值.
	*/
	inline static e_int hashCode(e_uint num) {return Math::abs((e_int)num);}
	
	/**
		\brief 到字符串函数.
		
		@param num 无符号 32 位整数值.
		@return num 的字符串表示.
	*/
	static String toString(e_uint i);
	
	static String toString(e_uint i, e_int radix);
	
	/**
		\brief 到16进制字符串.
		
		@param num 无符号 32 位整数值.
		@return num 的16进制字符串表示.
	*/
	static String toHexString(e_uint i);
	
	/**
		\brief 到8进制字符串.
		
		@param num 无符号 32 位整数值.
		@return num 的8进制字符串表示.
	*/
	static String toOctString(e_uint i);
	
	/**
		\brief 到2进制字符串.
		 
		@param num 无符号 32 位整数值.
		@return num 的2进制字符串表示.
	*/
	static String toBinaryString(e_uint i);
	
	/**
		\brief 字符串到 e_uint 值.
		
		@param str 待解析的字符串.
		@return str 对应的 e_uint 值.
		@throw 如果 str 不满足 [0-9]+, 抛出 NumberFormatException.
		@throw 如果 str 不在 UInteger::MAX 和 UInteger::MIN 之间, 抛出 NumberOverflowException. 
	*/
	static e_uint parse(const String& s);
	static e_uint parseHex(const String& s);
	static e_uint parseOct(const String& s);
	static e_uint parseBinary(const String& s);
	
	// TODO: XXX
	static e_uint parse(const String& s, e_int radix);
private:
	e_uint m_value;
}; // UInteger

/**
	\class ULong
	
	\brief 无符号 64 位整数包装类.
	@since 1.0
*/
class ULong {
	
public:

	/**
		\brief 最小 64 位无符号整数值.
	*/
	static const e_ulong MIN = 0;
	
	/**
		\brief 最大 64 位无符号整数值.
	*/
	static const e_ulong MAX = UINT64_MAX;

	/**
		\brief 构造函数.
		
		@param value  64 位无符号整数值, 默认为 0.
	*/
	inline ULong(e_ulong value=0): m_value(value) {}
	
	/**
		\brief 获取值.
		
		@return 接受者的值.
	*/
	inline e_ulong getValue() const {return m_value;}
	
	/**
		\brief 等于函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
	*/
	inline e_bool equals(const ULong& other) const {
		return equals(m_value, other.m_value);
	}
	
	/**
		\brief 比较函数.
		
		@param other
		@return 如果接受者的值大于 item 的值, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	inline e_int compare(const ULong& other)  const {
		return compare(m_value, other.m_value);
	}
	
	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	inline e_int hashCode() const {return ULong::hashCode(m_value);}

	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	inline String toString() const {return System::move(ULong::toString(m_value));}
	
	/**
		\brief 等于函数.
		
		@param l1  64 位无符号整数值.
		@param l2  64 位无符号整数值.
		@return l1 == l2
	*/
	inline static e_bool equals(e_ulong l1, e_ulong l2) {return l1==l2;}
	
	/**
		\brief 比较函数.
		
		@param l1  64 位无符号整数值.
		@param l2  64 位无符号整数值.
		@return l1==l2 ? 0 : (l1>l2 ? 1 : -1)
	*/
	inline static e_int compare(e_ulong l1, e_ulong l2) {return l1==l2?0:(l1>l2?1:-1);}

	/**
	 * \brief 哈希函数.
	 * 
	 * @param l 求哈希值的数.
	 * @return l 的哈希值.
	*/
	inline static e_int hashCode(e_ulong l) {return Math::abs((e_int)l);}
	
	/**
		\brief 到字符串函数.
		 
		@param l  64 位无符号整数值.
		@return l 的字符串表示.
	*/
	static String toString(e_ulong l);
	
	static String toString(e_ulong l, e_int radix);
	
	/**
		\brief 到16进制字符串.
		
		@param l  64 位无符号整数值.
		@return l 的16进制字符串表示.
	*/
	static String toHexString(e_ulong l);
	
	/**
		\brief 到8进制字符串.
		
		@param l  64 位无符号整数值.
		@return l 的8进制字符串表示.
	*/
	static String toOctString(e_ulong l);
	
	/**
		\brief 到2进制字符串.
		
		@param l  64 位无符号整数值.
		@return l 的2进制字符串表示.
	*/
	static String toBinaryString(e_ulong l);
	
	/**
		\brief 字符串到 e_ulong 值.
		
		@param str 待解析的字符串.
		@return str 对应的 e_ulong 值.
		@throw 如果 str 不满足 [0-9]+, 抛出 NumberFormatException.
		@throw 如果 str 不在 ULong::MAX 和 ULong::MIN 之间, 抛出 NumberOverflowException. 
	*/
	static e_ulong parse(const String& s);
	static e_ulong parseHex(const String& s);
	static e_ulong parseOct(const String& s);
	static e_ulong parseBinary(const String& s);
	
	static e_ulong parse(const String& str, e_int radix);
private:
	e_ulong m_value;
}; // ULong

/**
 * \class UByte
 * 
 * \brief An e_ubyte wrapper.
 * 
 * @author Tes Alone
 * @since 1.0
*/

class UByte {
private:
	e_ubyte m_value;
public:
	/**
	 * \brief Min e_ubyte value. 
	*/
	const static e_ubyte MIN = 0;
	
	/**
	 * \brief Max e_ubyte value.
	*/
	const static e_ubyte MAX = 255;
	
	/**
	 * \brief ctor.
	 * 
	 * @param value default 0.
	*/
	UByte(e_ubyte value=0) : m_value(value) {}
	
	/**
	 * \brief Get value.
	 * 
	 * @return receiver's value.
	*/
	inline e_ubyte getValue() const {return m_value;}

	/**
	 * \brief Check equality.
	 * 
	 * @param other the compared object with receiver.
	 * @return receiver's value == other's value.
	*/
	inline e_bool equals(const UByte& other) const {return m_value==other.m_value;}

	/**
	 * \brief Check order.
	 * 
	 * @param other the compared object with receiver.
	 * @return if receiver's value bigger than other's, return 1; both equals return 0; otherwise return -1.
	*/
	inline e_int  compare(const UByte& other) const {return m_value==other.m_value?0:(m_value>other.m_value?1:-1);}
	
	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	inline e_int hashCode() const {return Integer::hashCode(m_value);}
	
	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {return System::move(Integer::toString(m_value));}
}; // UByte

/**
 * \class UShort
 * 
 * \brief 有符号 16 位整数包装类.
*/
class UShort {
private:
	e_ushort m_value;
public:
	/**
		\brief 最大 16 位无符号整数值.
	*/
	const static e_ushort MIN = 0;
	
	/**
		\brief 最小 16 位无符号整数值.
	*/
	const static e_ushort MAX = UINT16_MAX;
	
	/**
		\brief 构造函数.
			
		@param value 无符号 16 位整数值, 默认为 0.
	*/
	inline UShort(e_ushort value=0) : m_value(value) {}
	
	/**
		\brief 获取值.
		
		@return 接受者的值.
	*/
	inline e_ushort getValue() const {return m_value;}
	
	/**
		\brief 等于函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
	*/
	inline e_bool equals(const UShort& other) const {return m_value==other.m_value;}
	
	/**
		\brief 比较函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值大于 other 的值, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	inline e_int  compare(const UShort& other) const {return m_value==other.m_value?0:(m_value>other.m_value?1:-1);}
	
	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	inline e_int hashCode() const {return Integer::hashCode(m_value);}
	
	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	inline String toString() const {return System::move(UInteger::toString(m_value));}
}; // UShort

} // namespace Easy


#endif // LEC_UINTEGER_H_