/*
 * File Name:	Character.hpp
 * Date/Time:
 * 	Start: 	2019-11-28 15:28 week:4
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_CHARACTER_H_
#define _EASY_CHARACTER_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"
#include "Integer.hpp"

namespace Easy {

/**
 * \class Character
 * 
 * \brief An e_char wrapper.
 * 
 * @author Tes Alone
 * @since 1.0
 * */

class Character {
private:
	e_char m_ch;
public:
	/**
	 * \brief Max code point of Unicode BMP.
	*/
	const static e_char MAX = L'\uFFFF';

	/**
	 * \brief Min code point of Unicode BMP.
	*/
	const static e_char MIN = L'\u0000';
	
	/**
	 * \brief Sentinel character.
	 * 
	 * The character is Unicode Private Use Code Point '\\ue000';
	 * */
	const static Character& SENTINEL;
	
	/**
	 * \brief Sentinel character.
	 * 
	 * The character is Unicode Private Use Code Point '\\ue000';
	 * */
	const static e_char sentinel = L'\ue000';

	/**
	 * \brief ctor.
	 * 
	 * @param c Unicode code point, default L'\\u0000'
	*/
	Character(e_char c=L'\u0000') : m_ch(c) {}

	/**
		\brief Get char.
		
		@return receiver's char.
	*/
	inline e_char getChar() const {
		return m_ch;
	}
	
	/**
	 * \brief Check equality.
	 * 
	 * This method do not care about normalization.
	 * 
	 * @param other the compared object with receiver.
	 * @return receiver's char == other's char.
	*/
	inline e_bool equals(const Character& other) const {
		return Character::equals(m_ch, other.m_ch);
	}

	/**
	 * \brief Check order.
	 * 
	 * This method do not care about normalization.
	 * 
	 * @param other the compared object with receiver.
	 * @return if receiver's char bigger than other's, return 1; both equals return 0; otherwise return -1.
	*/
	inline e_int compare(const Character& other) const {
		return Character::compare(m_ch, other.m_ch);
	}

	/**
	 * \brief Get hash code.
	 * 
	 * @return receiver's hash code.
	 * */
	inline e_int hashCode() const {return Integer::hashCode(m_ch);}
	
	/**
	 * \brief To string．
	 * 
	 * @returns receiver's string represent.
	*/
	inline String toString() const {
		return System::move(Character::toString(m_ch));
	}

	/**
	 * \brief Check equality.
	 * 
	 * This method do not care about normalization.
	 * 
	 * @param c1 .
	 * @param c2 .
	 * @return c1 == c2.
	*/
	inline static e_bool equals(e_char c1, e_char c2) {
		return c1 == c2;
	}

	/**
	 * \brief Check order.
	 * 
	 * This method do not care about normalization.
	 * 
	 * @param c1 .
	 * @param c2 .
	 * @return c1==c2 ? 0 : (c1>c2?1:-1).
	*/
	inline static e_int compare(e_char c1, e_char c2) {
		return c1==c2 ? 0 : (c1>c2?1:-1);
	}
	
	/**
	 * \brief Get hash code.
	 * 
	 * @param c .
	 * @return c's hash code.
	*/
	inline static e_int hashCode(e_char c) {return Integer::hashCode(c);}

	/**
	 * \brief To string.
	 * 
	 * @param c .
	 * @return c's string represent.
	*/
	inline static String toString(e_char c) {
		e_char tmp[2] = {c, 0};
		return System::move(String{tmp});
	}

	/**
	 * \brief Check whether is a digit.
	 * 
	 * @param c  .
	 * @return c>='0' && c<='9'.
	*/
	inline static e_bool isDigit(e_char c) {
		return c>='0' && c<='9';
	}
	
	/**
	 * \brief Check whether is a hex digit.
	 * 
	 * @param c  .
	 * @return (c>='0'&&c<='9') || (c>='a'&&c<='f') || (c>='A'&&c<='F').
	*/
	inline static e_bool isHexDigit(e_char c) {
		return (c>='0'&&c<='9') || (c>='a'&&c<='f') || (c>='A'&&c<='F');
	}

	/**
	 * \brief Check whether is a letter.
	 * 
	 * <b>Just care English, Greek, Russia and CJK letter.</b>
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isLetter(e_char c) {
		return isUSLetter(c) || isGreekLetter(c) ||
				isRussiaLetter(c) || isHanLetter(c);
	}

	/**
	 * \brief Check whether is an upper case letter.
	 * 
	 * <b>Just care English, Greek, Russia letter.</b>
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isUpperCase(e_char c) {
		return (c>='A'&&c<='Z') || (c>=0x0391&&c<=0x03a9) || (c>=0x0410&&c<=0x042f);
	}

	/**
	 * \brief Check whether is a lower case letter.
	 * 
	 * <b>Just care English, Greek, Russia letter.</b>
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isLowerCase(e_char c) {
		return (c>='a'&&c<='z') || (c>=0x03b1 && c<=0x03c9) || (c>=0x0430&&c<=0x044f);
	}

	/**
	 * \brief To lower case.
	 * 
	 * if Character::isUpperCase(c) if false, return c.
	 * 
	 * @param c .
	 * @return c c's lower case.
	*/
	inline static e_char toLowerCase(e_char c) {
		return Character::isUpperCase(c) ? c+32 : c;
	}

