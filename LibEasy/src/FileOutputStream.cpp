/*
 * File Name:	FileOutputStream.cpp
 * Date/Time:
 * 	Start: 	2019-11-27 18:11 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <windows.h>

#include "String.hpp"
#include "FileOutputStream.hpp"

namespace Easy {

FileOutputStream::FileOutputStream(const File& file) {
	m_filePath = file.getFullPath();
	if (m_filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_file = _wfopen(m_filePath.toCStr(), L"wb");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
	
//	m_filePath = file.getFullPath();
//	if (_wfopen_s(&m_file, m_filePath.toCStr(), L"wb")) {
//		throw  FileOpenFailedException{m_filePath};
//	}
}

FileOutputStream::FileOutputStream(const String& filePath) {
	if (filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_filePath = filePath;
	m_file = _wfopen(m_filePath.toCStr(), L"wb");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
	
//	if (_wfopen_s(&m_file, filePath.toCStr(), L"wb")) {
//		throw  FileOpenFailedException{filePath};
//	}
}

e_void FileOutputStream::write(e_int c) {
	e_int check = fputc(c, m_file);
	if (check == EOF) {
		throw  IOException{__func__, __LINE__, m_filePath};
	}
}

e_void FileOutputStream::write(const UByteArray& buf, e_int  length) {
	if (length == -1) length = buf.length();
	if (length<0 || length>buf.length())
		throw IllegalArgumentException{__func__, __LINE__};
	e_int check = fwrite(buf.toCArray(), 1, length, m_file);
	if (check < length) {
		throw  IOException{__func__, __LINE__, m_filePath};
	}
}

e_void FileOutputStream::flush() {
	e_int check = ::fflush(m_file);
	if (check == EOF) {
		throw  IOException{__func__, __LINE__, m_filePath};
	}
}

e_void FileOutputStream::close() {
	if (!m_closed) {
		e_int check = fclose(m_file);
		if (check == EOF) {
			m_closed = true; // this stmt, if throw a exception, receiver will dtor, may issue some error.
			throw  FileCloseFailedException{__func__, __LINE__, m_filePath};
		}
		m_closed = true;
	}
}

String FileOutputStream::toString() const {
	return System::move(String{m_filePath});
}

} // Easy