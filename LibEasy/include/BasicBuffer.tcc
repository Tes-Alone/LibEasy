
#ifndef _EASY_BASICBUFFER_TCC_H_
#define _EASY_BASICBUFFER_TCC_H_

#include "BasicBuffer.hpp"
#include "StringBuffer.hpp"
#include "System.hpp"

namespace Easy {
	
template<class E>
String BasicBuffer<E>::toString() const {
	StringBuffer sb{};
	sb.append(String{L"["});
	for (e_int i=0; i<m_length; i++) {
		sb.append(toString(m_buffer[i]));
		sb.append(String{L","});
	}
	if (sb.length() > 1) {
		return System::move(sb.removeTail(sb.length()-1).append(String{L"]"}).toString());
	}
	return System::move(sb.append(String{L"]"}).toString());
}

	
} // Easy

#endif //_EASY_BASICBUFFER_TCC_H_