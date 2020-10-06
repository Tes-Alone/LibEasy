
/*
 * File name: String.hpp 
 *
 * Date/Time:
 *		start: 2019年10月03日 15时05分50秒 周4
 *		end:
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_STRING_H_
#define LEC_STRING_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "System.hpp"
#include "Math.hpp"
#include "BasicArray.hpp"

namespace Easy {

class Integer;
class Character;
class Bool;
class Long;
class Double;
class Float;

template<typename>
class Vector;

/**
	\class String
	
	\brief 字符串实现.
	
	Unicode 代码点序列实现.
	
	这是一个不可变类.
	
	<b>注意: </b>在文档中，如果需要区分 C 风格字符串和 String 类对象, 会称 C 风格字符串为 C 风格字符串.
	String 类对象为字符串.<br/>
	如果不需要区分, 统称为 “字符串”.
	
	@since 1.0
*/

class String
{

public:
	
	/**
	 * \brief 无参构造函数.
	 * 
	 * 用于集合类内部建立对象.
	*/
	inline String() {
		m_str    = new e_char[1];
		m_length = 0;
		m_str[m_length] = '\0';
	}
		
	/**
		\brief 构造函数.
		
		如果 str 长度为0, 方法构造一个空字符串.
		<br/>
		使用的位置包括 start 但不包括 end.
		<br/>
		如果 start 等于 end, end 会被加 1.
		
		@param str C 风格字符串.
		@param start str 开始位置, 默认为0, 范围在 0~(str 长度-1);
		@param end  str 结尾位置, 默认为 -1, 代表 str 的长度, 范围在 (0 ~ str 长度).
		@throw 如果 str 为空指针, 抛出 NullPointerException.
		@throw 如果 start < 0或 start >= str 长度, 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end  > str 长度, 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.
	*/
	String(const e_char* str, e_int start=0, e_int end=-1);
	
	/**
		\brief 根据多字节字符串建立对象.
		
		@param bytes 多字节字符串.
		@throw 如果 bytes 为空指针, 抛出 NullPointerException.
	*/
	String(const e_byte* bytes);
	
	/**
		\brief 析构函数.
	*/
	inline ~String() {delete [] m_str;}
	
	/**
		\brief copy 构造器.
		
		@param copy
	*/
	String(const String& copy);
	
	/**
		\brief move 构造器.
		
		@param move	
	*/
	String(String&& move);
	
	/**
		\brief copy 赋值.

		@param copy
	*/
	String& operator=(const String& copy);
	
	/**
		\brief move 赋值.

		@param move
	*/
	String& operator=(String&& move);
	
	/**
		\brief 等于函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者和 other 的字符串值逐字相等, 返回真; 否则, 返回假.
	*/
	e_bool equals(const String& other) const;

	/**
		\brief 比较函数.
		
		@param other 接受者的比较对象.
		@return 如果接受者的字符串按数值顺序大于 other 的字符串, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	e_int compare(const String& other) const ;

	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	e_int hashCode() const;
	
	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	inline String toString() const {return System::move(String{m_str});}
    
    /**
	 * \brief 连接字符串函数.
	 * 
	 * @param t 与接受者连接的对象.
	 * @return 在接受者后追加 t 的字符串表示的新字符串.
	*/
    template<class T>
    inline String operator+(const T& t) const {
    	return this->concat(t.toString());
    }
    
    /**
     * \brief 连接 C 风格字符串.
     * 
     * @param s1 C 风格字符串
     * @return 在接受者之后添加 s1 的新副本.
     * */
     inline String operator+(const e_char* s1) const {
     	return this->concat(String{s1});
     }
     
    /**
     * \brief 连接 C 风格字符串.
     * 
     * @param s1 C 风格字符串
     * @return 在接受者之后添加 s1 的新副本.
     * */
     inline String operator+(const e_byte* s1) const {
     	return this->concat(String{s1});
     }
    
