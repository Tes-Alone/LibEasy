
/*
 * File name: StringBuffer.hpp 
 *
 * Date/Time:
 *		start: 2019年10月04日 19时47分06秒 周5
 *		end:   2019年10月08日 15时06分28秒 周2
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_STRINGBUFFER_H_
#define LEC_STRINGBUFFER_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "Float.hpp"
#include "Bool.hpp"
#include "BasicArray.hpp"

namespace Easy {

/**
	\class StringBuffer
	
	\brief 字符串缓存工具.

	这是一个维护字符串的工具类, 它实现了字符串的增删改查操作,  它使用动态内存.
	
	@since 1.0
*/
	
class StringBuffer 
{

public:
	/**
		\brief 构造函数.
		
		@param capacity 缓存区的初始内存大小,默认为16, 以字符计.
	*/
	explicit StringBuffer(e_int capacity=16);
	
	/**
		\brief 析构函数.
	*/
	inline ~StringBuffer() {delete [] m_strBuf;}
	
	/**
		\brief copy 构造器.
		
		@param copy
	*/
	StringBuffer(const StringBuffer& copy);
	
	/**
		\brief move 构造器.
		
		@param move
	*/
	StringBuffer(StringBuffer&& move);
	
	/**
		\brief copy 赋值.
		
		@param copy
	*/
	StringBuffer& operator=(const StringBuffer& copy);
	
	/**
		\brief move 赋值.
		
		@param move
	*/
	StringBuffer& operator=(StringBuffer&& move);
	
	/**
	 * \brief 等于函数.
	 * 
	 * @param other 与接受者比较的对象.
	 * @return 如果接受者与 other 的元素分别对应相等, 返回真, 否则返回假.
	 * */
	