	/**
	 * \brief To upper case.
	 * 
	 * if Character::isLowerCase(c) if false, return c.
	 * 
	 * @param c .
	 * @return c c's upper case.
	*/
	inline static e_char toUpperCase(e_char c) {
		return Character::isLowerCase(c) ? c-32 : c;
	}

	/**
	 * \brief Check whether is an English letter.
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isUSLetter(e_char c) {
		return (c>='a'&&c<='z') || (c>='A'&&c<='Z');
	}

	/**
	 * \brief Check whether is a Greek letter.
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isGreekLetter(e_char c) {
		return (c>=0x03b1 && c<=0x03c9) || (c>=0x0391&&c<=0x03a9) ;
	}

	/**
	 * \brief Check whether is a Russia letter.
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isRussiaLetter(e_char c) {
		return (c>=0x0430&&c<=0x044f) || (c>=0x0410&&c<=0x042f);
	}

	/**
	 * \brief Check whether is a CJK letter.
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isHanLetter(e_char c) {
		return c >= 0x4e00 && c <= 0x9fff;
	}

	/**
	 * \brief Check whether is a letter or a digit.
	 * 
	 * @param c  .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isLetterOrDigit(e_char c) {
		return isLetter(c) || isDigit(c);
	}

	/**
	 * \brief Check whether is a space.
	 * 
	 * Include: space, \\t, \\n, \\r, \\f, \\v.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isWhiteSpace(e_char c) {
		return c==' ' || c=='\t' || c=='\r' || c=='\n' || c=='\f' || c=='\v';
	}
	
	/**
	 * \brief Check whether is a control code.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isControlCode(e_int c) {
		return (c>=0 && c<=0x1f) || (c>=0x7f && c<=0x9f);
	}
	
	/**
	 * \brief Check whether is not a character.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isNonCharacter(e_int c) {
		return (c>=0xfdd0 && c<=0xfdef) 
					|| (c<=0x10ffff 
						&& ((c&0xffff)==0xfffe || (c&0xffff)==0xffff));
	}
	
	/**
	 * \brief Check whether is the replacement.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isReplaceChar(e_int c) {
		return c == 0xfffd;
	}
	
	/**
	 * \brief Get the replacement.
	 * 
	 * @return the replacement.
	*/
	inline static e_int getReplaceChar() {
		return 0xfffd;
	}
	
	/**
	 * \brief Check whether is a private used character.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isPrivateUsed(e_int c) {
		return (c>=0xe000&&c<=0xf8ff) 
					|| (c>=0xf0000&&c<=0xffffd)
						|| (c>=0x10000&&c<=0x10fffd);
	}
	
	/**
	 * \brief Check whether is a high surrogate char.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isHighSurrogate(e_char c) {
		return c>=0xd800 && c<=0xdbff;
	}
	
	/**
	 * \brief Check whether is a low surrogate char.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isLowSurrogate(e_char c) {
		return c>=0xdc00 && c<=0xdfff;
	}
	
	/**
	 * \brief Check whether is a surrogate pair.
	 * 
	 * @param c .
	 * @return if is return true, otherwise return false.
	*/
	inline static e_bool isSurrogatePair(e_int c) {
		return (isHighSurrogate((c>>16)&0xffff) && isLowSurrogate(c&0xffff)) 
					|| (isHighSurrogate(c&0xffff) && isLowSurrogate((c>>16)&0xffff));
	}
	
	/**
	 * \brief To digit.
	 * 
	 * @param c .
	 * @return if c is a digit, return c - '0'; if not, return -1 temporarily.
	*/
	inline static e_int toDigit(e_char c) {
		if (isDigit(c)) return c-'0';
		return -1; // FIXME 暂时
	}
	
	/**
	 * \brief To hex digit.
	 * 
	 * @param c .
	 * @return if c is a digit, return c-'0' or c-'a'+10; if not, return -1 temporarily.
	*/
	inline static e_int toHexDigit(e_char c) {
		if (isDigit(c)) return c-'0';
		if (isHexDigit(c)) return toUpperCase(c)-'A'+10;
		return -1; // FIXME 暂时
	}
}; // Character
} // namespace Easy

#endif // _EASY_CHARACTER_H_

