/*
 * File Name:	AutoPointer.hpp
 * Date/Time:
 * 	Start: 	2019-12-07 11:18 week:6
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_AUTOPOINTER_H_
#define _EASY_AUTOPOINTER_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include "System.hpp"

namespace Easy {

/**
 * \class AutoPointer
 * 
 * \brief A simple scope pointer wrapper.
 * 
 * <p>
 * AutoPointer provides a convenient way for using pointers in some restricted 
 * circumstances. For example, if you must use pointer in a function, like a iterator of
 * a container, you can wrap the iterator pointer into a AutoPointer object, when the function's
 * job done or some exception throw, the iterator will be released auto, you need not care 
 * about it.
 * </p>
 * 
 * <p>
 * This class is only for pointers of single object, for pointers of array, you can use Array for 
 * class types or  BasicArray for basic types.
 * </p>
 * 
 * @author Tes Alone
 * @since  1.0
*/

template<class T>
class AutoPointer {
private:
	T* m_pointer;
public:
	/**
	 * \brief ctor.
	 * 
	 * @param pointer wrapped pointer, may be nullptr, default is nullptr.
	*/
	inline explicit AutoPointer(T* pointer=nullptr) : m_pointer(pointer) {}

	/**
	 * \brief dtor.
	*/
	inline ~AutoPointer() {delete m_pointer;}

	/**
	 * \brief copy ctor.
	 * 
	 * Providing this method is just for containers.
	 * 
	 * @throw throw an UnsupportedOperationException forever.
	*/
	inline AutoPointer(const AutoPointer& copy) {
		throw  UnsupportedOperationException{__func__, __LINE__, String{L"AutoPointer copy ctor."}};
	}

	/**
	 * \brief move ctor.
	 * 
	 * Move move's pointer to a new AutoPointer object, and 
	 * set move's pointer to nullptr.
	 * 
	 * @param move 
	*/
	inline AutoPointer(AutoPointer&& move) {
		m_pointer      = move.m_pointer;
		move.m_pointer = nullptr;
	}
	
	/**
	 * \brief copy assignment.
	 * 
	 * Providing this method is just for containers.
	 * 
	 * @throw throw an UnsupportedOperationException forever.
	*/
	inline AutoPointer& operator=(const AutoPointer& copy) {
		throw  UnsupportedOperationException{__func__, __LINE__, String{L"AutoPointer copy assignment."}};
		return *this; // never arrive here.
	}
	
	/**
	 * \brief move assignment.
	 * 
	 * Move move's pointer to a new AutoPointer object, and 
	 * set move's pointer to nullptr.
	 * 
	 * @param move 
	*/
	inline AutoPointer& operator=(AutoPointer&& move) {
		delete m_pointer;
		m_pointer      = move.m_pointer;
		move.m_pointer = nullptr;
		return *this;
	}
	
	/**
		\brief check equality.
		
		Compare receiver and other, if the objects their pointer pointing 
		are equals, return true, otherwise return false.
		<br/>
		<br/>
		Receiver and other's may be nullptr, if they are all nullptr, 
		return true, if there just one nullptr, return false, if neither  
		is nullptr, do the objects equals() method.
		<br/>
		<br/>
		Wrapped types need implement equals().
		
		@param other 
		@return check result.
	*/
	e_bool equals(const AutoPointer& other) const {
		if (m_pointer==nullptr && other.m_pointer==nullptr) 
			return true;
		if (m_pointer==nullptr && other.m_pointer!=nullptr)
			return false;
		if (m_pointer!=nullptr && other.m_pointer==nullptr)
			return false;
		return m_pointer->equals(*other.m_pointer);
	}
	
	/**
		\brief check order.
		
		Compare receiver and other, if the object pointing by receiver's pointer
		is bigger than other's, return 1, equals return 0, otherwise return -1.
		<br/>
		<br/>
		Receiver and other's may be nullptr, the nullptr is the lesser.
		If they are both not nullptr, do the compare() method.
		<br/>
		<br/>
		Wrapped types need implement compare().
		
		@param other 
		@return check result.
	*/
	e_int compare(const AutoPointer& other) const {
		if (m_pointer==nullptr && other.m_pointer==nullptr) 
			return 0;
		
		if (m_pointer==nullptr && other.m_pointer!=nullptr)
			return -1;
		
		if (m_pointer!=nullptr && other.m_pointer==nullptr)
			return 1;
		
		return m_pointer->compare(*other.m_pointer);
	}
	
	/**
	 * \brief get hash code.
	 * 
	 * Get receiver's hash code.
	 * <br/>
	 * Wrapped types need implement hashCode().
	 * 
	 * @return receiver's hash code.
	*/
	inline e_int hashCode() const {
		if (m_pointer == nullptr) 
			return 0;
		return m_pointer->hashCode();
	}
	
	/**
	 * \brief to string represent．
	 * 
	 * Get receiver's string represent.
	 * <br/>
	 * Wrapped types need implement toString().
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {
		if (m_pointer == nullptr)
			return System::move(String{L"(nullptr)"});
		return System::move(m_pointer->toString());
	}
	
	/**
	 * \brief dereference pointer.
	 * 
	 * Indirect access the object pointing by receiver's pointer.
	 * 
	 * @return the reference of the object pointing by receiver's pointer.
	 * @throw if receiver is a nullptr, throw a NullPointerException.
	*/
	inline T& operator*() const {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return *m_pointer;
	}
	
	/**
	 * \brief reference pointer.
	 * 
	 * Direct access the receiver's pointer.
	 * 
	 * @return the receiver's pointer.
	 * @throw if receiver is a nullptr, throw a NullPointerException.
	*/
	inline T* operator-> () const {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return m_pointer;
	}
	
	/**
	 * \brief get pointer.
	 *
	 * Get receiver's pointer.
	 * <br/>
	 * <b>Note: Do not delete return value.</b> 
	 *
	 * @return receiver's pointer.
	 * @throw if receiver is a nullptr, throw a NullPointerException.
	*/
	inline T* lock() const {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return m_pointer;
	}
	
	/**
		\brief check whether is a nullptr.
		
		Check whether receiver's pointer is a nullptr.
		
		@return if is a nullptr return true, otherwise return false.
	*/
	inline e_bool isNullPtr() const {
		return m_pointer == nullptr;
	}
	
	/**
	 * \brief move pointer.
	 * 
	 * This move methed move the receiver's pointer to some other place,
	 * need not to another AutoPointer, it return receiver's pointer and 
	 * set the pointer field of receiver to nullptr.
	 * <br/>
	 * <b>Note: Do not conflict with the moves in big five.</b>
	 * 
	 * @return receiver's pointer.
	 * @throw if receiver is a nullptr, throw a NullPointerException.
	 * */
	T* move() {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		T* result = m_pointer;
		m_pointer = nullptr;
		return result;
	}
}; // AutoPointer
} // Easy

#endif // _EASY_AUTOPOINTER_H_

