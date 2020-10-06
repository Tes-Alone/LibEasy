/*
 * File Name:	Float.hpp
 * Date/Time:
 * 	Start: 	2019-11-26 18:28 week:2
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FLOAT_H_
#define _EASY_FLOAT_H_

#include <cfloat>

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"

namespace Easy {

/**
 * \class Float
 * 
 * \brief An e_float wrapper.
 * 
 * @author Tes Alone
 * @since 1.0
*/
class Float {
private:
	e_float m_value;
public:

	/**
		\brief Constant PI.
		
		@return PI.
	*/
	constexpr static e_float PI() {return 3.14159265f;}
	
	/**
		\brief Constant e.
		
		@return e.
	*/
	constexpr static e_float e() {return  2.71828182f;}

	/**
	 * \brief Constant epsilon.
	 * 
	 * @return epsilon.
	*/
	constexpr inline static e_float epsilon() {return FLT_EPSILON;}
	
	/**
	 * \brief Max e_float value.
	 * 
	 * @return max e_float value.
	 * 
	*/
	constexpr inline static e_float max() {return FLT_MAX;}
	
	/**
	 * \brief Min e_float value.
	 * 
	 * @return min e_float value.
	 * 
	*/
	constexpr inline static e_float min() {return FLT_MIN;}
	
	/**
	 * \brief Get max allowed exponent.
	 * 
	 * @return max allowed exponent.
	 * */
	constexpr inline static e_float maxExp() {return FLT_MAX_EXP;}
	
	/**
	 * \brief Get min allowed exponent.
	 * 
	 * @return min allowed exponent.
	 * */
	constexpr inline static e_float minExp() {return FLT_MIN_EXP;}
	
	/**
	 * \brief ctor.
	 * 
	 * @param value  defualt 0.0.
	*/
	Float(e_float value=0.0) : m_value(value)  {}
	
	/**
		\brief Get wrapped value.
		
		@return receiver's value.
	*/
	inline e_float getValue() const {return m_value;}
	
	/**
	 * \brief Check equality.
	 * 
	 * Compare receiver and other's value, equals return true, otherwise return false.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param other .
	 * @return result.
	*/
	inline e_bool equals(const Float& other) const {
		return equals(m_value, other.m_value);
	}

	/**
	 * \brief Check order.
	 * 
	 * If receiver's value bigger than other's, return 1; both equals return 0; otherwise return -1.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param other .
	 * @return result.
	*/
	inline e_int compare(const Float& other) const {
		return compare(m_value, other.m_value);
	}

	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	inline e_int hashCode() const {return Float::hashCode(m_value);}

	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {return System::move(Float::toString(m_value));}

	/**
	 * \brief Check equality.
	 * 
	 * Compare d1 and d2, equals return true, otherwise return false.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param f1 .
	 * @param f2 .
	 * @return result.
	*/
	static e_bool equals(e_float f1, e_float f2);

	/**
	 * \brief Check order.
	 * 
	 * If d1 is bigger than d2, return 1; both equals return 0; otherwise return -1.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param f1 .
	 * @param f2 .
	 * @return result.
	*/
	static e_int compare(e_float f1, e_float f2);

	/**
	 * \brief Get hash code.
	 * 
	 * @param f .
	 * @return f's hash code.
	 * */
	static e_int hashCode(e_float f);	
	
	/**
	 * \brief To string．
	 * 
	 * @param f .
	 * @returns f's string represent.
	*/
	static String toString(e_float f);
	
	/**
	 * \brief To hex string.
	 * 
	 * @param f .
	 * @return f's hex string.
	*/
	static String toHexString(e_float f);
	
	/**
	 * \brief To oct string.
	 * 
	 * @param f .
	 * @return f's oct string.
	*/
	static String toOctString(e_float f);
	
	/**
	 * \brief To binary string.
	 * 
	 * @param f .
	 * @return f's binary string.
	*/
	static String toBinaryString(e_float f);
	
	/**
		\brief To 32-bits.
		
		@param f
		@return f's 32-bits.
	*/
	static e_uint toIntBits(e_float f);
	
	/**
		\brief Check whether is a nan.
		
		@param f
		@return if f is a nan return true, otherwise return false.
	*/
	inline static e_bool isNaN(e_float f) {
		e_int bits = toIntBits(f);
		return bits == (e_int)0xffc00000;
	}
	
	/**
		\brief Check whether is infinite, both positive and negative.
		
		@param f
		@return if f is a infinite return true, otherwise return false.
	*/
	inline static e_bool isInfinite(e_float f) {
		e_int bits = toIntBits(f);
		return (bits==(e_int)0x7f800000) || (bits==(e_int)0xff800000);
	}
	
	/**
	 * \brief Parse string to e_float.
	 * 
	 * str needs match: "-?[0-9]*\.[0-9]+" 、"-?[0-9]+[eE]-?[0-9]+"、"-?[0-9]*\.[0-9]+[eE]-?[0-9]+".
	 * 
	 * @param str 
	 * @return a e_float value.
	 * @throw if str is illegal, throws NumberFormatException.
	 * @throw if str's exponent is bigger or less than Float::maxExp() and Float::minExp(), respectively, throws FloatExpException.
	*/
	static e_float parse(const String& str);
	
}; // Float

