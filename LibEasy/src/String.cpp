
/*
 * File name: String.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月07日 18时13分32秒 周1
 * End Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
 * 
*/


#include "Exception.hpp"
#include "String.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "Bool.hpp"
#include "System.hpp"
#include "StringBuffer.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "BasicArray.hpp"
#include "BasicArray.tcc"

#include "EasyC.hpp"

namespace Easy {
	
String::String(const e_char* str, e_int start, e_int end) {
	if (str == nullptr) 
		throw NullPointerException{__func__, __LINE__};

	m_length = length(str);
	
	if (m_length == 0)
	{
		m_str = new e_char[1];
		m_str[m_length] = '\0';
	}
	else
	{			
		_checkStartAndEnd(start, end, __func__, __LINE__);
		m_length = end - start;
		m_str    = new e_char[m_length+1];
		e_memcopy(m_str, &str[start], m_length*sizeof(e_char));
		m_str[m_length] = '\0';
	}
}

String::String(const e_byte* bytes) {
	if (bytes == nullptr)
		throw NullPointerException{__func__, __LINE__};
	m_length = MultiByteToWideChar(CP_ACP, 0, (const char*)bytes, -1, nullptr, 0);
	m_str    = new e_char[m_length];
	MultiByteToWideChar(CP_ACP, 0, (const char*)bytes, -1, m_str, m_length);
	m_length--;
}

String::String(const String& copy) {
	if (copy.m_length == 0) {
		m_length = 0;
		m_str    = new e_char[1];
		m_str[0] = '\0';
	} else {
		m_length = copy.m_length;
		m_str    = new e_char[m_length+1];
		e_memcopy(m_str, copy.m_str, m_length*sizeof(e_char));
		m_str[m_length] = '\0';
	}
}

String& String::operator=(const String& copy) {
	delete [] m_str;
	if (copy.m_length == 0) {
		m_length = 0;
		m_str    = new e_char[1];
		m_str[0] = '\0';
	} else {
		m_length = copy.m_length;
		m_str    = new e_char[m_length+1];
		e_memcopy(m_str, copy.m_str, m_length*sizeof(e_char));
		m_str[m_length] = '\0';
	}
	return *this;
}

String::String(String&& move) {
	m_length = move.m_length;
	m_str 	 = move.m_str;
	move.m_str = new e_char[1];
	move.m_length = 0;
	move.m_str[0] = '\0';
}
	
String& String::operator=(String&& move) {
	delete [] m_str;
	m_length = move.m_length;
	m_str 	 = move.m_str;
	move.m_str = new e_char[1];
	move.m_length = 0;
	move.m_str[0] = '\0';
	return *this;
}

e_bool String::equals(const String& other) const {		
	if (other.m_length != this->m_length) return false;
	for (e_int i=0; i<this->m_length; i++) 
		if (other.m_str[i] != this->m_str[i])
			return false;
	return true;
}

e_int String::compare(const String& other) const
{	
    e_int len = Math::min(this->m_length, other.m_length);

    for (e_int i=0; i<len; i++){
        if (this->m_str[i] > other.m_str[i])
            return 1;

        if (this->m_str[i] < other.m_str[i])
            return -1;
    }

    if (this->m_length == other.m_length)
        return 0;

    return (this->m_length > other.m_length) ? 1 : -1;
}

e_int String::hashCode() const {
	const e_int p = 31;
	const e_int m = 1e9 + 9;
	e_long hash_value = 0;
	e_long p_pow      = 1;
	
	for (e_int i=0; i<m_length; i++) {
		hash_value = (hash_value + (m_str[i] * p_pow)) % m;
		p_pow = (p_pow * p) % m;
	}
	
	return Math::abs(hash_value);
}

String String::operator*(int times) const {
	if (times < 0) throw IllegalArgumentException{__func__, __LINE__};
	StringBuffer sb;
	for (e_int i=0; i<times; i++) {
		sb.append(*this);
	}
	return System::move(sb.toString());
}

e_bool String::equalsIgnoreCase(const String& other) const
{
	if (m_length != other.m_length) return false;
	for (e_int i=0; i<m_length; i++) {
		if (Character::toUpperCase(m_str[i]) != Character::toUpperCase(other[i])) {
			return false;
		}
	}
	return true;
}

e_int String::compareIgnoreCase(const String& other) const
{
	e_int otherLen = other.length();
	e_int len = Math::min(this->m_length, otherLen);

    for (e_int i=0; i<len; i++)
    {
        if (Character::toUpperCase(this->m_str[i]) > Character::toUpperCase(other[i]))
            return 1;

        if (Character::toUpperCase(this->m_str[i]) < Character::toUpperCase(other[i]))
            return -1;
    }

    if (this->m_length == otherLen) return 0;
    return (this->m_length > otherLen) ? 1 : -1;
}

ByteArray String::getBytes() const {
	e_int len = WideCharToMultiByte(CP_ACP, 0, m_str, -1, nullptr, 0, nullptr, nullptr);
	ByteArray pMulStr{len};
	WideCharToMultiByte(CP_ACP, 0, m_str, -1, (char*)pMulStr.toCArray(), len, nullptr, nullptr);
	return System::move(pMulStr);
}

String String::substring(e_int start, e_int end) const
{
	if (m_length == 0) return String{};
	
	_checkStartAndEnd(start, end, __func__, __LINE__);
	return System::move(String{this->m_str, start, end});
}

e_bool String::endsWith(const String& chs) const
{			
	if (m_length < chs.length()) return false;

	for (e_int i=chs.length()-1,j=m_length-1; i>=0; i--,j--) {
		if (m_str[j] != chs[i]) return false;
	}

	return true;
}

e_bool String::startsWith(const String& chs) const
{
	if (m_length < chs.length()) return false;
 
	for (e_int i=0; i<chs.length(); i++) {
		if (m_str[i] != chs[i]) return false;
	}

	return true;
}


String String::toUpperCase() const
{		
	e_char* upperChs = new e_char[m_length+1];
		
	for (e_int i=0; i<m_length; i++)
	{
		upperChs[i] = Character::toUpperCase(m_str[i]);
	}
	
	upperChs[m_length] = '\0';
	
	String upperStr{};
	delete [] upperStr.m_str;

	upperStr.m_str    = upperChs;
	upperStr.m_length = m_length;
	
	return System::move(upperStr);
}


String String::toLowerCase() const
{		
	e_char* lowerChs = new e_char[m_length+1];
		
	for (e_int i=0; i<m_length; i++)
	{
		lowerChs[i] = Character::toLowerCase(m_str[i]);
	}
	
	lowerChs[m_length] = '\0';
	
	String lowerStr{};
	delete [] lowerStr.m_str;

	lowerStr.m_str    = lowerChs;
	lowerStr.m_length = m_length;
	
	return System::move(lowerStr);
}

static e_void nextPosForSub(IntArray& next, const String& str);

e_int String::indexOf(const String& chs, e_int offset) const
{		
	if (offset<0 || offset>=m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"String"}, m_length, offset};

