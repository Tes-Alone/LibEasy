
/*
 * File name: Integer.hpp 
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

#ifndef LEC_INTEGER_H_
#define LEC_INTEGER_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"
#include "Math.hpp"

namespace Easy {
	
/**
	\class Integer
	
	\brief Signed 32-bits integer wrapper.
	
	@since 1.0
*/
class Integer {
	public:
		/**
			\brief Max signed 32-bits integer.
		*/
		static const e_int MAX = INT32_MAX;
		
		/**
			\brief Min signed 32-bits integer.
		*/
		static const e_int MIN = INT32_MIN;

		/**
			\brief Ctor.
			
			@param value default 0.
		*/
		Integer(e_int value=0):m_value(value){}
		
		/**
			\brief Get wrapped value.
			
			@return receiver's value.
		*/
		inline e_int getValue() const {return m_value;}
		
		/**
		 * \brief Check equality.
		 * 
		 * @param other the compared object with receiver.
		 * @return receiver's value == other's value.
		*/
		inline e_bool equals(const Integer& other) const {
			return equals(m_value, other.m_value);
		}

		/**
		 * \brief Check order.
		 * 
		 * @param other the compared object with receiver.
		 * @return if receiver's value bigger than other's, return 1; both equals return 0; otherwise return -1.
		*/	
		inline e_int compare(const Integer& other) const {
			return compare(m_value, other.m_value);
		}
		
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

		/**
		 * \brief Check equality.
		 * 
		 * @param i1 .
		 * @param i2 .
		 * @return i1 == i2.
		*/
		inline static e_bool equals(e_int i1, e_int i2) {return i1==i2;}
		
		/**
		 * \brief Check order.
		 * 
		 * @param i1 .
		 * @param i2 .
		 * @return i1==i2?0:(i1>i2?1:-1).
		*/
		inline static e_int compare(e_int i1, e_int i2) {return i1==i2?0:(i1>i2?1:-1);}

		/**
		 * \brief Get hash code.
		 * 
		 * @param i .
		 * @return i's hash code.
		 * */
		inline static e_int hashCode(e_int i) {return Math::abs((e_int)i);}
		
		/**
		 * \brief To string．
		 * 
		 * @param i .
		 * @returns i's string represent, in radix 10.
		*/
		static String toString(e_int i);
		
		/**
		 * \brief To string．
		 * 
		 * @param i .
		 * @param radix can be 2,8,10 or 16, if not these, use radix 10.
		 * @returns i's string represent, in radix.
		*/
		static String toString(e_int i, e_int radix);
		
		/**
		 * \brief To string．
		 * 
		 * @param i .
		 * @returns i's string represent, in radix 16.
		*/
		static String toHexString(e_int i);
		
		/**
		 * \brief To string．
		 * 
		 * @param i .
		 * @returns i's string represent, in radix 8.
		*/
		static String toOctString(e_int i);
		
		/**
		 * \brief To string．
		 * 
		 * @param i .
		 * @returns i's string represent, in radix 2.
		*/
		static String toBinaryString(e_int i);
		
		/**
			\brief Convert string to e_int.
			
			@param s .
			@return the e_int value for which s represent.
			@throw if s not match [-+]?[0-9]+, throw NumberFormatException.
			@throw if s not in between Integer::MAX and  Integer::MIN, throw NumberOverflowException. 
		*/
		static e_int parse(const String& chs);
		
		/**
			\brief Convert string to e_int.
			
			@param s .
			@return the e_int value for which s represent.
			@throw if s not match [-+]?[0-9a-fA-F]+, throw NumberFormatException.
			@throw if s not in between Integer::MAX and  Integer::MIN, throw NumberOverflowException. 
		*/
		static e_int parseHex(const String& chs);
		
		/**
			\brief Convert string to e_int.
			
			@param s .
			@return the e_int value for which s represent.
			@throw if s not match [-+]?[0-7]+, throw NumberFormatException.
			@throw if s not in between Integer::MAX and  Integer::MIN, throw NumberOverflowException. 
		*/
		static e_int parseOct(const String& chs);
		
		/**
			\brief Convert string to e_int.
			
			@param s .
			@return the e_int value for which s represent.
			@throw if s not match [-+]?[01]+, throw NumberFormatException.
			@throw if s not in between Integer::MAX and  Integer::MIN, throw NumberOverflowException. 
		*/
		static e_int parseBinary(const String& chs);
		
		/**
			\brief Convert string to e_int.
			
			@param s .
			@param radix can be 2,8,10,16, if not these, use radix 10.
			@return the e_int value for which s represent.
			@throw if s cannot be parsed by parse(),parseHex(), parseOct(), parseBinary() , throw NumberFormatException.
			@throw if s not in between Integer::MAX and  Integer::MIN, throw NumberOverflowException. 
		*/
		static e_int parse(const String& chs, e_int radix);
	private:
		e_int m_value;
};

/**
	\class Long
	
	\brief 有符号 64 位整数包装类.
	@since 1.0
*/
class Long {
	
	public:
		/**
			\brief 最大 64 位有符号整数值.
		*/
		static const e_long MAX = INT64_MAX;

		/**
			\brief 最小 64 位有符号整数值.
		*/
		static const e_long MIN = INT64_MIN;

		/**
			\brief 构造函数.
			
			@param value  64 位有符号整数值, 默认为 0.
		*/
		inline Long(e_long value=0): m_value(value) {}
		
		/**
			\brief 获取值.
			
			@return 接受者的值.
		*/
		inline e_long getValue() const {return m_value;}
		
