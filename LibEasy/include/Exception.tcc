#ifndef _EASY_EXCEPTION_TCC_
#define _EASY_EXCEPTION_TCC_

#include "String.hpp"

namespace Easy {

template<class X>
NotInFunctionDomainException::NotInFunctionDomainException( 
						const String& func, e_int line, X x) 
	: Exception(String{L"Argument '"} + x + String{L"' is not in function "} 
			+ func + String{L"'s Domain."}, func, line) {}

} // Easy

#endif // _EASY_EXCEPTION_TCC_