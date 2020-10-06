
/*
 * File name: Console.hpp 
 *
 * Date/Time:
 *		start: 2019年10月03日 15时57分53秒 周4
 *		end:
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_CONSOLE_H_
#define LEC_CONSOLE_H_

#include <iostream>
#include <cstdio>

#include "BaseTypes.hpp"
#include "String.hpp"

namespace Easy {
	
/**
	\class Console
	
	\brief Standard input/output.
	
	@author Tes Alone
	@since 1.0
*/
	
class Console {
	public:
		/**
		 * \brief Console color flag.
		 * 
		 * Background red.
		 * */
		static const e_int BK_RED   = BACKGROUND_RED;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Background green.
		 * */
		static const e_int BK_GREEN = BACKGROUND_GREEN;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Background blue.
		 * */
		static const e_int BK_BLUE  = BACKGROUND_BLUE;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Background high light.
		 * */
		static const e_int BK_HIGHLIGHT = BACKGROUND_INTENSITY;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Background white.
		 * */
		static const e_int BK_WHITE = BK_RED | BK_GREEN | BK_BLUE;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Background black.
		 * */
		static const e_int BK_BLACK = 0;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Foreground red.
		 * */
		static const e_int FR_RED   = FOREGROUND_RED;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Foreground green.
		 * */
		static const e_int FR_GREEN = FOREGROUND_GREEN;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Foreground blue.
		 * */
		static const e_int FR_BLUE  = FOREGROUND_BLUE;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Foreground high light.
		 * */
		static const e_int FR_HIGHLIGHT = FOREGROUND_INTENSITY;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Foreground white.
		 * */
		static const e_int FR_WHITE = FR_RED | FR_GREEN | FR_BLUE;
		
		/**
		 * \brief Console color flag.
		 * 
		 * Foreground black.
		 * */
		static const e_int FR_BLACK = 0;
		
		/**
			\brief Output an e_ubyte.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_ubyte num)
		{
			std::printf("%" PRIu8 "\n", num);
		}
		
		/**
			\brief Output an e_ubyte.
			
			@param num 		
		*/
		inline static e_void out(e_ubyte num)
		{
			std::printf("%" PRIu8 , num);
		}
		
		/**
			\brief Output an e_ubyte.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_ubyte num)
		{
			fprintf(stderr, "%d" PRIu8 "\n", num);
		}
		
		/**
			\brief Output an e_ubyte.
			
			@param num 		
		*/
		inline static e_void err(e_ubyte num)
		{
			fprintf(stderr, "%d" PRIu8, num);
		}
		
		/**
			\brief Output an e_byte.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_byte num)
		{
			std::printf("%" PRId8 "\n", num);
		}
		
		/**
			\brief Output an e_byte.
			
			@param num 		
		*/
		inline static e_void out(e_byte num)
		{
			std::printf("%" PRId8 , num);
		}
		
		/**
			\brief Output an e_byte.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_byte num)
		{
			fprintf(stderr, "%d" PRIu8 "\n", num);
		}
		
		/**
			\brief Output an e_byte.
			
			@param num 		
		*/
		inline static e_void err(e_byte num)
		{
			fprintf(stderr, "%d" PRIu8, num);
		}
		
		/**
			\brief Output an e_short.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_short num)
		{
			std::cout << num << std::endl;
		}
		
		/**
			\brief Output an e_short.
			
			@param num 		
		*/
		inline static e_void out(e_short num)
		{
			std::cout << num;
		}
		
		/**
			\brief Output an e_short.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_short num)
		{
			std::cerr << num << std::endl;
		}
		
		/**
			\brief Output an e_short.
			
			@param num 		
		*/
		inline static e_void err(e_short num)
		{
			std::cerr << num;
		}
		
/**
			\brief Output an e_ushort.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_ushort num)
		{
			std::cout << num << std::endl;
		}
		
		/**
			\brief Output an e_ushort.
			
			@param num 		
		*/
		inline static e_void out(e_ushort num)
		{
			std::cout << num;
		}
		
		/**
			\brief Output an e_ushort.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_ushort num)
		{
			std::cerr << num << std::endl;
		}
		
		/**
			\brief Output an e_ushort.
			
			@param num 		
		*/
		inline static e_void err(e_ushort num)
		{
			std::cerr << num;
		}
		
		/**
			\brief Output an e_int.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_int num)
		{
			std::cout << num << std::endl;
		}
		
		/**
			\brief Output an e_int.
			
			@param num 		
		*/
		inline static e_void out(e_int num)
		{
			std::cout << num;
		}
		
		/**
			\brief Output an e_int.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_int num)
		{
			std::cerr << num << std::endl;
		}
		
		/**
			\brief Output an e_int.
			
			@param num 		
		*/
		inline static e_void err(e_int num)
		{
			std::cerr << num;
		}
		
