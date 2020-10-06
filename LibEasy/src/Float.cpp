/*
 * File Name:	Float.cpp
 * Date/Time:
 * 	Start: 	2019-11-26 18:28 week:2
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <cstdio>

#include "AsmLib.hpp"
#include "Math.hpp"
#include "Float.hpp"
#include "String.hpp"
#include "System.hpp"
#include "StringBuffer.hpp"
#include "UInteger.hpp"

namespace Easy {

String Float::toString(e_float f) {
	
	// FIXME 
	
	e_byte buf[30];
	::gcvt(f, 29, (char*)buf);
	return System::move(String{buf});
}

e_uint Float::toIntBits(e_float f) {
	return asm_hex_float(f);
}

String Float::toHexString(e_float f) {
	e_uint hex = asm_hex_float(f);
	return System::move(UInteger::toHexString(hex));
}

String Float::toOctString(e_float f) {
	e_uint hex = asm_hex_float(f);
	return System::move(UInteger::toOctString(hex));
}

String Float::toBinaryString(e_float f) {
	e_uint hex = asm_hex_float(f);
	return System::move(UInteger::toBinaryString(hex));
}

e_bool Float::equals(e_float f1, e_float f2) {
	return Math::abs(f1-f2) <= Float::epsilon();
}

e_int  Float::compare(e_float f1, e_float f2) {
	return Float::equals(f1, f2)?0:(f1>f2?1:-1);
}

e_int Float::hashCode(e_float f) {
	e_uint hex = asm_hex_float(f);
	return Math::abs((e_int)hex);
}

e_float Float::parse(const String& value) {
	if (value.empty())
		throw NumberFormatException{__func__, __LINE__, value};

	e_float d = 0.0f;
	e_int 	i = 0;
	e_bool 	passedDot = false;
	e_bool	passedE   = false;
	e_bool	hasNoNumber = true;
	e_bool  hasNoExpNumber = true;
	e_int   dotPos    = 0;
	e_int 	ePos      = 0;
	e_int 	exp		  = 0;
	e_int 	resultSign = 1;
	e_int 	expSign	   = 1;
	e_float	pow10	   = 1.0f;

	while (i < value.length()) {
		if (value[i]>='0' && value[i]<='9') {
			hasNoNumber = false;
			if (!passedE && !passedDot) {
				d = (value[i] - '0') + d * 10.0f;
				i++;
			} else if (!passedE && passedDot) {
				pow10 *= 10.0f;
				d = d + ((e_float)(value[i] - '0') / pow10);
				i++;
			} else if (passedE && !passedDot) {
				hasNoExpNumber = false;
				exp = (value[i] - '0') + exp * 10.0f;
				i++;
			} else { //passedE && passedDot
				if ((dotPos > ePos))
					throw NumberFormatException{__func__, __LINE__, value};
				hasNoExpNumber = false;
				exp = (value[i] - '0') + exp * 10.0f;
				i++;
			}
		} else if (!passedDot && value[i] == '.') {
			dotPos = i;
			passedDot = true;
			i++;
		} else if (!passedE && (value[i]=='e' || value[i]=='E')) {
			if (i == 0) throw NumberFormatException{__func__, __LINE__, value};
			ePos = i;
			passedE = true;
			i++;
		} else if (!passedE && value[i]=='-') {
			if (i != 0) {
				throw NumberFormatException{__func__, __LINE__, value};
			} else {
				resultSign = -1;
				i++;
			}
		} else if (passedE && value[i]=='-') {
			if (ePos != i-1) {
				throw NumberFormatException{__func__, __LINE__, value};
			} else {
				expSign = -1;
				i++;
			}
		} else {
			throw NumberFormatException{__func__, __LINE__, value};
		}
	}

	if (hasNoNumber)
		throw NumberFormatException{__func__, __LINE__, value};
	if (passedE && hasNoExpNumber)
		throw NumberFormatException{__func__, __LINE__, value};
	
	e_float checkExp = expSign * exp;
	if (checkExp<Float::minExp() || checkExp>Float::maxExp()) {
		throw FloatExpException{__func__, __LINE__, value};
	}
	return resultSign * d * Math::pow(10.0f, checkExp);
}

String Double::toString(e_double d) {
	// FIXME
	e_byte buf[30];
	::gcvt(d, 29, (char*)buf);
	return System::move(String{buf});
}

e_ulong Double::toLongBits(e_double d) {
	return asm_hex_double(d);
}

e_bool Double::equals(e_double d1, e_double d2) {
	return Math::abs(d1-d2) <= Double::epsilon();
}

e_int Double::compare(e_double d1, e_double d2) {
	return Double::equals(d1,d2)?0:(d1>d2?1:-1);
}

e_int Double::hashCode(e_double d) {
	e_long hex = asm_hex_double(d);
	return Math::abs((e_int)(hex));
}

String Double::toHexString(e_double d) {
	e_ulong hex = asm_hex_double(d);
	return System::move(ULong::toHexString(hex));
}

String Double::toOctString(e_double d) {
	e_ulong hex = asm_hex_double(d);
	return System::move(ULong::toOctString(hex));
}

String Double::toBinaryString(e_double d) {
	e_ulong hex = asm_hex_double(d);
	return System::move(ULong::toBinaryString(hex));
}

e_double Double::parse(const String& value) {
	if (value.empty())
		throw NumberFormatException{__func__, __LINE__, value};

	e_double  d 		   = 0.0;
	e_double  pow10 	   = 1.0;
	e_int 	  i 		   = 0;
	e_bool    passedDot    = false;
	e_bool	  passedE      = false;
	e_bool	  hasNoNumber  = true;
	e_bool 	  hasNoExpNumber = true;
	e_int     dotPos       = 0;
	e_int 	  ePos         = 0;
	e_int 	  exp		   = 0;
	e_int 	  resultSign   = 1;
	e_int 	  expSign	   = 1;

	while (i < value.length()) {
		if (value[i]>='0' && value[i]<='9') {
			hasNoNumber = false;
			if (!passedE && !passedDot) {
				d = (value[i] - '0') + d * 10;
				i++;
			} else if (!passedE && passedDot) {
				pow10 *= 10.0;
				d = d + ((e_double)(value[i] - '0') / pow10);
				i++;
			} else if (passedE && !passedDot) {
				hasNoExpNumber = false;
				exp = (value[i] - '0') + exp * 10;
				i++;
			} else {
				if ((dotPos > ePos))
					throw NumberFormatException{__func__, __LINE__, value};
				hasNoExpNumber = false;
				exp = (value[i] - '0') + exp*10;
				i++;
			}
		} else if (!passedDot && value[i] == '.') {
			dotPos = i;
			passedDot = true;
			i++;
		} else if (!passedE && (value[i]=='e' || value[i]=='E')) {
			if (i==0) throw NumberFormatException{__func__, __LINE__, value};
			ePos = i;
			passedE = true;
			i++;
		} else if (!passedE && value[i]=='-') {
			if (i != 0) {
				throw NumberFormatException{__func__, __LINE__, value};
			} else {
				resultSign = -1;
				i++;
			}
		} else if (passedE && value[i]=='-') {
			if (ePos != i-1) {
				throw NumberFormatException{__func__, __LINE__, value};
			} else {
				expSign = -1;
				i++;
			}
		} else {
			throw NumberFormatException{__func__, __LINE__, value};
		}
	}

	if (hasNoNumber)
		throw NumberFormatException{__func__, __LINE__, value};
	if (passedE && hasNoExpNumber)
		throw NumberFormatException{__func__, __LINE__, value};
	
	e_double checkExp = expSign * exp;
	if (checkExp<Double::minExp() || checkExp>Double::maxExp()) {
		throw FloatExpException{__func__, __LINE__, value};
	}

	return resultSign * d * Math::pow(10.0, checkExp);
}

} // Easy