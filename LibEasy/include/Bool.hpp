/*
 * File Name:	Bool.hpp
 * Date/Time:
 * 	Start: 	2019-11-30 16:33 week:6
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_BOOL_H_
#define _EASY_BOOL_H_

#include "BaseTypes.hpp"
#include "String.hpp"

namespace Easy {

/**
 * \class Bool
 * 
 * \brief An e_bool wrapper.
 * 
 * @author Tes Alone
 * @since 1.0
*/

class Bool {
private:
	e_bool m_truth;
public:
	/**
	 * \brief ctor.
	 * 
	 * @param truth default false.
	*/
	Bool(e_bool truth=false) : m_truth(truth) {}

	/**
	 * \brief Get value.
	 * 
	 * @return receiver's value.
	*/
	inline e_bool getTruth() const {return m_truth;}

	/**
	 * \brief Check equality.
	 * 
	 * @param other the compared object with receiver.
	 * @return receiver's value == other's value.
	*/
	inline e_bool equals(const Bool& other) const {return m_truth==other.m_truth;}

	/**
	 * \brief Check order.
	 * 
	 * @param other the compared object with receiver.
	 * @return if receiver is true and other is false, return 1; both equals return 0; otherwise return -1.
	*/
	inline e_int compare(const Bool& other) const {return m_truth==other.m_truth?0:(m_truth?1:-1);}

	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	inline e_int hashCode() const {return m_truth ? 1 : 0;}
	
	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {return Bool::toString(m_truth);}
	
	/**
		\brief Check equality.
		
		@param b1 .
		@param b2 .
		@return b1 == b2
	*/
	inline static e_bool equals(e_bool b1, e_bool b2) {return b1==b2;}
	
	/**
		\brief Check order.
		
		@param b1 .
		@param b2 .
		@return b1==b2?0:(b1?1:-1)
	*/
	inline static e_int compare(e_bool b1, e_bool b2) {return b1==b2?0:(b1?1:-1);}

	/**
	 * \brief Get hash code.
	 * 
	 * @param truth .
	 * @return truth's hash code.
	*/
	inline static e_int hashCode(e_bool truth) {return truth ? 1 : 0;}
	
	/**
	 * \brief To string．
	 * 
	 * @param truth .
	 * @returns truth's string represent.
	*/
	inline static String toString(e_bool truth) {return System::move(truth?String{L"true"}:String{L"false"});}
	
}; // Bool
} // Easy

#endif // _EASY_BOOL_H_

