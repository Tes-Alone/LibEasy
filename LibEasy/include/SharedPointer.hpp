/*
 * File Name:	SharedPointer.hpp
 * Date/Time:
 * 	Start: 	2019年12月15日 19:17:01 周日
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_SHAREDPOINTER_H_
#define _EASY_SHAREDPOINTER_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include "Math.hpp"
#include "Integer.hpp"

namespace Easy {

/**
	\class RefCount
	
	\brief 引用计数类.
*/
class RefCount {

public:
	/**
		\brief 构造方法.
	*/
	RefCount() {
		m_useCount  = 0;
		m_weakCount = 0;
	}
	
	/**
		\brief 增加 Shared 计数.
	*/
	e_void incUse() {
		m_useCount++;
	}
	
	/**
		\brief 增加 Weak 计数.
	*/
	e_void incWeak() {
		m_weakCount++;
	}
	
	/**
		\brief 减少 Shared 计数.
	*/
	e_void decUse() {
		m_useCount--;
	}
	
	/**
		\brief 减少 Weak 计数.
	*/
	e_void decWeak() {
		m_weakCount--;
	}
	
	/**
		\brief 获取 Shared 计数.
	*/
	e_int useCount() {
		return m_useCount;
	}
	
	/**
		\brief 获取 Weak 计数.
	*/
	e_int weakCount() {
		return m_weakCount;
	}
	
private:
	e_int m_useCount;
	e_int m_weakCount;
};

template<class T>
class EnableSharedFromThis;

template<class T>
class WeakPointer;

/**
	\class SharedPointer
	
	\brief 简易实现的共享指针.
*/
template<class T>
class SharedPointer {	

template<typename>
friend class WeakPointer;

template<typename>
friend class EnableSharedFromThis;

private:
	T* 		  m_pointer;
	RefCount* m_rc;
public:
	/**
		\brief 构造函数.
		
		@param pointer 被共享的指针.
	*/
	explicit SharedPointer(T* pointer=nullptr);
	
	/**
	  	\brief ctor.
	  	
	  	Create a shared pointer from a weak pointer.
	  	
	  	@param weak the weak pointer.
	  	@throw if weak is a bad pointer, throw BadWeakPointerException.
	*/
	explicit SharedPointer(const WeakPointer<T>& weak);
	
	
	/**
		\brief 析构函数.
	*/
	inline ~SharedPointer() {
		_decCount();
	}
	
	/**
		\brief copy 构造函数.
		
		@param copy
	*/
	SharedPointer(const SharedPointer& copy) {		
		m_pointer = copy.m_pointer;
		m_rc      = copy.m_rc;
		if (m_rc != nullptr) m_rc->incUse();
	}
	
	/**
		\brief copy 赋值.
		
		@param copy
	*/
	SharedPointer& operator=(const SharedPointer& copy) {
		if (this == &copy) return  *this;
		_decCount();
		m_pointer = copy.m_pointer;
		m_rc      = copy.m_rc;
		if (m_rc != nullptr) m_rc->incUse();
		return *this;
	}
	
	/**
		\brief move 构造函数
		
		@param move
	*/
	SharedPointer(SharedPointer&& move) {
		m_pointer = move.m_pointer;
		m_rc      = move.m_rc;
		if (m_rc != nullptr) m_rc->incUse();
	}
	
	/**
		\brief move 赋值.
		
		@param move 
	*/
	SharedPointer& operator=(SharedPointer&& move) {
		if (this == &move) return *this;
		_decCount();
		m_pointer = move.m_pointer;
		m_rc      = move.m_rc;
		if (m_rc != nullptr) m_rc->incUse();
		return *this;
	}
	
	/**
		\brief 等于函数.
		
		接受者和比较者都可以是空指针.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
	*/
	e_bool equals(const SharedPointer& other) const {
		if (m_pointer==nullptr && other.m_pointer==nullptr) 
			return true;
		
		if (m_pointer==nullptr && other.m_pointer!=nullptr)
			return false;
		
		if (m_pointer!=nullptr && other.m_pointer==nullptr)
			return false;
		
		return m_pointer->equals(other);
	}
	