    /**
     * \brief 倍增字符串.
     * 
     * 如果 times 为 0, 方法返回空串.
     * 
     * @param times 倍增次数, 可以为 0.
     * @return 接受者连接自己 times 后的新字符串.
     * @throw 如果 times 小于 0, 抛出 IllegalArgumentException.
     * */
     String operator*(int times) const;
    
	/**
		\brief 忽略大小写比较.
		
		@param other 接受者的比较对象, 可以为空串.
		@return 如果等于返回真, 否则返回假.
	*/
	e_bool equalsIgnoreCase(const String& other) const ;
	
	/**
		\brief 忽略大小写比较.
		
		@param other 接受者的比较对象, 可以为空串.
		@return 如果接受者较大, 返回1; 等于返回0; 较小返回 -1.
	*/
	e_int compareIgnoreCase(const String& other) const ;
	
	/**
		\brief 获取 C 风格字符串.
		
		不要 delete 返回值.

		@return 接受者的 C 风格字符串.
	*/
	inline const e_char* toCStr() const {return m_str;}	
	
	
	/**
		\brief 转到多字节字符串.
		
		@return 接受者对应的多字节字符串.
	*/
	ByteArray getBytes() const ;
	
	/**
		\brief 获取长度.
		
		@return 接受者的长度.
	*/
	inline e_int length() const {return m_length;}
	
	/**
		\brief 获取指定位置的字符.
		
		@param index 字符所在位置, 范围为 0 到 String::length() -1.
		@return index 位置的字符.
		@throw 如果 index < 0 或者 index >= String::length(), 抛出 IndexOutOfBoundsException.
	*/
	inline e_char charAt(e_int index) const	{
		if (index<0 || index>=m_length)
			throw  IndexOutOfBoundsException{__func__, __LINE__, String{L"String"}, m_length, index};
		
		return m_str[index];
	}

	/**
		\brief 获取指定位置的字符.
		
		@param index 字符所在位置, 范围为 0 到 String::length() -1.
		@return index 位置的字符.
		@throw 如果 index < 0 或者 index >= String::length(), 抛出 IndexOutOfBoundsException.
	*/
	inline e_char operator[](e_int index) const {
		return charAt(index);
	}
	
	/**
		\brief 判断接受者是否为空串.
		
		@return 接受者为空串,返回真; 否则, 返回假.
	*/
	inline e_bool empty() const {return m_length == 0;}
	
	/**
		\brief 获取子串.
		
		<b>如果接受者长度为0, 方法返回一个空字符串.</b>
		<br/>
		<br/>
		使用的位置包括 start 但不包括 end.
		<br/>
		如果 start 等于 end, end 会被加 1.
		
		@param start 接受者使用的开始位置, 默认为0, 范围为 (0 ~ String::length() -1);
		@param end  接受者使用的结尾位置, 默认为 -1, 代表 String::length(), 范围为 (0 ~ String::length()).
		
		@return 接受者从 start 到 end-1 的子字符串.
		
		@throw 如果 start < 0 或 start>= String::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 end < 0 或 end > String::length(), 抛出 IndexOutOfBoundsException.
		@throw 如果 start > end, 抛出 IllegalArgumentException.
	*/
	String substring(e_int start=0, e_int end=-1) const ;
	
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

	/**
		\brief 转到大写字符串.
		
		@return 接受者对应的大写字符串.
	*/
	String toUpperCase() const ;

	/**
		\brief 转到小写字符串.
		
		@return 接受者对应的小写字符串.
	*/
	String toLowerCase() const ;
	
	/**
		\brief 查找子串.
		
		如果 sub 为空串, 返回 offset.
		如果接受者不包含 sub, 返回 -1.
		
		@param sub 子串, 可以为空串.
		@param offset 查找起始位置, 默认为 0, 范围为 0 到 String::length() -1.
		@return 接受者从 offset 开始, 第一个匹配 sub 的位置.
		
		@throw 如果 offset < 0 或 offset >= String::length(), 抛出 IndexOutOfBoundsException.
	*/
	e_int indexOf(const String& sub, e_int offset=0) const ;
	