	e_bool equals(const StringBuffer& other) const {
		if (m_length != other.m_length) return false;
		for (e_int i=0; i<m_length; i++) {
			if (m_strBuf[i]!=other.m_strBuf[i]) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * \brief 比较函数.
	 * 
	 * @param other 与接受者比较的对象.
	 * @return 如果接受者的元素按顺序大于 other 的元素, 返回 1; 等于返回 0; 小于返回 -1.
	 * */
	e_int compare(const StringBuffer& other) const {
		e_int len = Math::min(this->m_length, other.m_length);

	    for (e_int i=0; i<len; i++){
	    	e_int flag = m_strBuf[i]-other.m_strBuf[i];
	        if (flag > 0)
	            return 1;
	        if (flag < 0)
	            return -1;
	    }
	
	    if (this->m_length == other.m_length)
	        return 0;
	
	    return (this->m_length > other.m_length) ? 1 : -1;
	}
	
	/**
	 * \brief 获取哈希值.
	 * 
	 * @return 接受者的哈希值.
	 * */
	e_int hashCode() const {
		return Long::hashCode(reinterpret_cast<e_long>(this));
	}
	
	/**
		\brief 转到字符串.
		
		@return 接受者目前所拥有的字符序列.
	*/
	inline String toString() const {
		return System::move(String{m_strBuf});
	}
	
	/**
		\brief 追加字符串.
		
		从接受者尾部添加字符串. str 可以为空串.
		
		如果 start == end, end 会加 1.
		使用的范围包括 start， 但不包括 end.
		
		@param str 要添加的字符串.
		@param start str 的开始位置,默认为 0, 范围为 0 ~ str的长度-1.
		@param end  str 的结束位置,默认为 -1, 代表  str的长度; 范围为 0 ~ str的长度.
		@return 完成添加后的接受者.
		
		@throw 如果 start < 0 或 start >= str的长度, 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > str的长度, 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException. 
	*/
	StringBuffer& append(const String& str, e_int start=0, e_int end=-1);
	
	/**
		\brief 追加字符串缓存.
		
		从接受者尾部添加字符串. buf 可以为空串.
		
		如果 start == end, end 会加 1.
		使用的范围包括 start， 但不包括 end.
		
		@param buf 要添加的字符串缓存.
		@param start buf 的开始位置,默认为 0, 范围为 0 ~ str的长度-1.
		@param end  buf 的结束位置,默认为 -1, 代表  str的长度; 范围为 0 ~ str的长度.
		@return 完成添加后的接受者.
		
		@throw 如果 buf 就是接受者, 抛出 IllegalArgumentException.
		@throw 如果 start < 0 或 start >= buf 的长度, 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > buf的长度, 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException. 
	*/
	StringBuffer& append(const StringBuffer& buf, e_int start=0, e_int end=-1);
	
	/**
	 * \brief 添加字符数组.
	 * 
	 * @param buf 要添加的字符数组.
	 * @param len 添加的长度.
	 * @return 添加了 buf 的前 len 个字符的接受者.
	 * @throw 如果 buf.length() < len, 抛出 IllegalArgumentException.
	*/
	StringBuffer& append(const CharArray& buf, e_int len);
	
	/**
		\brief 追加任意对象.
		
		T 需要实现 toString() 方法.
		
		@param t 要追加的对象.
		@return 完成添加后的接受者.
	*/
	template<class T>
	inline StringBuffer& append(const T& t) {
		return append(System::move(t.toString()));
	}
	
	/**
		\brief 缩减缓存区大小至正好容纳现有字符串.
		
		@return 缩减内存后的接受者.
	*/
	StringBuffer& trimToLength();
	
	/**
		\brief 获取字符串长度.
		
		@return 接受者的字符串长度.		
	*/
	inline e_int length() const {return m_length;}
	
	/**
		\brief 获取指定位置的字符.
		
		@param index 字符所在位置, 范围为 0 到 StringBuffer::length() -1.
		@return index 位置的字符.
		@throw 如果 index < 0 或者 index >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	inline e_char& charAt(e_int index) {
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, index};
		return m_strBuf[index];
	}
	
	/**
		\brief 获取指定位置的字符.
		
		@param index 字符所在位置, 范围为 0 到 StringBuffer::length() -1.
		@return index 位置的字符.
		@throw 如果 index < 0 或者 index >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	inline e_char& operator[](e_int index) {
		return charAt(index);
	}
	
	/**
		\brief 获取内存大小.
		
		@return 接受者目前使用的内存大小, 以字符大小计.
	*/
	inline e_int capacity() const {	return m_capacity;}
	
	
	/**
		\brief 获取 C 风格字符串.
		
		不要 delete 返回值.

		@return 接受者的 C 风格字符串.
	*/
	inline const e_char* toCStr() const {return m_strBuf;}	
	
	/**
		\brief 删除前缀.
		
		@param head 前缀结束位置, 范围为 0 到 StringBuffer::length() -1.
		@return 删除前缀后的接受者.
		
		@throw 如果 head < 0 或 head >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	StringBuffer& removeHead(e_int head);
	
	/**
		\brief 删除后缀.
		
		@param tail 后缀开始位置, 范围为 0 到 StringBuffer::length() -1.
		@return 删除后缀后的接受者.
		
		@throw 如果 tail < 0 或 taile >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	inline StringBuffer& removeTail(e_int tail)
	{		
		if (tail<0 || tail>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"StringBuffer"}, m_length, tail};
			
		m_strBuf[tail] = '\0';
		m_length = tail;

		return *this;
	}
	
	/**
		\brief 删除子串.
		
		如果 start == end, end 会加 1.
		使用的范围包括 start， 但不包括 end.
		
		@param start 删除的开始位置,默认为 0, 范围为 0 ~ StringBuffer::length() -1.
		@param end   删除的结束位置,默认为 -1, 代表  StringBuffer::length(), 范围为 0 ~ StringBuffer::length().
		@return 删除子串后的接受者.
		
		@throw 如果 start < 0 或 start >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.
	*/
	StringBuffer& remove(e_int start=0, e_int end=-1);
	
	/**
		\brief 插入字符串.
		
		@param str 欲插入的字符串,可以为空字符串.
		@param offset 插入的位置,范围为 0 到 StringBuffer::length().
		@return 插入 str 后的接受者.
		
		@throw 如果 offset < 0 或 offset > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	StringBuffer& insert(const String& str, e_int offset);
	
	/**
		\brief 插入字符串缓存.
		
		@param buf 欲插入的字符串缓存,可以为空字符串.
		@param offset 插入的位置,范围为 0 到 StringBuffer::length().
		@return 插入 buf 后的接受者.
		@throw 如果 buf 就是接受者, 抛出 IllegalArgumentException.
		@throw 如果 offset < 0 或 offset > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	StringBuffer& insert(const StringBuffer& buf, e_int offset);
	
	/**
		\brief 插入字符数组.
		
		@param buf 欲插入的字符数组,可以为空字符数组.
		@param offset 插入的位置,范围为 0 到 StringBuffer::length().
		@return 插入 buf 后的接受者.
		
		@throw 如果 offset < 0 或 offset > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	StringBuffer& insert(const CharArray& buf, e_int offset);
	
	/**
		\brief 插入指定对象的字符串表示.
		
		@param t
		@param offset
		@return 插入 t 后的接受者.
		@throw 如果 offset < 0 或 offset > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/
	template<class T>
	inline StringBuffer& insert(const T& t, e_int offset) {
		return insert(System::move(t.toString()), offset);
	}
	
	/**
		\brief 替换子串.
		
		如果 start == end, end 会加 1.
		使用的范围包括 start， 但不包括 end.
		
		如果 str 为空串, 等同于 StringBuffer::remove(start, end).
		
		@param str 用于替换的新字符串, 可以为空串.
		@param start 替换的开始位置,默认为 0, 范围为 0 ~ StringBuffer::length() -1.
		@param end  替换的结束位置,默认为 -1, 代表  StringBuffer::length(), 范围为 0 ~ StringBuffer::length().
		@return 替换子串后的接受者.
		
		@throw 如果 start < 0 或 start >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.

	*/
	StringBuffer& replace(const String& str, e_int start=0, e_int end=-1);
	
	/**
		\brief 替换子串.
		
		如果接受者不包含 oldWord, 或 oldWord 为空串, 函数什么都不做.
		<br/>
		如果 newWord 为空串, 函数删除接受者中的 oldWord.
		
		@param oldWord 要被替换的子串, 可以为空串.
		@param newWord 用于替换的字符串, 可以为空串.
		@return 替换子串后的接受者.
	*/
	StringBuffer& replace(const String& oldWord, const String& newWord);
	
	/**
		\brief 反转字符串.
		
		@return 接受者的反转版本.
	*/
	StringBuffer& reverse();
	
	/**
		\brief 获取子串.
		
		如果 start == end, end 会加 1.
		使用的范围包括 start， 但不包括 end.
		
		@param start 子串的开始位置,默认为 0, 范围为 0 ~ StringBuffer::length() -1.
		@param end  子串的结束位置,默认为 -1, 代表  StringBuffer::length(), 范围为 0 ~ StringBuffer::length().
		@return 接受者从 start 到 end-1 所构成的子串.
		
		@throw 如果 start < 0 或 start >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > StringBuffer::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.	
	*/
	String substring(e_int start, e_int end=-1) const;

	/**
		\brief 查找子串.
		
		如果 str 为空串, 返回 offset.
		如果接受者不包含 str, 返回 -1 .
		
		@param str 要查找的子串.
		@param offset 查找开始位置, 默认为0, 范围为 0 到 StringBuffer::length() -1.
		@return offset 之后 str 第一次出现的位置.
		
		@throw 如果 offset < 0 或 offset >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.		
	*/
	e_int indexOf(const String& str, e_int offset=0) const;
	
	/**
		\brief 倒序查找子串.
		
		如果 str 为空串, 返回 offset.
		如果接受者不包含 str, 返回 -1 .<br/>
		
		<b>注意:</b> 函数不会匹配 offset 之后的内容.
		
		@param str 要查找的字串.
		@param offset 查找开始位置, 默认为-1, 代表 StringBuffer::length() -1,范围为 0 到StringBuffer::length() -1.
		@return offset 之前 str 最后一次出现的位置.
		
		@throw 如果 offset < 0 或 offset >= StringBuffer::length(), 抛出 IndexOutOfBoundsException.
	*/	
	e_int lastIndexOf(const String& str, e_int offset=-1) const;
	
	
	/**
		\brief 检测是否包含字符串.
		
		如果 str 为空串, 返回真.
		
		@param str 欲查找的字符串, 可以为空串.
		@return 如果接受者包含 str, 返回真; 否则, 返回假.
	*/	
	inline e_bool contains(const String& str) const {
		if (str.empty()) return true;
		return indexOf(str, 0)==-1 ? false : true;		
	}
	
	/**
		\brief 检测是否是空串.
		
		@return 如果接受者长度为0, 返回真; 否则, 返回假.
	*/
	inline e_bool empty() const {return m_length == 0;}
	
	
	/**
		\brief 清空缓存.
		
		@return 清空缓存后的接受者.
	*/
	inline StringBuffer& clean() {
		delete [] m_strBuf;
		m_capacity = 16;
		m_length   = 0;
		m_strBuf   = new e_char[16];
		return *this;
	}
	
	/**
		\brief 判断接受者是否以给定字符串结尾.
		
		如果 chs 为空串, 返回真.
		
		@param chs 用于比较的字符串, 可以为空串.
		@return 如果 chs 是接受者的后缀, 返回真; 否则, 返回假.
	*/
	e_bool endsWith(const String& chs) const ;
	
	
	/**
		\brief 判断接受者是否以给定字符串开始.
		
		如果 chs 为空串, 返回真.
			
		@param chs 用于比较的字符串, 可以为空串.
		@return 如果 chs 是接受者的前缀, 返回真; 否则, 返回假.
	*/
	e_bool startsWith(const String& chs) const ;
private:
	/*
		所有的操作必须保证:
			m_strBuf 最少有 16 个空间,
			m_capacity 最少为 16,
			m_length 必须 > 0, 且 < m_capacity. 
	*/
	e_char* m_strBuf;
	e_int   m_length;
	e_int 	m_capacity;
	e_void _updateCapa(e_int newCapa);
	e_void _append(const e_char* chs, e_int len, e_int start, e_int end);
	e_void _remove(e_int resultLen, e_int moveLen, e_int start, e_int end);
	e_void _insert(const e_char* chs, e_int len, e_int offset);
	e_int _indexOf(const e_char* chs, e_int len, e_int offset) const ;
	e_int _lastIndexOf(const e_char* chs, e_int len, e_int offset) const ;
};

} // Easy


#endif // LEC_STRINGBUFFER_H_