		/**
			\brief Output an e_uint.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_uint num)
		{
			std::cout << num << std::endl;
		}
		
		/**
			\brief Output an e_uint.
			
			@param num 		
		*/
		inline static e_void out(e_uint num)
		{
			std::cout << num;
		}
		
		/**
			\brief Output an e_uint.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_uint num)
		{
			std::cerr << num << std::endl;
		}
		
		/**
			\brief Output an e_uint.
			
			@param num 		
		*/
		inline static e_void err(e_uint num)
		{
			std::cerr << num;
		}
		
		/**
			\brief Output an e_long.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_long num)
		{
			std::cout << num << std::endl;
		}
		
		/**
			\brief Output an e_long.
			
			@param num 		
		*/
		inline static e_void out(e_long num)
		{
			std::cout << num;
		}
		
		/**
			\brief Output an e_long.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_long num)
		{
			std::cerr << num << std::endl;
		}
		
		/**
			\brief Output an e_long.
			
			@param num 		
		*/
		inline static e_void err(e_long num)
		{
			std::cerr << num;
		}
		
		/**
			\brief Output an e_ulong.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void outln(e_ulong num)
		{
			std::cout << num << std::endl;
		}
		
		/**
			\brief Output an e_ulong.
			
			@param num 		
		*/
		inline static e_void out(e_ulong num)
		{
			std::cout << num;
		}
		
		/**
			\brief Output an e_ulong.
			
			Output num and a new line.
			
			@param num 		
		*/
		inline static e_void errln(e_ulong num)
		{
			std::cerr << num << std::endl;
		}
		
		/**
			\brief Output an e_ulong.
			
			@param num 		
		*/
		inline static e_void err(e_ulong num)
		{
			std::cerr << num;
		}
		
		/**
			\brief Output an e_char.
			
			Output c and a new line.
			
			@param c 		
		*/
		inline static e_void outln(e_char c)
		{
			const e_char tmp[2] = {c, '\0'};
			Console::outln(String{tmp});
		}
		
		/**
			\brief Output an e_char.
			
			@param c 		
		*/
		inline static e_void out(e_char c)
		{
			const e_char tmp[2] = {c, '\0'};
			Console::out(String{tmp});
		}
		
		/**
			\brief Output an e_char.
			
			Output c and a new line.
			
			@param c 		
		*/
		inline static e_void errln(e_char c)
		{
			const e_char tmp[2] = {c, '\0'};
			Console::errln(String{tmp});
		}
		
		/**
			\brief Output an e_char.
			
			@param c 		
		*/
		inline static e_void err(e_char c)
		{
			const e_char tmp[2] = {c, '\0'};
			Console::err(String{tmp});
		}
		
		/**
			\brief Output an e_float.
			
			Output f and a new line.
			
			@param f 		
		*/
		inline static e_void outln(e_float f)
		{
			std::cout << f << std::endl;
		}
		
		/**
			\brief Output an e_float.
			
			@param f 		
		*/
		inline static e_void out(e_float f)
		{
			std::cout << f;
		}
		
		/**
			\brief Output an e_float.
			
			Output f and a new line.
			
			@param f		
		*/
		inline static e_void errln(e_float f)
		{
			std::cerr << f << std::endl;
		}
		
		/**
			\brief Output an e_float.
			
			@param f	
		*/
		inline static e_void err(e_float f)
		{
			std::cerr << f;
		}
		
		/**
			\brief Output an e_double.
			
			Output f and a new line.
			
			@param d		
		*/
		inline static e_void outln(e_double d)
		{
			std::cout << d << std::endl;
		}
		
		/**
			\brief Output an e_double.
			
			@param d		
		*/
		inline static e_void out(e_double d)
		{
			std::cout << d;
		}
		
		/**
			\brief Output an e_double.
			
			Output d and a new line.
			
			@param d	
		*/
		inline static e_void errln(e_double d)
		{
			std::cerr << d << std::endl;
		}
		
		/**
			\brief Output an e_double.
			
			@param d 		
		*/
		inline static e_void err(e_double d)
		{
			std::cerr << d;
		}
		
		/**
			\brief Output an e_bool.
			
			Output b and a new line.
			
			@param b		
		*/
		inline static e_void outln(e_bool b)
		{
			std::cout << (b?"true":"false") << std::endl;
		}
		
		/**
			\brief Output an e_bool.
			
			@param b		
		*/
		inline static e_void out(e_bool b)
		{
			std::cout << (b?"true":"false");
		}
		
		/**
			\brief Output an e_bool.
			
			Output b and a new line.
			
			@param b 		
		*/
		inline static e_void errln(e_bool b)
		{
			std::cerr << (b?"true":"false") << std::endl;
		}
		
		/**
			\brief Output an e_bool.
			
			@param b		
		*/
		inline static e_void err(e_bool b)
		{
			std::cerr << (b?"true":"false");
		}
		