		/**
		\brief 倒序查找子串.
		
		如果 sub 为空串, 返回 offset.
		如果接受者不包含 sub, 返回 -1.
		
		<b>注意:</b> 函数不会匹配 offset 之后的内容.
		
		@param sub 子串, 可以为空串.
		@param offset 查找起始位置, 默认为-1, 代表 String::length() -1 , 范围为 0 到 String::length() -1.
		@return offset 之前 sub 最后一次出现的位置.
		
		@throw 如果 offset 小于 0 或 offset 大于等于 接受者长度, 抛出 IndexOutOfBoundsException.
	*/
	e_int lastIndexOf(const String& sub, e_int offset=-1) const ;

	/**
		\brief 删除首尾空白符.
		
		空白符包括: 空格、水平制表符、换行符、回车符、垂直制表符和进纸符.
		
		@return 删除首尾空白符的副本.
	*/
	String trim() const ;
	
	/**
		\brief 连接字符串.
		
		@param chs 欲连接的字符串, 可以为空串.
		@return 由接受者作为前缀, chs 作为后缀的新字符串.
	*/
	String concat(const String& chs) const ;
	
	/**
		\brief 判断是否包含子串.
		
		如果 chs 为空串, 返回真.
		
		@param chs 可以为空串
		@return 如果接受者包含 chs, 返回真; 否则, 返回假.
	*/
	inline e_bool contains(const String& chs) const {
		if (chs.empty()) return true;
		return indexOf(chs)==-1 ? false : true;
	}
	
	/**
		\brief 子串替换.
		
		如果 oldWord 为空串,或接受者不包括 oldWord, 返回的副本等于接受者.
		如果 newWord 为空串, 返回的副本等于接受者删除所有 oldWord 后的结果.
		
		@param oldWord 要被替换的子串, 可以为空串.
		@param newWord 用于替换的字符串, 可以为空串.
		@return 将接受者中的 oldWord 替换为 newWord 的副本.
	*/
	String replace(const String& oldWord, const String& newWord) const;
	
	/**
		\brief 反转字符串.
		
		@return 接受者的反转版本.
	*/
	String reverse() const;
	
	
	/**
		\brief 分割字符串.
		
		如果 sub 为空串, 返回值为只包含接受者的列表.
		
		@param sub 分割用的引子.
		@return 接受者被 sub 分割的所有部分.
	*/
	Vector<String> split(const String& sub) const;
	
	/**
		\brief 计算 C 风格字符串的长度.
		
		@param chs 
		@return chs 的长度.
		
		@throw 如果 chs 为空指针, 抛出 NullPointerException.
	*/
	static e_int length(const e_char* chs);

	/**
	 * \brief 格式化字符串.
	 * 
	 * 这个函数类似于 sprintf(), 但只使用两种格式说明符: %% 和 %%s.
	 * 
	 * %% 同 sprintf(), %%s 表示一个占位符, 代表一个字符串, 支持左对齐、填充0、指定长度的格式化方式.
	 * 
	 * 如, 在 Date 类的 toString() 方法中, 使用的格式化字符串为: "%4s年%02s月%02s日 %02s:%02s:%02s %s".
	 * 
	 * <b>注: 使用的 %%s 和 args 的个数应保持一致, 当然, 如果 %%s 多于 args, 程序不会访问未知内存, 其内部使用的 Vector 会抛出
	 * IndexOutOfBoundsException.</b>
	 * 
	 * @param str 字符串格式.
	 * @param args 被格式化参数, 需实现 toString() 方法.
	 * @return 格式化的字符串.
	*/
	template<class ...T>
	static String format(const String& str, const T&... args);

private:
	e_char* m_str;
	e_int   m_length;

	e_void _checkStartAndEnd(e_int& start, e_int &end, const String& func, e_int line) const;
};


} // namespace Easy

#endif // LEC_STRING_H_