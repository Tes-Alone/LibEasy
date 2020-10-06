
/*
 * File name: System.hpp 
 *
 * Date/Time:
 *		start: 2019年10月03日 20时10分07秒 周4
 *		end:
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_SYSTEM_H_
#define LEC_SYSTEM_H_

#include <windows.h>

#include <stdlib.h>
#include <time.h>

#include "BaseTypes.hpp"
#include "Exception.hpp"

namespace Easy {

class String;

/**
	\class System

	\brief 系统功能函数.
	
	@since 1.0
*/
class System {
	public:
			
		/**
			\brief 获取程序执行时间.
			
			@return 程序执行到此函数调用的时间长度, 以毫秒计.
		*/
		inline static e_long clock() {return ::clock();}
	
		/**
			\brief 程序退出.
			
			@param code 程序退出值.
		*/
		inline static e_void exit(e_int code){
			throw ExitException(code);
		}
		
		/**
		 * \brief 运行程序.
		 * 
		 * 此方法用于 Win32 GUI 应用, 如果用于 CLI 应用, 会在显示输出时出现不理想行为.
		 * 
		 * @param cmd 程序命令.
		 * @throw 如果程序建立失败, 抛出 ProcessCreateException.
		 * */
		static e_void exec(const String& cmd);
		
		
		/**
		 * \brief 运行程序.
		 * 
		 * 此方法用于 CLI 应用, 如果用于 GUI 应用, 方法会打开一个 Console.
		 * 
		 * @param cmd 程序命令.
		 * @throw 如果程序建立失败, 抛出 ProcessCreateException.
		 * */
		static e_void shell(const String& cmd);
		

		/**
		 * \brief 获取数据类型大小.
		 * 
		 * @return 数据类型大小, 以字节计.
		 * 
		*/
		template<class T>
		constexpr inline static e_long sizeOf() {return (e_long)sizeof(T);}
		
		/**
			\brief move 对象.
			
			@param t 被 move 的对象.
			@return  move t 得到的副本.
		*/
		template<class T>
		inline static T&& move(T& t) {return (T&&)t;}
		
		/**
			\brief move 对象.
			
			@param t 被 move 的对象.
			@return move t 得到的副本.
		*/
		template<class T>
		inline static T&& move(T&& t) {return (T&&)t;}
		
		/**
		* \brief beep.
		* 
		* @param freq 频率.
		* @param duration 持续时间.
		* */
		inline static e_void beep(e_int freq, e_int duration) {
			::Beep(freq, duration);
		}
	
	private:
		System() =delete;
		~System() =delete;
		System(System& copy) =delete;
		System& operator=(System& copy) =delete;
		System(System&& move) =delete;
		System& operator=(System&& move) =delete;
};
	
} // Easy


#endif // LEC_SYSTEM_H_