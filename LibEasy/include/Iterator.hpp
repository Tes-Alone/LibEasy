/*
 * File Name:	Iterator.hpp
 * Date/Time:
 * 	Start: 	2019-12-06 14:18 week:5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_ITERATOR_H_
#define _EASY_ITERATOR_H_

#include "BaseTypes.hpp"
#include "SharedPointer.hpp"

namespace Easy {

/**
 * \class Iterator
 * 
 * \brief 迭代器类.
 * 
 * 用于遍历集合类.
*/

template<class E>
class Iterator : public EnableSharedFromThis<Iterator<E>> {
public:
	/**
	 * \brief 构造函数.
	*/
	Iterator() {}

	/**
	 * \brief 析构函数.
	*/
	virtual ~Iterator() {}

	/**
	 * \brief 判断是否结束.
	 * 
	 * @return 结束返回真; 否则返回假.
	*/
	virtual e_bool isEnd() const =0;

	/**
	 * \brief 获取当前元素.
	 * 
	 * @return 当前元素.
	 * @throw 如果迭代器以结束, 抛出 IteratorIsEndException.
	*/
	virtual E& current() const =0;
	
	/**
		\brief 移动到下一位置.
		
		@return 如果没有下一位置, 返回假, 否则返回真.
	*/
	virtual e_bool advance() =0;
	
	String toString() const override {
		return String{L"Iterator@"} + Integer{EnableSharedFromThis<Iterator<E>>::hashCode()};
	}

private:
	Iterator(Iterator& copy)  = delete;
	Iterator(Iterator&& move)  = delete;
	Iterator& operator=(Iterator& copy)  = delete;
	Iterator& operator=(Iterator&& move)  = delete;
}; // Iterator


} // Easy

#endif // _EASY_ITERATOR_H_