	/**
		\brief 比较函数.
		
		接受者和比较者都可以是空指针, 空指针被视为较小者.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值大于 other 的值, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	e_int compare(const SharedPointer& other) const {
		if (m_pointer==nullptr && other.m_pointer==nullptr) 
			return 0;
		
		if (m_pointer==nullptr && other.m_pointer!=nullptr)
			return -1;
		
		if (m_pointer!=nullptr && other.m_pointer==nullptr)
			return 1;
		return m_pointer->compare(other);
	}
	
	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	e_int hashCode() const {
		if (m_pointer == nullptr) 
			return 0;
		return m_pointer->hashCode();
	}
	
	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	String toString() const {
		if (m_pointer == nullptr)
			return System::move(String{L"(nullptr)"});
		return System::move(m_pointer->toString());
	}
	
	/**
	 * \brief 指针解引用.
	 * 
	 * @return 接受者封装的指针指向的对象引用.
	 * @throw 如果接受者是空指针, 抛出 NullPointerException;
	*/
	T& operator*() const {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return *m_pointer;
	}
	
	/**
	 * \brief 指针解引用.
	 * 
	 * @return 接受者封装的指针.
	 * @throw 如果接受者是空指针, 抛出 NullPointerException;
	*/
	T* operator-> () const {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return m_pointer;
	}
	
	/**
	 * \brief 获取指针.
	 * 
	 * 不要 delete 返回值.
	 *
	 * @return 接受者封装的指针.
	 * @throw 如果接受者是空指针, 抛出 NullPointerException;
	*/
	inline T* lock() const {
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return m_pointer;
	}
	
	/**
		\brief 判断是否是空指针.
		
		@return 如果接受者是空指针, 返回真; 否则返回假.
	*/
	inline e_bool isNullPtr() const {
		return m_pointer == nullptr;
	}
	
	/**
		\brief 获取共享次数.
		
		@return 当前被共享指针次数.
	*/
	inline e_int count() const {
		return m_rc!=nullptr ? m_rc->useCount() : 0;
	}
private:
	e_void _decCount() {
		if (m_rc != nullptr) {
			m_rc->decUse();
			if (m_rc->useCount() == 0) {
				delete m_pointer;
			}
		}
	}
}; // SharedPointer

/**
	\class WeakPointer
	
	\brief 简易实现的弱指针.
*/

template<class T>
class WeakPointer {

template<typename>
friend class SharedPointer;

template<typename>
friend class EnableSharedFromThis;

private:
	T*        m_pointer;
	RefCount* m_rc;

public:
	
	/**
	 * \brief 构造函数.
	 * */
	WeakPointer() {
		m_pointer = nullptr;
		m_rc      = new RefCount; // 这里的 m_rc 赋值不是可有可无, 为了辅助 EnableSharedFromThis
		m_rc->incWeak();
	}
	
	/**
		\brief 构造函数.
		
		@param pointer 被共享的指针.
	*/
	explicit WeakPointer(const SharedPointer<T>& pointer) {
		m_pointer = pointer.m_pointer;
		m_rc      = pointer.m_rc;
		if (m_rc != nullptr) m_rc->incWeak();
	}
	
	
	/**
		\brief 析构函数.
	*/
	inline ~WeakPointer() {
		_decCount();
	}
	
	/**
		\brief copy 构造函数.
		
		@param copy
	*/
	WeakPointer(const WeakPointer& copy) {
		m_pointer = copy.m_pointer;
		m_rc	  = copy.m_rc;
		if (m_rc != nullptr) m_rc->incWeak();
	}
	
	/**
		\brief copy 赋值.
		
		@param copy
	*/
	WeakPointer& operator=(const WeakPointer& copy) {
		if (this == &copy) return *this;
		_decCount();
		m_pointer = copy.m_pointer;
		m_rc	  = copy.m_rc;
		if (m_rc != nullptr) m_rc->incWeak();	
		return *this;
	}
	
	/**
		\brief move 构造函数
		
		@param move
	*/
	WeakPointer(WeakPointer&& move) {
		m_pointer = move.m_pointer;
		m_rc	  = move.m_rc;
		if (m_rc != nullptr) m_rc->incWeak();
	}
	
	/**
		\brief move 赋值.
		
		@param move 
	*/
	WeakPointer& operator=(WeakPointer& move) {
		if (this == &move) return *this;
		_decCount();
		m_pointer = move.m_pointer;
		m_rc	  = move.m_rc;
		if (m_rc != nullptr) m_rc->incWeak();
		return *this;
	}
	
