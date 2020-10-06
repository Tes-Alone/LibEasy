
/*
 * File name: StringBuffer.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月07日 18时53分49秒 周1
 * End   Date: 2019年10月08日 15时06分15秒 周2
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include <cstring>

#include "StringBuffer.hpp"
#include "Exception.hpp"
#include "Integer.hpp"
#include "Array.hpp"
#include "System.hpp"

#include "EasyC.hpp"

namespace Easy {
	
/*
	所有的操作必须保证:
		m_strBuf 最少有 16 个空间,
		m_capacity 最少为 16,
		m_length 必须 > 0, 且 < m_capacity.
*/
	
StringBuffer::StringBuffer(e_int capacity) {	
	if (capacity < 16) {
		m_capacity = 16;
	} else {
		m_capacity = capacity;
	}

	m_strBuf    = new e_char[m_capacity];		
	m_length    = 0;
	m_strBuf[0] = '\0';
}

StringBuffer::StringBuffer(const StringBuffer& copy) {
	m_length   = copy.m_length;
	m_capacity = ((m_length+1)<16) ? 16 : (m_length+1);
	m_strBuf   = new e_char[m_capacity];
	
	e_memcopy(m_strBuf, copy.m_strBuf, m_length*sizeof(e_char));
	m_strBuf[m_length] = '\0';
}

StringBuffer& StringBuffer::operator=(const StringBuffer& copy) {
	delete [] m_strBuf;
	m_length   = copy.m_length;
	m_capacity = ((m_length+1)<16) ? 16 : (m_length+1);
	m_strBuf   = new e_char[m_capacity];
	
	e_memcopy(m_strBuf, copy.m_strBuf, m_length*sizeof(e_char));
	m_strBuf[m_length] = '\0';
	return *this;
}

StringBuffer::StringBuffer(StringBuffer&& move) {
	m_strBuf      = move.m_strBuf;
	m_length      = move.m_length;
	m_capacity    = move.m_capacity;
	move.m_capacity  = 16;
	move.m_strBuf    = new e_char[16];
	move.m_length    = 0;
	move.m_strBuf[0] = '\0';
}

StringBuffer& StringBuffer::operator=(StringBuffer&& move) {
	delete [] m_strBuf;
	m_strBuf      = move.m_strBuf;
	m_length      = move.m_length;
	m_capacity    = move.m_capacity;
	move.m_capacity  = 16;
	move.m_strBuf    = new e_char[16];
	move.m_length    = 0;
	move.m_strBuf[0] = '\0';
	return *this;
}

StringBuffer& StringBuffer::append(const String& str, e_int start, e_int end) {	
	if (str.length() == 0) return *this;
	
	if (end == -1)	end = str.length();
	if (start<0 || start>=str.length()) 
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"String"}, str.length(), start};
	if (end<0 || end>str.length())
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"String"}, str.length(), end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
	
	_append(str.toCStr(), str.length(), start, end);
	
	return *this;
}

StringBuffer& StringBuffer::append(const StringBuffer& buf, e_int start, e_int end) {
	if (&buf == this) 
		throw IllegalArgumentException{__func__, __LINE__};
		
	if (buf.length() == 0) return *this;
	
	if (end == -1)	end = buf.length();
	if (start<0 || start>=buf.length())
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, buf.length(), start};
	if (end<0 || end>buf.length()) 
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, buf.length(), end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
	
	_append(buf.m_strBuf, buf.m_length, start, end);
	
	return *this;
}