/**
 * \class Double
 * 
 * \brief An e_double wrapper.
 * 
 * @author Tes Alone
 * @since 1.0
*/
class Double {
private:
	e_double m_value;
public:

	/**
		\brief Constant PI.
		
		@return PI.
	*/
	constexpr inline static e_double PI() {return 3.141592653589793;}
	
	/**
		\brief Constant e.
		
		@return e.
	*/
	constexpr inline static e_double e() {return 2.718281828459045;}
		
	/**
	 * \brief Constant epsilon.
	 * 
	 * @return epsilon.
	*/
	constexpr inline static e_double epsilon() {return DBL_EPSILON;}
	
	/**
	 * \brief Max e_double value.
	 * 
	 * @return max e_double value.
	 * 
	*/
	constexpr inline static e_double max() {return DBL_MAX;}
	
	/**
	 * \brief Min e_double value.
	 * 
	 * @return min e_double value.
	 * 
	*/
	constexpr inline static e_double min() {return DBL_MIN;}
	
	/**
	 * \brief Get max allowed exponent.
	 * 
	 * @return max allowed exponent.
	 * */
	constexpr inline static e_double maxExp() {return DBL_MAX_EXP;}
	
	/**
	 * \brief Get min allowed exponent.
	 * 
	 * @return min allowed exponent.
	 * */
	constexpr inline static e_double minExp() {return DBL_MIN_EXP;}
	
	/**
	 * \brief ctor.
	 * 
	 * @param value  defualt 0.0.
	*/
	Double(e_double value=0.0) : m_value(value)  {}
	
	/**
		\brief Get wrapped value.
		
		@return receiver's value.
	*/
	inline e_double getValue() const {return m_value;}
	
	/**
	 * \brief Check equality.
	 * 
	 * Compare receiver and other's value, equals return true, otherwise return false.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param other .
	 * @return result.
	*/
	inline e_bool equals(const Double& other) const {
		return equals(m_value, other.m_value);
	}

	/**
	 * \brief Check order.
	 * 
	 * If receiver's value bigger than other's, return 1; both equals return 0; otherwise return -1.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param other .
	 * @return result.
	*/
	inline e_int compare(const Double& other) const {
		return compare(m_value, other.m_value);
	}

	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	inline e_int hashCode() const {return Double::hashCode(m_value);}

	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {return System::move(Double::toString(m_value));}

	/**
	 * \brief Check equality.
	 * 
	 * Compare d1 and d2, equals return true, otherwise return false.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param d1 .
	 * @param d2 .
	 * @return result.
	*/
	static e_bool equals(e_double d1, e_double d2);

	/**
	 * \brief Check order.
	 * 
	 * If d1 is bigger than d2, return 1; both equals return 0; otherwise return -1.
	 * <b>Method use the epsilon.</b>
	 * 
	 * @param d1 .
	 * @param d2 .
	 * @return result.
	*/
	static e_int compare(e_double d1, e_double d2);

	/**
	 * \brief Get hash code.
	 * 
	 * @param d .
	 * @return d's hash code.
	 * */
	static e_int hashCode(e_double d);	
	
	/**
	 * \brief To string．
	 * 
	 * @param d .
	 * @returns d's string represent.
	*/
	static String toString(e_double d);
	
	/**
	 * \brief To hex string.
	 * 
	 * @param d .
	 * @return d's hex string.
	*/
	static String toHexString(e_double d);
	
	/**
	 * \brief To oct string.
	 * 
	 * @param d .
	 * @return d's oct string.
	*/
	static String toOctString(e_double d);
	
	/**
	 * \brief To binary string.
	 * 
	 * @param d .
	 * @return d's binary string.
	*/
	static String toBinaryString(e_double d);
	
	/**
		\brief To 64-bits.
		
		@param d
		@return d's 64-bits.
	*/
	static e_ulong toLongBits(e_double d);
	
	/**
		\brief Check whether is a nan.
		
		@param d
		@return if d is a nan return true, otherwise return false.
	*/
	inline static e_bool isNaN(e_double d) {
		e_long bits = toLongBits(d);
		return bits == (e_long)0xfff8000000000000;
	}
	
	/**
		\brief Check whether is infinite, both positive and negative.
		
		@param d
		@return if d is a infinite return true, otherwise return false.
	*/
	inline static e_bool isInfinite(e_double d) {
		e_long bits = toLongBits(d);
		return (bits==(e_long)0x7ff0000000000000) || 
				(bits==(e_long)0xfff0000000000000);
	}
	
	/**
	 * \brief Parse string to e_double.
	 * 
	 * str needs match: "-?[0-9]*\.[0-9]+" 、"-?[0-9]+[eE]-?[0-9]+"、"-?[0-9]*\.[0-9]+[eE]-?[0-9]+".
	 * 
	 * @param str 
	 * @return a e_double value.
	 * @throw if str is illegal, throws NumberFormatException.
	 * @throw if str's exponent is bigger or less than Double::maxExp() and Double::minExp(), respectively, throws FloatExpException.
	*/
	static e_double parse(const String& str);
}; // Double

} // Easy

#endif // _EASY_FLOAT_H_