		/**
			\brief 等于函数.
			
			@param other 接受者的比较对象.
			@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
		*/
		inline e_bool equals(const Long& other) const {
			return equals(m_value, other.m_value);
		}
		
		/**
			\brief 比较函数.
			
			@param other
			@return 如果接受者的值大于 item 的值, 返回 1; 等于返回 0; 小于返回 -1.
		*/
		inline e_int compare(const Long& other)  const {
			return compare(m_value, other.m_value);
		}
		
		/**
		 * \brief 哈希函数.
		 * 
		 * @return 接受者的哈希值.
		*/
		inline e_int hashCode() const {return Long::hashCode(m_value);}

		/**
		 * \brief 转到字符串．
		 * 
		 * @returns 接受者的字符串表示.
		*/
		inline String toString() const {return System::move(Long::toString(m_value));}
		
		/**
			\brief 等于函数.
			
			@param l1  64 位有符号整数值.
			@param l2  64 位有符号整数值.
			@return l1 == l2
		*/
		inline static e_bool equals(e_long l1, e_long l2) {return l1==l2;}
		
		/**
			\brief 比较函数.
			
			@param l1  64 位有符号整数值.
			@param l2  64 位有符号整数值.
			@return l1==l2 ? 0 : (l1>l2 ? 1 : -1)
		*/
		inline static e_int compare(e_long l1, e_long l2) {return l1==l2?0:(l1>l2?1:-1);}

		/**
		 * \brief 哈希函数.
		 * 
		 * @param l 求哈希值的数.
		 * @return l 的哈希值.
		*/
		inline static e_int hashCode(e_long l) {return Math::abs((e_int)l);}
		
		/**
			\brief 到字符串函数.
			 
			@param l  64 位有符号整数值.
			@return l 的字符串表示.
		*/
		static String toString(e_long l);
		
		static String toString(e_long l, e_int radix);
		
		/**
			\brief 到16进制字符串.
			
			@param l  64 位有符号整数值.
			@return l 的16进制字符串表示.
		*/
		static String toHexString(e_long l);
		
		/**
			\brief 到8进制字符串.
			
			@param l  64 位有符号整数值.
			@return l 的8进制字符串表示.
		*/
		static String toOctString(e_long l);
		
		/**
			\brief 到2进制字符串.
			
			@param l  64 位有符号整数值.
			@return l 的2进制字符串表示.
		*/
		static String toBinaryString(e_long l);
		
		/**
			\brief 字符串到 e_long 值.
			
			@param s 待解析的字符串.
			@return s 对应的 e_long 值.
			@throw 如果 s 不满足 -?[0-9]+, 抛出 NumberFormatException.
			@throw 如果 s 不在 Long::MAX 和 Long::MIN 之间, 抛出 NumberOverflowException. 
		*/
		static e_long parse(const String& s);
		
		static e_long parseHex(const String& s);
		static e_long parseOct(const String& s);
		static e_long parseBinary(const String& s);
		static e_long parse(const String& s, e_int radix);
	private:
		e_long m_value;
}; // Long


/**
 * \class Byte
 * 
 * \brief An e_byte wrapper.
 * 
 * @author Tes Alone
 * @since 1.0
*/

class Byte {
private:
	e_byte m_value;
public:
	/**
	 * \brief Max e_byte value.
	*/
	const static e_byte MAX = INT8_MAX;

	/**
	 * \brief Min e_byte value. 
	*/
	const static e_byte MIN = INT8_MIN;
	
	/**
	 * \brief ctor.
	 * 
	 * @param value default 0.
	*/
	Byte(e_byte value=0) : m_value(value) {}
	
	/**
	 * \brief Get value.
	 * 
	 * @return receiver's value.
	*/
	inline e_byte getValue() const {return m_value;}

	/**
	 * \brief Check equality.
	 * 
	 * @param other the compared object with receiver.
	 * @return receiver's value == other's value.
	*/
	inline e_bool equals(const Byte& other) const {return m_value==other.m_value;}

	/**
	 * \brief Check order.
	 * 
	 * @param other the compared object with receiver.
	 * @return if receiver's value bigger than other's, return 1; both equals return 0; otherwise return -1.
	*/
	inline e_int  compare(const Byte& other) const {return m_value==other.m_value?0:(m_value>other.m_value?1:-1);}
	
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
}; // Byte

/**
 * \class Short
 * 
 * \brief 有符号 16 位整数包装类.
*/
class Short {
private:
	e_short m_value;
public:
	/**
		\brief 最大 16 位有符号整数值.
	*/
	const static e_short MAX = INT16_MAX;
	
	/**
		\brief 最小 16 位有符号整数值.
	*/
	const static e_short MIN = INT16_MIN;
	
	/**
		\brief 构造函数.
			
		@param value 有符号 16 位整数值, 默认为 0.
	*/
	inline Short(e_short value=0) : m_value(value) {}
	
	/**
		\brief 获取值.
		
		@return 接受者的值.
	*/
	inline e_short getValue() const {return m_value;}
	
	/**
	 * \brief Check equality.
	 * 
	 * @param other the compared object with receiver.
	 * @return receiver's value == other's value.
	*/
	inline e_bool equals(const Short& other) const {return m_value==other.m_value;}
	
	/**
	 * \brief Check order.
	 * 
	 * @param other the compared object with receiver.
	 * @return if receiver's value bigger than other's, return 1; both equals return 0; otherwise return -1.
	*/
	inline e_int  compare(const Short& other) const {return m_value==other.m_value?0:(m_value>other.m_value?1:-1);}
	
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
}; // Short
	
}


#endif // LEC_INTEGER_H_