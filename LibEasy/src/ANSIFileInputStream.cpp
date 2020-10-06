/*
 * File Name:	ANSIFileInputStream.cpp
 * Date/Time:
 * 	Start: 	2019年12月23日 17:56:27 周一
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <Windows.h>
#include "ANSIFileInputStream.hpp"

namespace Easy {

e_int ANSIFileInputStream::read() {
	uint8_t mbuf[3];
	e_char  wbuf[2];

label:	
	mbuf[0] = (uint8_t)m_in->read();
	
	if (mbuf[0] == (uint8_t)m_in->eof()) 
		return this->eof();
	
	if (mbuf[0] > 127) {
		mbuf[1] = (uint8_t)m_in->read();
		mbuf[2] = '\0';
		MultiByteToWideChar(CP_ACP, 0, (e_byte*)mbuf, 3, wbuf, 2);
		return wbuf[0];
	} else {
		if (mbuf[0] == '\r')
			goto label;
		return mbuf[0];
	}
	return this->eof(); //never arrive here
}

e_int ANSIFileInputStream::read(CharArray& buf) {
	for (e_int i=0; i<buf.length(); i++) {
		e_int c = this->read();
		if (c == this->eof()) {
			return i;
		}
		buf[i] = (e_char)c;
	}
	return buf.length();
}

e_void ANSIFileInputStream::close() {
	m_in->close();
}

String ANSIFileInputStream::toString() const {
	return System::move(m_in->toString());
}

} // Easy