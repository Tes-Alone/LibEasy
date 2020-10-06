/*
 * File Name:	String.tcc
 * Date/Time:
 * 	Start: 	2019-12-01 15:17 week:0
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_STRINGTCC_H_
#define _EASY_STRINGTCC_H_

#include "String.hpp"
#include "StringBuffer.hpp"
#include "Array.hpp"
#include "Vector.hpp"
#include "AutoPointer.hpp"

namespace Easy {

#define CHARS 0
#define FIELD 1
#define END   2

class Token {
public:
	Token(e_int _type) : type(_type) {}
	e_int type;
	e_bool equals(const Token& other) const {return false;}
	e_int  compare(const Token& other) const {return -1;}
	e_int  hashCode() const {return 0;}
	String toString() const {return System::move(String{});}
};

class Chars : public Token {
public:
	Chars() : Token(CHARS) {chars = String{};}
	Chars(const Chars& copy) : Token(CHARS) {chars = copy.chars;}
	String chars;
};

class Field : public Token {
public:
	Field() : Token(FIELD) {
		left = false;
		zero = false;
		sign = -1;
		length = 0;
	}
	Field(const Field& copy) : Token(FIELD){
		left = copy.left;
		zero = copy.zero;
		sign = copy.sign;
		length = copy.length;
	}
	e_bool left;
	e_bool zero;
	e_byte sign;
	e_int length;
};

class FormatParser {
private:
	const String& m_format;
	e_int index;
	e_bool isEnd() {return index>=m_format.length();}
public:
	FormatParser(const String& format) : m_format(format), index(0) {}	
	
	Vector<AutoPointer<Token>> parse() {
		Vector<AutoPointer<Token>> tks;
		Token* tk = nextToken();
		while (tk->type != END) {
			tks.add(System::move(AutoPointer<Token>{tk}));
			tk = nextToken();
		}
		return System::move(tks);
	}
private:	
	Token* nextToken()  {
		if (!isEnd() && m_format[index]=='%') {
			return field(index);
		} else if (!isEnd()) {
			return chars(index);
		} else {
			return new Token{END};
		}
	}

	Token* chars(e_int i) {
		Chars* tk = new Chars;
		while (!isEnd() && m_format[index]!='%') {
			index++;
		}
		tk->chars = m_format.substring(i, index);
		return tk;
	}

	Token* field(e_int i) {
		Field* tk = new Field;
		index++;
		while (!isEnd()) {
			if (i==index-1 && m_format[index]=='%') {
				index++; tk->sign = 0; return tk;
			} else if (i==index-1 && m_format[index]=='-') {
				tk->left = true; index++;
			} else if (i==index-1 && m_format[index]=='0') {
				tk->zero = true; index++;
			} else if (m_format[index]>='1' && m_format[index]<='9') {
				e_int length = m_format[index]-'0'; index++;
				while (!isEnd() && (m_format[index]>='0'&&m_format[index]<='9')) {
					length = (m_format[index]-'0') + length * 10;
					index++;
				}
				if (isEnd()) throw  FormatedStringFormatException{__func__, __LINE__, m_format};
				tk->length = length;
			} else if (m_format[index]=='s') {
				tk->sign = 1; index++;
				return tk;
			} else {
				throw  FormatedStringFormatException{__func__, __LINE__, m_format};
			}
		}

		if (tk->sign == -1) throw  FormatedStringFormatException{__func__, __LINE__, m_format};
		else {
			return new Token{END};
		}
	}
};

template<class T>
void getArguments(Vector<String>& strs, const T& arg) {
	strs.add(System::move(arg.toString()));
}

template<class F, class ...T>
void getArguments(Vector<String>& strs, const F& first, const T&... rest) {
	strs.add(System::move(first.toString()));
	getArguments(strs, rest...);
}

template<class ...T>
String String::format(const String& str, const T&... args) {
	Vector<String> strs;
	FormatParser parser{(String&)str};
	Vector<AutoPointer<Token>> formats = System::move(parser.parse());
	getArguments(strs, args...);

	StringBuffer sb;
	e_int argIndex = 0;
	e_int size     = formats.size();

	for (e_int i=0; i<size; i++) {
		if (formats[i]->type==CHARS) {
			Chars* tk = (Chars*) formats[i].lock();
			sb.append(tk->chars);
		} else if (formats[i]->type==FIELD) {
			Field* tk = (Field*) formats[i].lock();
			switch (tk->sign) {
				case 0:
					sb.append(String{L"%"});
				break;
				case 1:
				{
					e_int gap = tk->length - strs[argIndex].length();
					if (gap>0 && tk->left) {
						sb.append(strs[argIndex++]);
						for (e_int i=0; i<gap; i++) {
							sb.append(String{L" "});
						}
					} else if (gap>0 && !tk->left && !tk->zero) {
						for (e_int i=0; i<gap; i++) {
							sb.append(String{L" "});
						}
						sb.append(strs[argIndex++]);
					} else if (gap>0 && tk->zero) {
						for (e_int i=0; i<gap; i++) {
							sb.append(String{L"0"});
						}
						sb.append(strs[argIndex++]);
					} else if (gap <= 0) {
						sb.append(strs[argIndex++]);
					}
					break;
				}
			} 
		}
	}

	return System::move(sb.toString());
}

} // Easy

#endif // _EASY_STRINGTCC_H_

