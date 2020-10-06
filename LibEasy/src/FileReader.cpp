/*
 * File Name:	FileReader.cpp
 * Date/Time:
 * 	Start: 	2019-11-27 18:21 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <windows.h>

#include "FileReader.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include "StringBuffer.hpp"
#include "Character.hpp"

namespace Easy {

FileReader::FileReader(const File& file) {
	m_filePath = file.getFullPath();
	if (m_filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_file = _wfopen(m_filePath.toCStr(), L"rt, ccs=UTF-8");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
//	if (_wfopen_s(&m_file, m_filePath.toCStr(), L"rt, ccs=UTF-8")) {
//		throw  FileOpenFailedException{m_filePath};
//	}
}

FileReader::FileReader(const String& filePath) {
	if (filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_filePath = filePath;
	m_file = _wfopen(m_filePath.toCStr(), L"rt, ccs=UTF-8");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
//	if (_wfopen_s(&m_file, filePath.toCStr(), L"rt, ccs=UTF-8")) {
//		throw  FileOpenFailedException{m_filePath};
//	}
}

e_int FileReader::read() {
    e_int check = fgetwc(m_file);
    if (check==WEOF && ferror(m_file))
        throw  IOException{__func__, __LINE__, m_filePath};
    return check;
}

e_int FileReader::read(CharArray& buf) {
    e_int check = fread(buf.toCArray(), 2, buf.length(), m_file);
    if (check<buf.length() && ferror(m_file)) {
        throw  IOException{__func__, __LINE__, m_filePath};
    }
    return check;
}

String FileReader::readLine() {
	StringBuffer line;
	e_int c = this->read();
	
	if (c == this->eof())
		throw  FileReachEOFException{__func__, __LINE__, m_filePath};
	
	while (c!='\n' && c!=this->eof()) {
		line.append(Character{(e_char)c});
		c = this->read();
	}
	return System::move(line.toString());
}

e_void FileReader::close() {
	if (!m_closed) {
		e_int check = fclose(m_file);
		if (check == EOF) {
			m_closed = true; // for close failed.
			throw  FileCloseFailedException{__func__, __LINE__, m_filePath};
		}
		m_closed = true;
	}
}

String FileReader::toString() const {
	return System::move(String{m_filePath});
}

} // Easy