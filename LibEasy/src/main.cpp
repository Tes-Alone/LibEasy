
/*
 * File name: main.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月03日 19时14分55秒 周4
 * End Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include <windows.h>

#include <cstdlib>

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include "Console.hpp"
#include "System.hpp"
#include "Array.hpp"
#include "BasicArray.hpp"

using namespace Easy;
using namespace std;

e_int e_main(Array<String>& args);
CharArray toWideChars(const char* chs);

int main(int argc, char** argv)
{
	Array<String> args{argc};
	
	for (e_int i=0; i<argc; i++)
	{
		CharArray wide = toWideChars(argv[i]);
		args.setAt(i, String{wide.toCArray()});
	}
	
	SYSTEMTIME st;
    GetSystemTime(&st);

    ::srand(st.wMilliseconds);
	
	e_int result = 0;
	
	try {
		result = e_main(args);
	} catch (ExitException& e) {
		result = e.getExitCode();
	} catch (Exception& e) {
		Console::outln(String{L"Easy exception arise, what() is: "} + e.what());
		Console::outln(String{L"Function is: "} + e.getFunction());
		Console::outln(String{L"At line: "} + Integer{e.getLine()});
		Console::outln(String{L"Message is: "} + e.getMessage());
	} catch (exception& stde) {
		std::cout << "C++ build-in exception arise, what() is: ";
		std::cout << stde.what() << std::endl;
	}
	
	return result;
}

CharArray toWideChars(const char* chs)
{
	e_int len = MultiByteToWideChar(CP_ACP, 0, chs, -1, nullptr, 0);
	CharArray pWideStr{len};
	MultiByteToWideChar(CP_ACP, 0, chs, -1, pWideStr.toCArray(), len);
	return System::move(pWideStr);
}