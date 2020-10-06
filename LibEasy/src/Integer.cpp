
/*
 * File name: Integer.cpp
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

#include "Integer.hpp"

namespace Easy {

e_byte digitChar[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

String Integer::toString(e_int i, e_int radix) {
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

String Integer::toHexString(e_int i) {
    e_char buf[11];
    e_bool negative = (i < 0);
    e_int charPos = 9;
    
    buf[10] = 0;
    
    if (!negative) {
        i = -i;
    }
    
    while (i <= -16) {
        buf[charPos--] = digitChar[-(i % 16)];
        i = i / 16;
    }
    
    buf[charPos] = digitChar[-i];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

String Integer::toString(e_int i) {
	e_char buf[12];
    e_bool negative = (i < 0);
    e_int charPos = 10;
    
    buf[11] = 0;
    
    if (!negative) {
        i = -i;
    }
    
    while (i <= -10) {
        buf[charPos--] = digitChar[-(i % 10)];
        i = i / 10;
    }
    
    buf[charPos] = digitChar[-i];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

String Integer::toOctString(e_int i) {
	e_char buf[14];
    e_bool negative = (i < 0);
    e_int charPos = 13;
    
    buf[13] = 0;
    
    if (!negative) {
        i = -i;
    }
    
    while (i <= -8) {
        buf[charPos--] = digitChar[-(i % 8)];
        i = i / 8;
    }
    
    buf[charPos] = digitChar[-i];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

String Integer::toBinaryString(e_int i) {
	e_char buf[66];
    e_bool negative = (i < 0);
    e_int charPos = 64;
    
    buf[65] = 0;
    
    if (!negative) {
        i = -i;
    }
    
    while (i <= -2) {
        buf[charPos--] = digitChar[-(i % 2)];
        i = i / 2;
    }
    
    buf[charPos] = digitChar[-i];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

e_int Integer::parse(const String& s) {		
	e_bool negative = false;
    e_int i = 0, len = s.length();
    e_int limit = -Integer::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Integer::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_int multmin = limit / 10;
        e_int result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if (s[i]>='0' && s[i]<='9') {
            	e_int digit = s[i]-'0';
            	if (result < multmin) {
            		throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result *= 10;
	            if (result < limit + digit) {
	                throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result -= digit;
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_byte digits[128] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

e_int Integer::parseHex(const String& s) {
	e_bool negative = false;
    e_int i = 0, len = s.length();
    e_int limit = -Integer::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Integer::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_int multmin = limit >> 4;
        e_int result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if ((e_ushort)s[i] > 127) 
            	throw NumberFormatException(__func__, __LINE__, s);
            e_int digit = digits[s[i]];
			if (digit < 0)
				throw NumberFormatException(__func__, __LINE__, s);
            if (result < multmin) {
        		throw NumberOverflowException(__func__, __LINE__, s);
            }
            result <<= 4;
            if (result < limit + digit) {
                throw NumberOverflowException(__func__, __LINE__, s);
            }
            result -= digit;
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_int Integer::parseOct(const String& s) {
	e_bool negative = false;
    e_int i = 0, len = s.length();
    e_int limit = -Integer::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Integer::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_int multmin = limit >> 3;
        e_int result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if (s[i]>='0' && s[i]<='7') {
            	e_int digit = s[i]-'0';
            	if (result < multmin) {
            		throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result <<= 3;
	            if (result < limit + digit) {
	                throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result -= digit;
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_int Integer::parseBinary(const String& s) {
	e_bool negative = false;
    e_int i = 0, len = s.length();
    e_int limit = -Integer::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Integer::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_int multmin = limit >> 1;
        e_int result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if (s[i]=='0' || s[i]=='1') {
            	e_int digit = s[i]-'0';
            	if (result < multmin) {
            		throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result <<= 1;
	            if (result < limit + digit) {
	                throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result -= digit;
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_int Integer::parse(const String& s, e_int radix) {
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

String Long::toString(e_long l, e_int radix) {
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

String Long::toString(e_long l) {
	e_char buf[21];
    e_bool negative = (l < 0);
    e_int charPos = 19;
    
    buf[20] = 0;
    
    if (!negative) {
        l = -l;
    }
    
    while (l <= -10) {
        buf[charPos--] = digitChar[-(l % 10)];
        l = l / 10;
    }
    
    buf[charPos] = digitChar[-l];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

String Long::toHexString(e_long l) {
    e_char buf[18];
    e_bool negative = (l < 0);
    e_int charPos = 16;
    
    buf[17] = 0;
    
    if (!negative) {
        l = -l;
    }
    
    while (l <= -16) {
        buf[charPos--] = digitChar[-(l % 16)];
        l = l / 16;
    }
    
    buf[charPos] = digitChar[-l];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

String Long::toOctString(e_long l) {
    e_char buf[23];
    e_bool negative = (l < 0);
    e_int charPos = 21;
    
    buf[22] = 0;
    
    if (!negative) {
        l = -l;
    }
    
    while (l <= -8) {
        buf[charPos--] = digitChar[-(l % 8)];
        l = l / 8;
    }
    
    buf[charPos] = digitChar[-l];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

String Long::toBinaryString(e_long l) {
    e_char buf[67];
    e_bool negative = (l < 0);
    e_int charPos = 65;
    
    buf[66] = 0;
    
    if (!negative) {
        l = -l;
    }
    
    while (l <= -2) {
        buf[charPos--] = digitChar[-(l % 2)];
        l = l / 2;
    }
    
    buf[charPos] = digitChar[-l];
    
    if (negative) {
        buf[--charPos] = '-';
    }
	
    return String(&buf[charPos]);
}

e_long Long::parse(const String& s) {
	e_bool negative = false;
    e_int  i = 0, len = s.length();
    e_long limit = -Long::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Long::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_long multmin = limit / 10;
        e_long result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if (s[i]>='0' && s[i]<='9') {
            	e_int digit = s[i]-'0';
            	if (result < multmin) {
            		throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result *= 10;
	            if (result < limit + digit) {
	                throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result -= digit;
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_long Long::parseHex(const String& s) {
	e_bool negative = false;
    e_int  i = 0, len = s.length();
    e_long limit = -Long::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Long::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_long multmin = limit >> 4;
        e_long result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if ((e_ushort)s[i] > 127) 
            	throw NumberFormatException(__func__, __LINE__, s);
            e_int digit = digits[s[i]];
			if (digit < 0)
				throw NumberFormatException(__func__, __LINE__, s);
            if (result < multmin) {
        		throw NumberOverflowException(__func__, __LINE__, s);
            }
            result <<= 4;
            if (result < limit + digit) {
                throw NumberOverflowException(__func__, __LINE__, s);
            }
            result -= digit;
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_long Long::parseOct(const String& s) {
	e_bool negative = false;
    e_int  i = 0, len = s.length();
    e_long limit = -Long::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Long::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_long multmin = limit >> 3;
        e_long result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if (s[i]>='0' && s[i]<='7') {
            	e_int digit = s[i]-'0';
            	if (result < multmin) {
            		throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result <<= 3;
	            if (result < limit + digit) {
	                throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result -= digit;
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_long Long::parseBinary(const String& s) {
	e_bool negative = false;
    e_int  i = 0, len = s.length();
    e_long limit = -Long::MAX;

    if (len > 0) {
        e_char firstChar = s.charAt(0);
        if (firstChar < '0') { // Possible leading "+" or "-"
            if (firstChar == '-') {
                negative = true;
                limit = Long::MIN;
            } else if (firstChar != '+') {
                throw NumberFormatException(__func__, __LINE__, s);
            }

            if (len == 1) { // Cannot have lone "+" or "-"
                throw NumberFormatException(__func__, __LINE__, s);
            }
            i++;
        }
        e_long multmin = limit >> 1;
        e_long result = 0;
        for (; i < len; i++) {
        	if (s[i]=='_') continue;
            // Accumulating negatively avoids surprises near MAX_VALUE
            if (s[i]=='0' || s[i]=='1') {
            	e_int digit = s[i]-'0';
            	if (result < multmin) {
            		throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result <<= 1;
	            if (result < limit + digit) {
	                throw NumberOverflowException(__func__, __LINE__, s);
	            }
	            result -= digit;
            } else {           
                throw NumberFormatException(__func__, __LINE__, s);
            }
        }
        return negative ? result : -result;
    } else {
        throw NumberFormatException(__func__, __LINE__, s);
    }
}

e_long Long::parse(const String& s, e_int radix) {
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