	if (chs.empty())
		return offset;

    e_int i=0, j=0, subLen=chs.length();
	IntArray next{subLen};

	nextPosForSub(next, chs);

    for (i=offset, j=0; i<m_length; )
    {
        if (m_str[i] == chs[j])
        {
            j++;
            if (j == subLen)
            {
                return i-j+1;
            }
            i++;
        }
        else if (j == 0)
        {
            i++;
        }
        else
        {
            j = next[j];
        }
    }

    return -1;
}

static e_void frontPosForSub(IntArray& front, const String& str);

e_int String::lastIndexOf(const String& chs, e_int offset) const 
{			
	if (offset == -1) offset = m_length - 1;
	
	if (offset<0 || offset>=m_length)
		throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"String"}, m_length, offset};
	
	if (chs.empty())
		return offset;
	
	e_int i=0, j=0, subLen=chs.length();
	IntArray front{subLen};

	frontPosForSub(front, chs);

    for (i=offset, j=subLen-1; i>=0;)
    {
        if (m_str[i] == chs[j])
        {
            j--;
            if (-1 == j)
            {
                return i;
            }
            i--;
        }
        else if (j == subLen-1)
        {
            i--;
        }
        else
        {
            j = front[j];
        }
    }

    return -1;
}


String String::trim() const
{		
	e_int start = 0;
	e_int end   = m_length;
	
	for (e_int i=0; i<m_length; i++)
	{
		if (Character::isWhiteSpace(m_str[i])) start++;
		else break;
	}
	
	if (start == m_length)
	{
		return String{};
	}
	
	for (e_int i=end-1; i>0; i--)
	{
		if (Character::isWhiteSpace(m_str[i])) end--;
		else break;
	}
	
	return System::move(this->substring(start, end));
}
	