StringBuffer& StringBuffer::append(const CharArray& buf, e_int len) {
	if (buf.length()==0 || len==0) return *this;
	if (len<0 || len>buf.length()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	_append(buf.toCArray(), len, 0, len);
	return *this;
}

StringBuffer& StringBuffer::trimToLength() {
	if (m_capacity == 16) 
		return *this;
	
	if (m_capacity == m_length+1)
		return *this;
	
	_updateCapa(((m_length+1)<16) ? 16 : (m_length+1));
	return *this;
}

StringBuffer& StringBuffer::removeHead(e_int head) {
	if (head<0 || head>=m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, head};
	
	e_int resultLen = m_length - (head + 1);
	_remove(resultLen, resultLen, 0, head+1);
	return *this;
}

StringBuffer& StringBuffer::remove(e_int start, e_int end) {	
	if (end == -1)	end = m_length;
	if (start<0 || start>=m_length) 
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, start};
	if (end<0 || end>m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
		
	e_int resultLen = m_length - (end-start);
	e_int moveLen   = m_length - end;
	
	_remove(resultLen, moveLen, start, end);
	
	return *this;
}

StringBuffer& StringBuffer::insert(const String& str, e_int offset) {	
	if (offset<0 || offset>m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, offset};
	
	if (str.length() == 0)
		return *this;
	
	_insert(str.toCStr(), str.length(), offset);

	return *this;
}

StringBuffer& StringBuffer::insert(const StringBuffer& buf, e_int offset) {
	if (offset<0 || offset>m_length)
		throw IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, offset};
	
	if (&buf== this) 
		throw IllegalArgumentException{__func__, __LINE__};
	
	if (buf.length() == 0)
		return *this;
	
	_insert(buf.toCStr(), buf.length(), offset);

	return *this;
}

StringBuffer& StringBuffer::insert(const CharArray& buf, e_int offset)
{	
	if (offset<0 || offset>m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, offset};
	
	if (buf.length() == 0)
		return *this;
	
	_insert(buf.toCArray(), buf.length(), offset);

	return *this;
}