		/**
		 * \brief Output a C-like string.
		 * 
		 * Output a C-like string and a new line.
		 * 
		 * @param str 
		*/
		inline static e_void outln(const e_byte* str) {
			std::cout << str << std::endl;
		}
		
		/**
		 * \brief Output a C-like string.
		 * 
		 * Output a C-like string and a new line.
		 * 
		 * @param str 
		*/
		inline static e_void errln(const e_byte* str) {
			std::cerr << str << std::endl;
		}
		
		/**
		 * \brief Output a C-like string.
		 * 
		 * @param str 
		*/
		inline static e_void out(const e_byte* str) {
			std::cout << str;
		}
		
		/**
		 * \brief Output a C-like string.
		 * 
		 * @param str 
		*/
		inline static e_void err(const e_byte* str) {
			std::cerr << str;
		}
		
		/**
		 * \brief Output a C-like wide string.
		 * 
		 * Output a C-like wide string and a new line.
		 * 
		 * @param str 
		*/
		inline static e_void outln(const e_char* str)
		{
			out(str);
			std::cout << std::endl;
		}
		
		/**
		 * \brief Output a C-like wide string.
		 * 
		 * Output a C-like wide string and a new line.
		 * 
		 * @param str 
		*/
		inline static e_void errln(const e_char* str)
		{
			err(str);
			std::cerr << std::endl;
		}
		
		/**
		 * \brief Output a C-like wide string.
		 * 
		 * @param str 
		*/
		static e_void out(const e_char* str);
		
		/**
		 * \brief Output a C-like wide string.
		 * 
		 * @param str 
		*/
		static e_void err(const e_char* str);
		
		/**
		 * \brief Output an Easy string.
		 * 
		 * Output an Easy string and a new line.
		 * 
		 * @param str 
		*/
		inline static e_void outln(const String& str)
		{
			out(str);
			std::cout << std::endl;
		}
		
		/**
		 * \brief Output an Easy string.
		 * 
		 * Output an Easy string and a new line.
		 * 
		 * @param str 
		*/
		inline static e_void errln(const String& str)
		{
			err(str);
			std::cout << std::endl;
		}
		
		/**
		 * \brief Output an Easy string.
		 * 
		 * @param str 
		*/
		static e_void out(const String& str);
		
		/**
		 * \brief Output an Easy string.
		 * 
		 * @param str 
		*/
		static e_void err(const String& str);
		
		/**
			\brief Output an object.

			Output an object and a new line.
			<br/>
			Class T needs implement toString().
			
			@param t .
		*/
		template <class T>
		static e_void outln(const T& t)
		{
			Console::outln(t.toString());
		}
		
		/**
			\brief Output an object.

			Class T needs implement toString().
			
			@param t .
		*/
		template <class T>
		static e_void out(const T& t)
		{
			Console::out(t.toString());
		}
		
		/**
			\brief Output an object.

			Output an object and a new line.
			<br/>
			Class T needs implement toString().
			
			@param t .
		*/
		template <class T>
		static e_void errln(const T& t)
		{
			Console::errln(t.toString());
		}
		
		/**
			\brief Output an object.
			
			Class T needs implement toString().
			
			@param t .
		*/
		template <class T>
		static e_void err(const T& t)
		{
			Console::err(t.toString());
		}

		/**
		 * \brief Output a new line.
		 * 
		 * <b>Note: </b> There is not an errln().
		*/
		
		inline static e_void outln()
		{
			std::cout << std::endl;
		}
		
		/**
		 * \brief Output formated string.
		 * 
		 * Output a formated string and a new line.
		 * 
		 * @param fmt 
		 * @param args
		 * 
		 * @sa String::format()
		 * */
		template<class ...T>
		inline static e_void printfln(const String& fmt, const T& ...args) {
			outln(String::format(fmt, args...));
		}
		
		/**
		 * \brief Output formated string.
		 * 
		 * Output a formated string and a new line.
		 * 
		 * @param fmt 
		 * @param args
		 * 
		 * @sa String::format()
		 * */
		template<class ...T>
		inline static e_void printf(const String& fmt, const T& ...args) {
			out(String::format(fmt, args...));
		}
		
		/**
			\brief Get input.
			
			<b>Note: </b> The method never throw a IOException.
			
			@return input.
		*/
		static String input();
		
		/**
		 * \brief Get current console color.
		 * 
		 * The color is an or flags combining. The flags are defined in this class.
		 * 
		 * @return color.
		 * */
		static e_int getColor();
		
		/**
		 * \brief Set current console color.
		 * 
		 * The setting only affects the next output.
		 * 
		 * @param color an or flags combining, flags are defined in this class.
		 * */
		static e_void setColor(e_int color);
		
	private:
		Console() =delete;
		~Console() =delete;
		Console(Console& copy) =delete;
		Console& operator=(Console& copy) =delete;
		Console(Console&& move)  =delete;
		Console& operator=(Console&& move)  =delete;
};
	
}


#endif // LEC_CONSOLE_H_