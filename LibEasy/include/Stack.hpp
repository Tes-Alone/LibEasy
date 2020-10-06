/*
 * File Name:	Stack.hpp
 * Date/Time:
 * 	Start: 	2019-12-05 19:32 week:4
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_STACK_H_
#define _EASY_STACK_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"

#include "Console.hpp"

namespace Easy {

/**
 * \class Stack
 * 
 * \brief 栈实现.
 * 
 * 使用值语义.
*/

template<class E>
class Stack {
private:
	E* m_stack;
	e_int m_top;
	e_int m_capacity;
public:
	/**
	 * \brief 构造函数.
	 * 
	 * 如果 initCapa 小于 16, 函数会将 initCapa 设置为 16.
	 *
	 * @param initCapa 初始化容量, 默认为 16.
	*/
	inline explicit Stack(e_int initCapa=16) {
		_init((initCapa<16)?16:initCapa);
	}

	/**
	 * \brief 析构函数.
	*/
	~Stack() {
		delete [] m_stack;
	}
	
	/**
	 * \brief copy 构造函数.
	 * 
	 * @param copy
	*/
	Stack(const Stack& copy) {
		m_top      = copy.m_top;
		m_capacity = (m_top<16) ? 16 : m_top;
		m_stack    = new E[m_capacity];
		_copy(m_stack, copy.m_stack);
	}

	/**
	 * \brief move 构造函数.
	 * 
	 * @param move
	*/
	Stack(Stack&& move) {
		m_top      = move.m_top;
		m_capacity = move.m_capacity;
		m_stack    = move.m_stack;
		move.m_capacity = 16;
		move.m_stack    = new E[m_capacity];
		move.m_top      = 0;
	}
	
	/**
	 * \brief copy 赋值.
	 * 
	 * @param copy
	*/
	Stack& operator=(const Stack& copy) {
		delete [] m_stack;
		m_top      = copy.m_top;
		m_capacity = (m_top<16) ? 16 : m_top;
		m_stack    = new E[m_capacity];
		_copy(m_stack, copy.m_stack);
		return *this;
	}

	/**
	 * \brief move 赋值.
	 * 
	 * @param move
	*/
	Stack& operator=(Stack&& move) {
		delete [] m_stack;
		m_top      = move.m_top;
		m_capacity = move.m_capacity;
		m_stack    = move.m_stack;
		move.m_capacity = 16;
		move.m_stack    = new E[m_capacity];
		move.m_top      = 0;
		return *this;
	}
	
	/**
	 * \brief 压栈.
	 * 
	 * @param item 要压栈的元素.
	 * @return 压入接受者的元素.
	*/
	E& push(const E& item) {
		if (m_top == m_capacity) {
			_updateCapacity(m_capacity*2);
		}
		m_stack[m_top++] = item;
		return m_stack[m_top-1];
	}

	/**
	 * \brief 压栈.
	 * 
	 * @param item 要压栈的元素.
	 * @return 压入接受者的元素.
	*/
	E& push(E&& item) {
		if (m_top == m_capacity) {
			_updateCapacity(m_capacity*2);
		}
		m_stack[m_top++] = System::move(item);
		return m_stack[m_top-1];
	}

	/**
	 * \brief 出栈.
	 * 
	 * @return 栈顶元素.
	 * @throw 如果栈空, 抛出 EmptyContainerException.
	*/
	inline E pop() {
		if (m_top == 0) 
			throw  EmptyContainerException{__func__, __LINE__, String{L"Stack"}};
		return System::move(m_stack[--m_top]);
	}

	/**
	 * \brief 查看栈顶.
	 * 
	 * @return 栈顶元素.
	 * @throw 如果栈空, 抛出 EmptyContainerException.
	*/
	inline E& peek() const {
		if (m_top == 0)
			throw  EmptyContainerException{__func__, __LINE__, String{L"Stack"}};
		return m_stack[m_top-1];
	}

	/**
	 * \brief 缩减内存到正好存储元素.
	 * 
	 * @return 缩减内存后的接受者.
	*/
	inline Stack& trimToLength() {
		if (m_capacity == 16)
			return *this;
		
		if (m_top != m_capacity) {
			_updateCapacity((m_top<16)?16:m_top);
		}
		return *this;
	}

	/**
	 * \brief 获取元素个数.
	 * 
	 * @return 接受者包含的元素个数.
	*/
	inline e_int size() const {
		return m_top;
	}

	/**
	 * \brief 检查集合是否为空.
	 * 
	 * @return 如果集合为空, 返回真; 否则, 返回假.
	*/
	e_bool empty() const {
		return m_top == 0;
	}
	
	/**
	 * \brief 查看当前使用内存, 以元素大小计.
	 * 
	 * @return 接受者当前使用内存大小.
	*/
	inline e_int capacity() const {
		return m_capacity;
	}
	
	/**
	 * \brief 清空容器.
	 * 
	 * 清空内部缓存区并重新初始化.
	*/
	e_void clean() {
		delete [] m_stack;
		m_capacity = 16;
		m_top      = 0;
		m_stack    = new E[m_capacity];
	}
private:
	e_void _init(e_int initCapa) {
		m_capacity = initCapa;
		m_stack    = new E[m_capacity];
		m_top      = 0;
	}

	e_void _updateCapacity(e_int newCapacity) {
		m_capacity  = newCapacity;
		E* newStack = new E[m_capacity];
		_move(newStack, m_stack);
		delete [] m_stack;
		m_stack = newStack;
	}

	e_void _move(E* des, E* src) {
		for (e_int i=0; i<m_top; i++) {
			des[i] = System::move(src[i]);
		}
	}

	e_void _copy(E* des, E* src) {
		for (e_int i=0; i<m_top; i++) {
			des[i] = src[i];
		}
	}
}; // Stack
} // Easy

#endif // _EASY_STACK_H_

