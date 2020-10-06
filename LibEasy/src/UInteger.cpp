
/*
 * File name: UInteger.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月06日 14时38分02秒 周7
 * End Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include "UInteger.hpp"
#include "EasyC.hpp"

namespace Easy {

extern e_byte digitChar[16];

String UInteger::toHexString(e_uint i) {
	e_char buf[9];
    e_int  charPos  = 8;
	buf[charPos--] = '\0';
	
	while (i >= 16)
	{
		buf[charPos--] = digitChar[i & 0xF];
		i = i >> 4;
	}
	
	buf[charPos] = digitChar[i];	
	return System::move(String{&buf[charPos]});
}

String UInteger::toString(e_uint i) {
	e_char buf[20];
    e_int  charPos  = 19;
	buf[charPos--] = '\0';
	
	while (i >= 10)
	{
		buf[charPos--] = (i%10) + '0';
		i = i / 10;
	}
	
	buf[charPos] = i + '0';	
	return System::move(String{&buf[charPos]});	
}

String UInteger::toString(e_uint i, e_int radix) {
	if (radix == 2) {
		return toBinaryString(i);
	}
	if (radix == 8) {
		return toOctString(i);
	}
	if (radix == 16) {
		return toHexString(i);
	}
	return toString(i);
}

String UInteger::toOctString(e_uint i) {
	e_char buf[20];
    e_int  charPos  = 19;
	buf[charPos--] = '\0';
	
	while (i >= 8)
	{
		buf[charPos--] = (i&07) + '0';
		i = i >> 3;
	}
	
	buf[charPos] = i + '0';	
	return System::move(String{&buf[charPos]});
}

String UInteger::toBinaryString(e_uint i) {
	e_char buf[33];
    e_int  charPos  = 32;
	buf[charPos--] = '\0';
	
	while (i >= 2)
	{
		buf[charPos--] = (i&1) + '0';
		i = i >> 1;
	}
	
	buf[charPos] = i + '0';	
	return System::move(String{&buf[charPos]});
}

e_uint UInteger::parse(const String& s) {	
    e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if (s[i]>='0' && s[i]<='9') {
	            result = (s[i]-'0') + result * 10;
	            if (result > UInteger::MAX) {
	            	throw NumberOverflowException(__func__, __LINE__, s);
	            }
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

extern e_byte digits[128];

e_uint UInteger::parseHex(const String& s) {		
    e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if ((e_ushort)s[i] > 127) 
            	throw NumberFormatException(__func__, __LINE__, s);
            e_int digit = digits[s[i]];
			if (digit < 0)
				throw NumberFormatException(__func__, __LINE__, s);
			result = digit + (result << 4);
			if (result > UInteger::MAX) {
            	throw NumberOverflowException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_uint UInteger::parseOct(const String& s) {		
	e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if (s[i]>='0' && s[i]<='7') {
	            result = (s[i]-'0') + (result << 3);
	            if (result > UInteger::MAX) {
	            	throw NumberOverflowException(__func__, __LINE__, s);
	            }
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_uint UInteger::parseBinary(const String& s) {		
	e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if (s[i]>='0' && s[i]<='1') {
	            result = (s[i]-'0') + (result << 1);
	            if (result > UInteger::MAX) {
	            	throw NumberOverflowException(__func__, __LINE__, s);
	            }
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_uint UInteger::parse(const String& s, e_int radix) {
	if (radix == 2) {
		return parseBinary(s);
	}
	if (radix == 8) {
		return parseOct(s);
	}
	if (radix == 16) {
		return parseHex(s);
	}
	return parse(s);
}

String ULong::toString(e_ulong l, e_int radix) {
	if (radix == 2) {
		return toBinaryString(l);
	}
	if (radix == 8) {
		return toOctString(l);
	}
	if (radix == 16) {
		return toHexString(l);
	}
	return toString(l);
}

String ULong::toString(e_ulong l) {
    e_char buf[32];
    e_int  charPos  = 31;
	buf[charPos--] = '\0';
	
	while (l >= 10)
	{
		buf[charPos--] = (l % 10) + '0';
		l = l / 10;
	}
	
	buf[charPos] = l + '0';	
	return System::move(String{&buf[charPos]});
}

String ULong::toHexString(e_ulong l) {
    e_char buf[17];
    e_int  charPos  = 16;
	buf[charPos--] = '\0';
	
	while (l >= 16)
	{
		buf[charPos--] = digitChar[(l & 0xF)];
		l = l >> 4;
	}
	
	buf[charPos] = digitChar[l];	
	return System::move(String{&buf[charPos]});
}

String ULong::toOctString(e_ulong l) {
    e_char buf[23];
    e_int  charPos  = 22;
	buf[charPos--] = '\0';
	
	while (l >= 8)
	{
		buf[charPos--] = (l & 07) + '0';
		l = l >> 3;
	}
	
	buf[charPos] = l + '0';	
	return System::move(String{&buf[charPos]});
}

String ULong::toBinaryString(e_ulong l) {
    e_char buf[65];
    e_int  charPos  = 64;
	buf[charPos--] = '\0';
	
	while (l >= 2)
	{
		buf[charPos--] = (l & 1) + '0';
		l = l >> 1;
	}
	
	buf[charPos] = l + '0';	
	return System::move(String{&buf[charPos]});
}

e_ulong ULong::parse(const String& s) {		
	e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if (s[i]>='0' && s[i]<='9') {
            	e_ulong old = result;
	            result = (s[i]-'0') + (result * 10);
	            if (old > result) {
	            	throw NumberOverflowException(__func__, __LINE__, s);
	            }
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_ulong ULong::parseHex(const String& s) {		
	e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if ((e_ushort)s[i] > 127) 
            	throw NumberFormatException(__func__, __LINE__, s);
            e_int digit = digits[s[i]];
			if (digit < 0)
				throw NumberFormatException(__func__, __LINE__, s);
			e_ulong old = result;
			result = digit + (result << 4);
			if (old > result) {
            	throw NumberOverflowException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_ulong ULong::parseOct(const String& s) {		
	e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if (s[i]>='0' && s[i]<='7') {
            	e_ulong old = result;
	            result = (s[i]-'0') + (result << 3);
	            if (old > result) {
	            	throw NumberOverflowException(__func__, __LINE__, s);
	            }
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_ulong ULong::parseBinary(const String& s) {		
	e_int  i = 0, len = s.length();
    if (len > 0) {
        e_char firstChar = s[0];
        if (firstChar < '0') { // Possible leading "+"
            if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" 
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_ulong result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            if (s[i]>='0' && s[i]<='1') {
            	e_ulong old = result;
	            result = (s[i]-'0') + (result << 1);
	            if (old > result) {
	            	throw NumberOverflowException(__func__, __LINE__, s);
	            }
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_ulong ULong::parse(const String& s, e_int radix) {		
	if (radix == 2) {
		return parseBinary(s);
	}
	if (radix == 8) {
		return parseOct(s);
	}
	if (radix == 16) {
		return parseHex(s);
	}
	return parse(s);
}

} // Easy