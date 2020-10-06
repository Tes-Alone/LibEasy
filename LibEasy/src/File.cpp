/*
 * File Name:	File.cpp
 * Date/Time:
 * 	Start: 	2019-11-29 16:58 week:5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <windows.h>
#include <accctrl.h>
#include <aclapi.h>

#include "File.hpp"
#include "Vector.hpp"
#include "BasicArray.hpp"
#include "System.hpp"

//#include "Console.hpp"

namespace Easy {

File::File(const String& filePath) {
	if (filePath.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	String tmp;
	if (filePath.length()==2 && filePath.endsWith(L":")) {
		tmp = filePath + L"\\";
	} else {
		tmp = filePath;
	}
	e_int len = GetFullPathNameW(tmp.toCStr(), 0, nullptr, nullptr);
	CharArray pathBuf{len};
    GetFullPathNameW(tmp.toCStr(), len, pathBuf.toCArray(), nullptr);
    m_fullPath = String{pathBuf.toCArray()};
    if (m_fullPath.length()>3 && m_fullPath.endsWith(L"\\")) {
    	m_fullPath = m_fullPath.substring(0, m_fullPath.length()-1);
    }
}

e_bool File::isHidden() const {
	WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW(((m_fullPath)).toCStr(), GetFileExInfoStandard, &attrData);
	return exist && ((attrData.dwFileAttributes&0x2)==0x2);
}

e_bool File::isDirectory() const {
	WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW(((m_fullPath)).toCStr(), GetFileExInfoStandard, &attrData);
	return exist && ((attrData.dwFileAttributes&0x10)==0x10);
}

e_bool File::isReadOnly() const {
	WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW(((m_fullPath)).toCStr(), GetFileExInfoStandard, &attrData);
	return exist && ((attrData.dwFileAttributes&0x1)==0x1);
}

e_bool File::exists() const {
	WIN32_FILE_ATTRIBUTE_DATA  attrData;
    return GetFileAttributesExW((m_fullPath).toCStr(), GetFileExInfoStandard, &attrData);
}

String File::getFullPath() const {
	return System::move(String{m_fullPath});
}

String File::getName() const {
	e_int tail = (m_fullPath).length()-1;
    for (e_int i=tail; i>=0; i--)
        if (i!=tail && (m_fullPath)[i]=='\\')
            return System::move(String{(m_fullPath).substring(i+1, (m_fullPath).length())});
    return System::move(String{(m_fullPath)});
}

File File::getParent() const {
	if (m_fullPath.length()==3 && m_fullPath.endsWith(L":\\")) {
		throw FileHasNoParentException{__func__, __LINE__, m_fullPath};
	}
	e_int tail = (m_fullPath).length()-1;
    for (e_int i=tail; i>=0; i--) {
        if (i!=tail && (m_fullPath)[i]=='\\') {
			return System::move(File{(m_fullPath).substring(0, i)});
		}
	}
	throw FileHasNoParentException{__func__, __LINE__, m_fullPath}; // never arrive here
}

e_long File::getSize() const {
	WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW((m_fullPath).toCStr(), GetFileExInfoStandard, &attrData);
	if (exist) {
		e_long tmp = (e_long) attrData.nFileSizeHigh;
		return (tmp << 32) | attrData.nFileSizeLow;
	} else {
		return 0;
	}
}

e_bool _canAccess(const e_char* folderName, DWORD genericAccessRights);

e_bool File::readable() const {
	return exists() && _canAccess((m_fullPath).toCStr(), FILE_GENERIC_READ);
}

e_bool File::writable() const {
	return exists() && _canAccess((m_fullPath).toCStr(), FILE_GENERIC_WRITE);
}

e_bool File::executable() const {
	if (exists() && _canAccess((m_fullPath).toCStr(), FILE_GENERIC_EXECUTE)) {
		DWORD type = 0;
		return GetBinaryTypeW((m_fullPath).toCStr(), &type) || this->isDirectory();
	} else {
		return false;
	}
}

Date File::getCreateDate() const {
    WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW((m_fullPath).toCStr(), GetFileExInfoStandard, &attrData);
	if (exist) {
		SYSTEMTIME st;
		SYSTEMTIME local;
		FileTimeToSystemTime(&attrData.ftCreationTime, &st);
		SystemTimeToTzSpecificLocalTime(nullptr, &st, &local);
		return _createDate(&local);
	} else {
		return Date{};
	}
}

Date File::getLastAccessDate() const {
    WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW((m_fullPath).toCStr(), GetFileExInfoStandard, &attrData);
	if (exist) {
		SYSTEMTIME st;
		SYSTEMTIME local;
		FileTimeToSystemTime(&attrData.ftLastAccessTime, &st);
		SystemTimeToTzSpecificLocalTime(nullptr, &st, &local);
		return _createDate(&local);
	} else {
		return Date{};
	}
}

Date File::getLastModifyDate() const {
    WIN32_FILE_ATTRIBUTE_DATA  attrData;
    e_int exist = GetFileAttributesExW((m_fullPath).toCStr(), GetFileExInfoStandard, &attrData);
	if (exist) {
		SYSTEMTIME st;
		SYSTEMTIME local;
		FileTimeToSystemTime(&attrData.ftLastWriteTime, &st);
		SystemTimeToTzSpecificLocalTime(nullptr, &st, &local);
		return _createDate(&local);
	} else {
		return Date{};
	}
}

e_bool File::createNewFile() const {
	HANDLE hFile = CreateFileW((m_fullPath).toCStr(), 0, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;
    CloseHandle(hFile);
    return true;
}

e_bool File::mkdir() const {
	return CreateDirectoryW((m_fullPath).toCStr(), nullptr);
}

static e_bool _mkdirs0(const File& file) {
	if (file.exists()) return false;
	_mkdirs0(file.getParent());
	return file.mkdir();
}

e_bool File::mkdirs() const {
	return _mkdirs0(*this);
}

e_bool File::deleteFile() const {
	return DeleteFileW((m_fullPath).toCStr());
}

e_bool File::remove() const {
	return RemoveDirectoryW((m_fullPath).toCStr());
}

Vector<File> File::listAll() const {
    if (exists() && isDirectory()) {
        WIN32_FIND_DATAW data;
        HANDLE h = FindFirstFileW(((m_fullPath)+String{L"\\*"}).toCStr(), &data);
		
		FindNextFileW(h, &data);
		
		Vector<File> files;
        while (FindNextFileW(h, &data)) {
            files.add(File{(m_fullPath) + String{L"\\"} + String{data.cFileName}});
        }

        FindClose(h);
        return System::move(files);
    } else {
        return System::move(Vector<File>{});
    }
}

e_bool File::rename(const String& newName) {
	if (newName.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	String tmp;
	if (newName.length()==2 && newName.endsWith(L":")) {
		tmp = newName + L"\\";
	} else {
		tmp = newName;
	}
	if (MoveFileW((m_fullPath).toCStr(), tmp.toCStr())) {
		e_int len = GetFullPathNameW(tmp.toCStr(), 0, nullptr, nullptr);
		CharArray pathBuf{len};
		GetFullPathNameW(tmp.toCStr(), len, pathBuf.toCArray(), nullptr);
		m_fullPath = String{pathBuf.toCArray()};
		if (m_fullPath.length()>3 && m_fullPath.endsWith(L"\\")) {
	    	m_fullPath = m_fullPath.substring(0, m_fullPath.length()-1);
	    }
		return true;
	} else {
		return false;
	}
}

e_bool File::touch() const {
	if (!exists()) {
		return createNewFile();
	} else {
		HANDLE hFile = CreateFileW((m_fullPath).toCStr(), FILE_WRITE_ATTRIBUTES, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    	
		if (hFile == INVALID_HANDLE_VALUE) {
			return false;
		}
		
		FILETIME ft;
		SYSTEMTIME st;
		e_bool f;
		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &ft); 
		f = SetFileTime(hFile, nullptr, nullptr, &ft);
		CloseHandle(hFile);
		return f;
	}
}

e_bool File::equals(const File& other) const {
	return (m_fullPath).equals(other.m_fullPath);
}

e_int File::compare(const File& other) const {
	if (this->isDirectory() && other.isDirectory()) {
		return (m_fullPath).compare(other.m_fullPath);
	}
	if (this->isDirectory()) return -1;
	if (other.isDirectory()) return 1;
	return (m_fullPath).compare(other.m_fullPath);
}

String File::getOwner() const {
	PSID pSidOwner;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD;
	
// Get the handle of the file object.
	hFile = CreateFileW(
                  (m_fullPath).toCStr(),
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);

// Get the owner SID of the file.
 	GetSecurityInfo(
                  hFile,
                  SE_FILE_OBJECT,
                  OWNER_SECURITY_INFORMATION,
                  &pSidOwner,
                  NULL,
                  NULL,
                  NULL,
                  &pSD);
	
	CloseHandle(hFile);
// First call to LookupAccountSid to get the buffer sizes.
	LookupAccountSidW(
                  NULL,           // local computer
                  pSidOwner,
                  nullptr,
                  (LPDWORD)&dwAcctName,
                  nullptr,
                  (LPDWORD)&dwDomainName,
                  &eUse);

	// Reallocate memory for the buffers.
	CharArray AcctName{(e_int)dwAcctName};
	CharArray DomainName{(e_int)dwDomainName};

    // Second call to LookupAccountSid to get the account name.
    LookupAccountSidW(
          NULL,                   // name of local or remote computer
          pSidOwner,              // security identifier
          AcctName.toCArray(),               // account name buffer
          (LPDWORD)&dwAcctName,   // size of account name buffer 
          DomainName.toCArray(),             // domain name
          (LPDWORD)&dwDomainName, // size of domain name buffer
          &eUse);                 // SID type
	
	String name{AcctName.toCArray()};
	LocalFree(pSD);
    return System::move(name);
}


Date File::_createDate(SYSTEMTIME* pst) const {
	Date date{};
	date.setMilliseconds(pst->wMilliseconds);
    date.setYear(pst->wYear);
    date.setMonth(pst->wMonth);
    date.setDayOfWeek(pst->wDayOfWeek);
    date.setDay(pst->wDay);
    date.setHour(pst->wHour);
    date.setMinute(pst->wMinute);
    date.setSecond(pst->wSecond);
    return date;
}

e_bool _canAccess(const e_char* folderName, DWORD genericAccessRights)
{
    e_bool bRet = false;
    DWORD length = 0;
    if (!::GetFileSecurityW(folderName, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION 
            | DACL_SECURITY_INFORMATION, NULL, 0, &length ) && 
            ERROR_INSUFFICIENT_BUFFER == ::GetLastError()) {
        PSECURITY_DESCRIPTOR security = static_cast< PSECURITY_DESCRIPTOR >( ::malloc( length ) );
        if (security && ::GetFileSecurityW( folderName, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION
                            | DACL_SECURITY_INFORMATION, security, length, &length )) {
            HANDLE hToken = NULL;
            if (::OpenProcessToken( ::GetCurrentProcess(), TOKEN_IMPERSONATE | TOKEN_QUERY | 
                    TOKEN_DUPLICATE | STANDARD_RIGHTS_READ, &hToken )) {
                HANDLE hImpersonatedToken = NULL;
                if (::DuplicateToken( hToken, SecurityImpersonation, &hImpersonatedToken )) {
                    GENERIC_MAPPING mapping = { 0xFFFFFFFF };
                    PRIVILEGE_SET privileges = { 0 };
                    DWORD grantedAccess = 0, privilegesLength = sizeof( privileges );
                    BOOL result = FALSE;
 
                    mapping.GenericRead = FILE_GENERIC_READ;
                    mapping.GenericWrite = FILE_GENERIC_WRITE;
                    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
                    mapping.GenericAll = FILE_ALL_ACCESS;
 
                    ::MapGenericMask( &genericAccessRights, &mapping );
                    if (::AccessCheck( security, hImpersonatedToken, genericAccessRights, 
                            &mapping, &privileges, &privilegesLength, &grantedAccess, &result )) {
                        bRet = (result == TRUE);
                    }
                    ::CloseHandle( hImpersonatedToken );
                }
                ::CloseHandle( hToken );
            }
            ::free( security );
        }
    }
 
    return bRet;
}

} // Easy