	/**
		\brief 等于函数.
		
		接受者和比较者都可以是空指针以及 BadWeakPointer.
		如果接受这和比较者都是 BadWeakPointer, 返回真, 否则返回假.
		如果接受这和比较者都是 空指针, 返回真, 否则返回假.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
	*/
	e_bool equals(const WeakPointer& other) const {
		if (_pointerDangling() && other._pointerDangling())
			return true;
		if (_pointerDangling() && !other._pointerDangling())
			return false;
		if (!_pointerDangling() && other._pointerDangling())
			return false;
		if (m_pointer==nullptr && other.m_pointer==nullptr) 
			return true;
		if (m_pointer==nullptr && other.m_pointer!=nullptr)
			return false;
		if (m_pointer!=nullptr && other.m_pointer==nullptr)
			return false;
		return m_pointer->equals(other);
	}
	
	/**
		\brief 比较函数.
		
		接受者和比较者都可以是空指针以及 BadWeakPointer, 空指针和 BadWeakPointer 被视为较小者.
		
		@param other 接受者的比较对象.
		@return 如果接受者的值大于 other 的值, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	e_int compare(const WeakPointer& other) const {
		if (_pointerDangling() && other._pointerDangling())
			return 0;
		if (_pointerDangling() && !other._pointerDangling())
			return -1;
		if (!_pointerDangling() && other._pointerDangling())
			return 1;
			
		if (m_pointer==nullptr && other.m_pointer==nullptr) 
			return 0;
		if (m_pointer==nullptr && other.m_pointer!=nullptr)
			return -1;
		if (m_pointer!=nullptr && other.m_pointer==nullptr)
			return 1;

		return m_pointer->compare(other);
	}
	
	/**
	 * \brief 哈希函数.
	 * 
	 * 接受者可以是 BadWeakPointer.
	 * 
	 * @return 接受者的哈希值.
	*/
	e_int hashCode() const {
		if (_pointerDangling())
			return 0;
		if (m_pointer == nullptr) 
			return 0;
		return m_pointer->hashCode();
	}
	
	/**
	 * \brief 转到字符串．
	 * 
	 * 接受者可以是 BadWeakPointer 以及空指针.
	 * 
	 * @returns 接受者的字符串表示.
	*/
	String toString() const {
		if (m_pointer == nullptr)
			return System::move(String{L"(nullptr)"});
		if (_pointerDangling())
			return System::move(String{L"(bad weak pointer)"});
		return System::move(m_pointer->toString());
	}
	
	/**
	 * \brief 指针解引用.
	 * 
	 * @return 接受者封装的指针指向的对象引用.
	 * @throw 如果接受者是空指针, 抛出 NullPointerException.
	 * @throw 如果接受者的指针被释放, 抛出 BadWeakPointerException.
	*/
	T& operator*() const {
		if (_pointerDangling())
			throw BadWeakPointerException{__func__, __LINE__};
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return *m_pointer;
	}
	
	/**
	 * \brief 指针解引用.
	 * 
	 * @return 接受者封装的指针.
	 * @throw 如果接受者是空指针, 抛出 NullPointerException.
	 * @throw 如果接受者的指针被释放, 抛出 BadWeakPointerException.
	*/
	T* operator-> () const {
		if (_pointerDangling())
			throw BadWeakPointerException{__func__, __LINE__};
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		return m_pointer;
	}
	
	/**
	 * \brief 获取指针.
	 * 
	 * 不要 delete 返回值.
	 *
	 * @return 接受接受者封装的指针的共享指针.
	 * @throw 如果接受者是空指针, 抛出 NullPointerException.
	 * @throw 如果接受者的指针被释放, 抛出 BadWeakPointerException.
	*/
	SharedPointer<T> lock() const {
		if (_pointerDangling())
			throw BadWeakPointerException{__func__, __LINE__};
		if (m_pointer == nullptr)
			throw NullPointerException{__func__, __LINE__};
		SharedPointer<T> sp;
		sp._decCount();
		sp.m_pointer = m_pointer;
		sp.m_rc = m_rc;
		if (m_rc != nullptr) sp.m_rc->incUse();
		return sp;
	}
	
	/**
		\brief 判断是否是空指针.
		
		@return 如果接受者是空指针, 返回真; 否则返回假.
	*/
	inline e_bool isNullPtr() const {
		return m_pointer == nullptr;
	}
	