String String::concat(const String& chs) const
{
	e_int len1 = m_length;
	e_int len  = chs.length() + len1;
	
	String cat{};
	delete [] cat.m_str;
	cat.m_str = new e_char[len+1];
		
	e_memcopy(cat.m_str, m_str, sizeof(e_char)*len1);
	e_memcopy(&cat.m_str[len1], chs.m_str, sizeof(e_char)*(len-len1));
	cat.m_str[len] = '\0';	
	cat.m_length   = len;
	
	return System::move(cat);
}

String String::reverse() const {
	e_char* chs = new e_char[m_length+1];
	for (e_int i=0,j=m_length-1; i<m_length; i++,j--) {
		chs[j] = m_str[i];
	}
	chs[m_length] = '\0';
	String str{};
	delete [] str.m_str;
	str.m_length = m_length;
	str.m_str    = chs;
	return System::move(str);
}

e_int String::length(const e_char* chs)
{
	if (chs == nullptr)
		throw NullPointerException{__func__, __LINE__};
		
	e_int count = 0;
	while (chs[count++]);
	return count - 1;
}

String String::replace(const String& oldWord, const String& newWord) const
{
	StringBuffer sb;
	sb.append(*this);
	sb.replace(oldWord, newWord);
	return System::move(sb.toString());
}

Vector<String> String::split(const String& sub) const {
	e_int index    = 0;
	e_int preIndex = 0;
	Vector<String> result;
	
	if (sub.empty()) {
		result.add(*this);
		return System::move(result);
	}
	
	try {
		while (true) {
			index = this->indexOf(sub, index);
			if (preIndex == index) {
				preIndex = index = index + sub.length();
			} else {
				result.add(this->substring(preIndex, index));
				if (index == -1) break;
				preIndex = index = index + sub.length();
			}
		}
	} catch (Exception& e) {}
	
	return System::move(result);
}

e_void String::_checkStartAndEnd(e_int& start, e_int& end, const String& func, e_int line) const {
	if (end == -1)	end = m_length;
	if (start<0 || start>=m_length) 
		throw IndexOutOfBoundsException{func, line, String{L"String"}, m_length, start};
	if (end<0 || end>m_length) 
		throw  IndexOutOfBoundsException{func, line, String{L"String"}, m_length, end};
	if (start > end)
		throw IllegalArgumentException{func, line};
	if (start == end) end++;
}


// ------------------------------------

static e_void nextPosForSub(IntArray& next, const String& str)
{		
	e_int len = str.length();
    e_int t = 0;
    next[0] = 0;

    for (e_int i=1; i<len; i++)
    {
        while (t>0 && (str[i] != str[t]))
            t = next[t-1];

        if (str[i] == str[t])
        {
            next[i] = t;
            t = t + 1;
        }
        else
		{
			next[i] = 0;	
		}
    }
}

static e_void frontPosForSub(IntArray& front, const String& str)
{		
    e_int len = str.length();
    e_int t = len - 1;
    front[t] = t;

    for (e_int i=t-1; i>=0; i--)
    {
        while ((t<len-1) && str[i] != str[t])
            t = front[t+1];

        if (str[i] == str[t])
        {
            front[i] = t;
            t--;
        }
        else
		{
			front[i] = len - 1;	
		}
    }
}

} //Easy