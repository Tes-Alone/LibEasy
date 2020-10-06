
/*
 * File name: Console.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月03日 16时02分52秒 周4
 * End Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include <windows.h>
#include <cstdio>

#include "Console.hpp"
#include "Exception.hpp"
#include "BasicArray.hpp"
#include "StringBuffer.hpp"

#include "Integer.hpp"

namespace Easy {

e_void Console::out(const String& str)
{
	using namespace std;
	e_int len = WideCharToMultiByte(CP_ACP, 0, str.toCStr(), -1, nullptr, 0, nullptr, nullptr);
	ByteArray pMulStr{len};
	WideCharToMultiByte(CP_ACP, 0, str.toCStr(), -1, (char*)pMulStr.toCArray(), len, nullptr, nullptr);
	std::cout << pMulStr.toCArray();
}

e_void Console::out(const e_char* str)
{
	using namespace std;
	e_int len = WideCharToMultiByte(CP_ACP, 0, str, -1, nullptr, 0, nullptr, nullptr);
	ByteArray pMulStr{len};
	WideCharToMultiByte(CP_ACP, 0, str, -1,(char*) pMulStr.toCArray(), len, nullptr, nullptr);
	std::cout << pMulStr.toCArray();
}

e_void Console::err(const String& str)
{
	using namespace std;
	e_int len = WideCharToMultiByte(CP_ACP, 0, str.toCStr(), -1, nullptr, 0, nullptr, nullptr);
	ByteArray pMulStr{len};
	WideCharToMultiByte(CP_ACP, 0, str.toCStr(), -1, (char*)pMulStr.toCArray(), len, nullptr, nullptr);
	std::cerr << pMulStr.toCArray();
}

e_void Console::err(const e_char* str)
{
	using namespace std;
	e_int len = WideCharToMultiByte(CP_ACP, 0, str, -1, nullptr, 0, nullptr, nullptr);
	ByteArray pMulStr{len};
	WideCharToMultiByte(CP_ACP, 0, str, -1, (char*)pMulStr.toCArray(), len, nullptr, nullptr);
	std::cerr << pMulStr.toCArray();
}

#define TWO 0xC0
#define THREE 0xE0

static e_int _byteCount(e_int c) {
	if (c>0 && c<0x80)
        return 1;

    if ((c&THREE) == THREE)
        return 3;

    if ((c&TWO) == TWO)
        return 2;
   return 0; //never arrive here
}

static e_int _nextChar() {
	e_int c = getchar();

    if (c == EOF)
        return -1;

    e_int _bc = _byteCount(c);

    if (_bc == 1) {
		return c;
	} else if (_bc == 2) {
		c = (c << 8) + getchar(); // do nothing.
		return c;
	} else if (_bc == 3) {
		e_int c2  = getchar();
        e_int c3  = getchar();
		e_int tmp = 0;
        c 		  = (c & 0x0F) << 4;
        c         =	(c + ((c2 & 0x3C)>>2)) << 8;
        tmp 	  = (c2 & 0x03) << 2;
        tmp       =	(tmp + ((c3 & 0x30)>>4)) << 4;
        c         = (tmp + (c3 & 0x0F)) + c;
		return c;
	}
	
	return -1; //never arrive here.
}

String Console::input() {
	StringBuffer sb;
	e_int c = _nextChar();
	while (c!='\n' && c!='\r' && c!=-1) {
		sb.append(Character{(e_char)c});
		c = _nextChar();
	}
	
	return sb.toString();
}

e_void Console::setColor(e_int color) {
	HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (h != nullptr) {
		::SetConsoleTextAttribute(h, color);
	}
}

e_int Console::getColor() {
	HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (h != nullptr) {
		::GetConsoleScreenBufferInfo(h, &info);
		return info.wAttributes;
	}
	return 0;
}

} // namespace Easy