	/**
		\brief 获取弱共享次数.
		
		@return 当前被弱共享指针次数.
	*/
	e_int count() const {
		return m_rc!=nullptr ? m_rc->weakCount() : 0;
	}
	
	/**
	 
	*/
	e_int useCount() const {
		return m_rc!=nullptr ? m_rc->useCount() : 0;
	}

private:
	e_void _decCount() {
		if (m_rc != nullptr) {
			m_rc->decWeak();
			if (m_rc->useCount() > 0) {
				m_rc->incWeak();
			}
			
			if (m_rc->weakCount() == 0) {
				delete m_rc;
			}
		}
	}
	
	e_bool _pointerDangling() const {
		return m_rc!=nullptr && m_rc->useCount()==0;
	}
}; //WeakPointer

/**
 * \class EnableSharedFromThis
 * 
 * \brief A base class for classes want to use shared pointer.
 * */
template<class T>
class EnableSharedFromThis {

template<typename>
friend class SharedPointer;

private:
	WeakPointer<T> m_weak;
public:
	/**
	 * \brief ctor.
	 * 
	 * do nothing.
	 * */
	EnableSharedFromThis() {}
	
	/**
	 * \brief dtor.
	 * 
	 * do nothing, because this class may be a base class, this method need be virtual.
	 * */
	virtual ~EnableSharedFromThis() {}
	
	/**
	 * \brief Create a shared pointer from this.
	 * 
	 * This method cannot be called in ctors.
	 * 
	 * @return the created shared pointer.
	 * */
	SharedPointer<T> sharedFromThis() {
		return SharedPointer<T>{(T*)this};
	}
	
	/**
	 * \brief Check equality.
	 * 
	 * Compares with memory addresses.
	 * 
	 * @param other the object compares with receiver.
	 * @return if receiver and other equals, return true, otherwise false.
	 * */
	virtual e_bool equals(const SharedPointer<T>& other) const {
		return this == other.lock();
	}
	
	/**
	 * \brief Check order.
	 * 
	 * Compares with memory addresses.
	 * 
	 * @param other the object compares with receiver.
	 * @return if receiver is bigger, return 1, the both equals return 0, otherwise return -1;
	 * */
	virtual e_int compare(const SharedPointer<T>& other) const {
		return this==other.lock()?0:(this>other.lock()?1:-1);
	}
	
	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	virtual e_int hashCode() const {
		return Math::abs(Long::hashCode(reinterpret_cast<e_long>(this)));
	}
	
	/**
	 * \brief Get the string represent.
	 * 
	 * @return thr string represent of receiver.
	 * */
	virtual String toString() const {
		return String{"EnableSharedFromThis@"} + Integer{hashCode()};
	}
	
	/**
	 * \brief Convert to sub types.
	 * 
	 * Convert receiver's pointer type to subs of receiver.
	 * The parameter S needs a sub type of this class, and method
	 * not check it.
	 * */
	template<class S>
	S* lockTo() const {
		return (S*)this;
	}
}; // EnableSharedFromThis

template<class T>
SharedPointer<T>::SharedPointer(T* pointer) {
	if (pointer == nullptr) {
		m_pointer = nullptr;
		m_rc      = nullptr;
	} else {
		EnableSharedFromThis<T>* ep = (EnableSharedFromThis<T>*)pointer;
		if (ep->m_weak.useCount() == 0) { // 如果 WeakPointer 初始化了 m_rc, 这里的调用永远不会出现内存访问问题
			ep->m_weak.m_pointer = pointer;
			m_pointer = pointer;
			m_rc = ep->m_weak.m_rc;
			m_rc->incUse();
		} else {
			m_pointer = pointer;
			ep->m_weak.m_pointer = pointer;
			m_rc = ep->m_weak.m_rc;
			m_rc->incUse();
		}
	}
}

template<class T>
SharedPointer<T>::SharedPointer(const WeakPointer<T>& weak) {
	if (weak._pointerDangling())
		throw BadWeakPointerException{__func__, __LINE__};
	m_pointer = weak.m_pointer;
	m_rc      = weak.m_rc;
	if (m_rc != nullptr) m_rc->incUse();
}

} // Easy

#endif // _EASY_SHAREDPOINTER_H_

