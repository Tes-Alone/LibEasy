/*
 * File Name:	FileWriter.cpp
 * Date/Time:
 * 	Start: 	2019-11-27 18:31 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <windows.h>

#include "FileWriter.hpp"
#include "String.hpp"

namespace Easy {

FileWriter::FileWriter(const File& file) {
	m_filePath = file.getFullPath();
	if (m_filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_file = _wfopen(m_filePath.toCStr(), L"wt, ccs=UTF-8");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
	
//	m_filePath = file.getFullPath();
//    if (_wfopen_s(&m_file, m_filePath.toCStr(), L"wt, ccs=UTF-8")) {
//        throw  FileOpenFailedException{m_filePath};
//    }
}

FileWriter::FileWriter(const String& filePath) {
	if (filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_filePath = filePath;
	m_file = _wfopen(m_filePath.toCStr(), L"wt, ccs=UTF-8");
	if (m_file == nullptr) {
		throw  FileOpenFailedException{__func__, __LINE__, m_filePath};
	}
	
//	m_filePath = filePath;
//    if (_wfopen_s(&m_file, filePath.toCStr(), L"wt, ccs=UTF-8")) {
//        throw  FileOpenFailedException{m_filePath};
//    }
}

e_void FileWriter::write(e_int c) {
    e_int check = fputwc((e_char)c, m_file);
    if (check == WEOF) throw  IOException{__func__, __LINE__, m_filePath};
}

e_void FileWriter::write(const CharArray& buf, e_int length) {
    if (length == -1) length = buf.length();
	if (length<0 || length>buf.length())
		throw  IllegalArgumentException{__func__, __LINE__};
	e_int check = fwrite(buf.toCArray(), 2, length, m_file);
    if (check < length) {
        throw  IOException{__func__, __LINE__, m_filePath};
    }
}

e_void FileWriter::write(const String& str, e_int length) {
    if (length == -1) length = str.length();
	if (length<0 || length>str.length())
		throw IllegalArgumentException{__func__, __LINE__};
	e_int check = fwrite(str.toCStr(), 2, length, m_file);
    if (check < length) {
        throw  IOException{__func__, __LINE__, m_filePath};
    }
}

e_void FileWriter::close() {
	if (!m_closed) {
		e_int check = fclose(m_file);
		if (check == EOF) {
			m_closed = true;
			throw  FileCloseFailedException{__func__, __LINE__, m_filePath};
		}
		m_closed = true;
	}
}

e_void FileWriter::flush() {
	e_int check = ::fflush(m_file);
    if (check == EOF) {
        throw  IOException{__func__, __LINE__, m_filePath};
    }
}

String FileWriter::toString() const {
	return System::move(String{m_filePath});
}

/*
e_void FileWriter::write(e_int c) {
	e_int check = 0;

	if (c>0 && c<0x80)
    {
        check = fputc(c, m_file);
		if (check == EOF) {
			throw new  IOException;
		}
		return;
    }

    if (c>0x80 && c<0x7FF)
    {
        check = fputc(c>>8, m_file);
		if (check == EOF) {
			throw new  IOException;
		}
        check = fputc(c&0x00FF, m_file);
		if (check == EOF) {
			throw new  IOException;
		}
		return;
    }

    if (c>0x800 && c<0x10000)
    {
        e_int tmp = 0xE0 + ((c&0xF000)>>12);

        check = fputc(tmp, m_file);
		
		if (check == EOF) {
			throw new  IOException;
		}

        tmp = 0x02;
        tmp = tmp << 2;
        tmp = tmp + ((c&0x0C00)>>10);
        tmp = tmp << 2;
        tmp = tmp + ((c&0x0300)>>8);
        tmp = tmp << 2;
        tmp = tmp + ((c&0x00C0)>>6);
        check = fputc(tmp, m_file);
		
		if (check == EOF) {
			throw new  IOException;
		}

        tmp = 0x02;
        tmp = tmp << 2;
        tmp = tmp + ((c&0x0030)>>4);
        tmp = tmp << 4;
        tmp = tmp + (c&0x000F);
        check = fputc(tmp, m_file);

		if (check == EOF) {
			throw new  IOException;
		}

        return ;
    }

    return ; //arrive here when c > 0x10000
}
*/

} // Easy