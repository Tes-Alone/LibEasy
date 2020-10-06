/*
 * File Name:	FileInputStream.cpp
 * Date/Time:
 * 	Start: 	2019-11-27 17:56 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <windows.h>

#include "FileInputStream.hpp"
#include "Exception.hpp"
#include "String.hpp"

namespace Easy {

FileInputStream::FileInputStream(const File& file) {
	m_filePath = file.getFullPath();
	if (m_filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_file = _wfopen(m_filePath.toCStr(), L"rb");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
//	if (_wfopen_s(&m_file, m_filePath.toCStr(), L"rb")) {
//		throw  FileOpenFailedException{m_filePath};
//	}
}

FileInputStream::FileInputStream(const String& filePath) {
	if (filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_filePath = filePath;
	m_file = _wfopen(m_filePath.toCStr(), L"rb");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
//	if (_wfopen_s(&m_file, filePath.toCStr(), L"rb")) {
//		throw  FileOpenFailedException{filePath};
//	}
}

e_bool FileInputStream::seek(e_long pos) {
	if (pos < 0) 
		throw IllegalArgumentException{__func__, __LINE__};
	return _fseeki64(m_file, pos, SEEK_SET) == 0;
}

e_int FileInputStream::read() {
	e_int check = fgetc(m_file);
	if (check==EOF && ferror(m_file)) {
		throw  IOException{__func__, __LINE__, m_filePath};
	}
	return check;
}

e_int FileInputStream::read(UByteArray& buf) {
	e_int check = fread(buf.toCArray(), 1, buf.length(), m_file);
	if (check<buf.length() && ferror(m_file)) {
		throw  IOException{__func__, __LINE__, m_filePath};
	}
	return check;
}

e_void FileInputStream::close() {
	if (!m_closed) {
		e_int check = fclose(m_file);
		if (check == EOF) {
			m_closed = true; // this stmt, if throw a exception, receiver will dtor, may issue some error.
			throw  FileCloseFailedException{__func__, __LINE__, m_filePath};
		}
		m_closed = true;
	}
}

String FileInputStream::toString() const {
	return System::move(String{m_filePath});
}

} // Easy