StringBuffer& StringBuffer::replace(const String& str, e_int start, e_int end)
{	
	if (m_length == 0) return *this;
	
	if (end == -1)	end = m_length;
	if (start<0 || start>=m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, start};
	if (end<0 || end>m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
	
	this->remove(start, end);
	this->insert(str, start);
	return *this;
}

StringBuffer& StringBuffer::replace(const String& oldWord, const String& newWord) {		
	if (oldWord.empty())
		return *this;
		
	e_int index = indexOf(oldWord, 0);
	
	while (index != -1)	{
		try {
			replace(newWord, index, oldWord.length()+index);
			index = indexOf(oldWord, index+newWord.length());
		} catch (Exception& e) {
			index = -1;
		}
	}
	
	return *this;
}

StringBuffer& StringBuffer::reverse() {
	e_int center = m_length / 2;
	for (e_int i=0,j=m_length-1; i<center; i++,j--) {
		e_char tmp  = m_strBuf[i];
		m_strBuf[i] = m_strBuf[j];
		m_strBuf[j] = tmp;
	}
	return *this;
}

String StringBuffer::substring(e_int start, e_int end) const
{
	if (m_length == 0) return String{};
	if (end == -1)	end = m_length;
	if (start<0 || start>=m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, start};
	if (end<0 || end>m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, end};
	if (start > end) throw IllegalArgumentException{__func__, __LINE__};
	if (start == end) end++;
	
	return System::move(String{m_strBuf, start, end});
}

e_int StringBuffer::indexOf(const String& str, e_int offset) const
{	
	if (offset<0 || offset>=m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, offset};
		
    if (str.length() == 0)
		return offset;
	
	return _indexOf(str.toCStr(), str.length(), offset);
}


e_int StringBuffer::lastIndexOf(const String& str, e_int offset) const
{

	if (offset == -1) offset = m_length - 1;	

	if (offset<0 || offset>=m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, offset};
	 
	if (str.length() == 0)
		return offset;
	
	return _lastIndexOf(str.toCStr(), str.length(), offset);
}

e_bool StringBuffer::endsWith(const String& chs) const
{			
	if (m_length < chs.length()) return false;

	for (e_int i=chs.length()-1,j=m_length-1; i>=0; i--,j--) {
		if (m_strBuf[j] != chs[i]) return false;
	}

	return true;
}

e_bool StringBuffer::startsWith(const String& chs) const
{
	if (m_length < chs.length()) return false;
 
	for (e_int i=0; i<chs.length(); i++) {
		if (m_strBuf[i] != chs[i]) return false;
	}

	return true;
}

//-------------------------------
static e_void frontPosForSub(IntArray& front, const e_char* chs, e_int len);
static e_void nextPosForSub(IntArray& next, const e_char* chs, e_int len);

e_int StringBuffer::_lastIndexOf(const e_char* chs, e_int len, e_int offset) const {
	e_int i=0, j=0;
	IntArray front{len};
	
	frontPosForSub(front, chs, len);

    for (i=offset, j=len-1; i>=0;) {
        if (m_strBuf[i] == chs[j]) {
            j--;
            if (-1 == j) {
                return i;
            }
            i--;
        } else if (j == len-1) {
            i--;
        } else {
            j = front[j];
        }
    }

    return -1;
}

e_int StringBuffer::_indexOf(const e_char* chs, e_int len, e_int offset) const
{
	e_int i=0, j=0;
	IntArray next{len};

	nextPosForSub(next, chs, len);

    for (i=offset, j=0; i<m_length; ) {
        if (m_strBuf[i] == chs[j]) {
            j++;
            if (j == len) {
                return i-j+1;
            }
            i++;
        } else if (j == 0) {
            i++;
        } else {
            j = next[j];
        }
    }

    return -1;
}

e_void StringBuffer::_insert(const e_char* chs, e_int len, e_int offset)
{
	e_int resultLen = m_length + len;
	
	if (resultLen>m_capacity && resultLen<m_capacity*2) {
		_updateCapa(m_capacity * 2);
	} else if (resultLen >= m_capacity*2) {
		_updateCapa(resultLen + 1);
	}

	e_memmove(&m_strBuf[offset+len], &m_strBuf[offset], (m_length-offset)*sizeof(e_char));
	e_memcopy(&m_strBuf[offset], chs, len*sizeof(e_char));
	m_length = resultLen;
	m_strBuf[m_length] = '\0';
}

e_void StringBuffer::_remove(e_int resultLen, e_int moveLen, e_int start, e_int end)
{
	if (resultLen == 0) {
		m_strBuf[0] = '\0';
		m_length = 0;
	} else {
		e_memmove(&m_strBuf[start], &m_strBuf[end], moveLen*sizeof(e_char));
		m_strBuf[resultLen] = '\0';
		m_length = resultLen;
	}
}

e_void StringBuffer::_append(const e_char* chs, e_int len, e_int start, e_int end)
{
	e_int addLen    = end - start;
	e_int resultLen = len + m_length;

	if (resultLen>m_capacity && resultLen<m_capacity*2)	{
		_updateCapa(m_capacity * 2);
	} else if (resultLen >= m_capacity*2) {
		_updateCapa(resultLen + 1);
	}
	
	if (addLen == 1) {
		m_strBuf[m_length] = chs[start];
		m_length++;
	} else {
		e_memcopy(&m_strBuf[m_length], &chs[start], addLen*sizeof(e_char));
		m_length = resultLen;
	}
	
	m_strBuf[m_length] = '\0';
}

e_void StringBuffer::_updateCapa(e_int newCapa) {
	m_capacity     = newCapa;
	e_char* newBuf = new e_char[m_capacity];	
	
	e_memcopy(newBuf, m_strBuf, m_length*sizeof(e_char));
	delete [] m_strBuf;

	m_strBuf           = newBuf;
	m_strBuf[m_length] = '\0';
}

static e_void nextPosForSub(IntArray& next, const e_char* str, e_int len) {				
    e_int t = 0;
    next[0] = 0;

    for (e_int i=1; i<len; i++) {
        while (t>0 && (str[i] != str[t]))
            t = next[t-1];

        if (str[i] == str[t]) {
            next[i] = t;
            t = t + 1;
        } else {
			next[i] = 0;	
		}
    }
}

static e_void frontPosForSub(IntArray& front, const e_char* str, e_int len) {
    e_int t = len - 1;
    front[t] = t;

    for (e_int i=t-1; i>=0; i--) {
        while ((t<len-1) && str[i] != str[t])
            t = front[t+1];

        if (str[i] == str[t]) {
            front[i] = t;
            t--;
        } else {
			front[i] = len - 1;	
		}
    }
}

} // Easy