/*
 * File Name:	Queue.hpp
 * Date/Time:
 * 	Start: 	2019-12-07 13:22 week:6
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_QUEUE_H_
#define _EASY_QUEUE_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "List.hpp"

namespace Easy {

/**
 * \class Queue
 * 
 * \brief 队列实现.
 * 
 * 用于值类型.
*/
template<class E>
class Queue {
private:
	List<E> m_list;
public:
	/**
	 * \brief 构造函数.
	*/
	inline Queue() {}
	
	/**
	 * \brief 元素入队.
	 * 
	 * @param item 入队的元素.
	 * @return 被加入的元素.
	*/
	inline E& enqueue(const E& item) {
		return m_list.add(item);
	}

	/**
	 * \brief 元素入队.
	 * 
	 * @param item 入队的元素.
	 * @return 被加入的元素.
	*/
	inline E& enqueue(E&& item) {
		return m_list.add(System::move(item));
	}

	/**
	 * \brief 元素离队.
	 * 
	 * @return 被移除的元素.
	 * @throw 如果接受者空, 抛出 EmptyContainerException.
	*/
	inline E dequeue() {
		if (m_list.empty())
			throw  EmptyContainerException{__func__, __LINE__, String{L"Queue"}};
		return System::move(m_list.removeFirst());
	}

	/**
	 * \brief 查看队尾元素.
	 * 
	 * @return 队尾元素, 也就是下一个出队的元素.
	 * @throw 如果接受者空, 抛出 EmptyContainerException.
	*/
	inline E& peek() const {
		if (m_list.empty())
			throw  EmptyContainerException{__func__, __LINE__, String{L"Queue"}};
		return m_list.getFirst();
	}

	/**
	 * \brief 获取元素个数.
	 * 
	 * @return 接受者包含的元素个数.
	*/
	inline e_int size() const {
		return m_list.size();
	}

	/**
	 * \brief 检查集合是否为空.
	 * 
	 * @return 如果集合为空, 返回真; 否则, 返回假.
	*/
	inline e_bool empty() const {
		return m_list.empty();
	}
	
	/**
	 * \brief 清空容器.
	 * 
	 * 清空内部缓存区并重新初始化.
	*/
	inline e_void clean() {
		m_list.clean();
	}
	
	/**
	 * \brief get string represent.
	 * 
	 * Get string represent.
	 * 
	 * @return receiver`s string represent.
	 * */
	inline String toString() const {
		return m_list.toString();
	}
}; // Queue
} // Easy

#endif // _EASY_QUEUE_H_

