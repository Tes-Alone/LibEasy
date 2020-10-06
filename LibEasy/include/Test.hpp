/*
 * File Name:	Test.hpp
 * Date/Time:
 * 	Start: 	2019年12月18日 17:52:46 周三
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_TEST_H_
#define _EASY_TEST_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "Console.hpp"

namespace Easy {

/**
	\class Test
	
	\brief 简易测试功能类.
*/

class Test {
	
public:
	/**
		\brief 测试两个 e_byte 值是否相等.
		
		@param index 测试号.
		@param b1 e_byte 值.
		@param b2 e_byte 值.
	*/
	static e_void equals(e_int index, e_byte b1, e_byte b2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (b1 == b2) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_short 值是否相等.
		
		@param index 测试号.
		@param s1 e_short 值.
		@param s2 e_short 值.
	*/
	static e_void equals(e_int index, e_short s1, e_short s2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (s1 == s2) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_int 值是否相等.
		
		@param index 测试号.
		@param i1 e_int 值.
		@param i2 e_int 值.
	*/
	static e_void equals(e_int index, e_int i1, e_int i2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (i1 == i2) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_long 值是否相等.
		
		@param index 测试号.
		@param l1 e_long 值.
		@param l2 e_long 值.
	*/
	static e_void equals(e_int index, e_long l1, e_long l2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (l1 == l2) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_char 值是否相等.
		
		@param index 测试号.
		@param c1 e_char 值.
		@param c2 e_char 值.
	*/
	static e_void equals(e_int index, e_char c1, e_char c2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (c1 == c2) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_bool 值是否相等.
		
		@param index 测试号.
		@param b1 e_bool 值.
		@param b2 e_bool 值.
	*/
	static e_void equals(e_int index, e_bool b1, e_bool b2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (b1 == b2) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_float 值是否相等.
		
		@param index 测试号.
		@param f1 e_float 值.
		@param f2 e_float 值.
	*/
	static e_void equals(e_int index, e_float f1, e_float f2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (Float::equals(f1, f2)) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试两个 e_double 值是否相等.
		
		@param index 测试号.
		@param d1 e_double 值.
		@param d2 e_double 值.
	*/
	static e_void equals(e_int index, e_double d1, e_double d2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (Double::equals(d1, d2)) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
		\brief 测试任意两个同类型对象是否相等.
		
		对象类型需实现 equals() 方法.
		
		@param index 测试号.
		@param e1 对象1.
		@param e2 对象2.
	*/
	template<class E>
	static e_void equals(e_int index, const E& e1, const E& e2) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		if (e1.equals(e2)) Console::outln(L"Test Pass.");
		else Console::outln(L"Test Failed!");
	}
	
	/**
	 * \brief test whether function throw Exception.
	 * 
	 * @param index test number.
	 * @param func tested function.
	 * @param args arguments pass to func.
	 * */
	template<class Func, class ...Args>
	static e_void assertThrow(e_int index, Func* func, Args... args) {
		Console::out(String{L"Test "}+Integer{index}+String{L":  "});
		try {
			func(args...);
			Console::outln(L"Test Failed!");
		} catch (Exception& e) {
			Console::outln(L"Test Pass.");
		}
	}
private:
	Test() =delete;
	~Test() =delete;
	Test(Test& copy)  = delete;
	Test(Test&& move)  = delete;
	Test& operator=(Test& copy)  = delete;
	Test& operator=(Test&& move)  = delete;
}; // Test
} // Easy

#endif // _EASY_TEST_H_

