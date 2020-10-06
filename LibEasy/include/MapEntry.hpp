/*
 * File Name:	MapMapEntry.hpp
 * Date/Time:
 * 	Start: 	2019-12-14 19:53 week:6
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_MAPENTRY_H_
#define _EASY_MAPENTRY_H_

#include "String.hpp"

namespace Easy {

/**
	\class MapEntry
	
	\brief 用于 map 类的表项.
*/
template<class K, class V>
struct MapEntry {
	
	/**
		\brief 键.
	*/
	K m_key;
	
	/**
		\brief 值.
	*/
	V m_value;

	/**
		\brief 构造函数.
	*/
	MapEntry() {
		m_key   = K{};
		m_value = V{};
	}

	/**
		\brief 构造函数.
	*/
	MapEntry(const K& key, const V& value) {
		m_key   = key;
		m_value = value;
	}
	
	/**
		\brief 构造函数.
	*/
	MapEntry(const K& key, V&& value) {
		m_key   = key;
		m_value = System::move(value);
	}
	
	/**
		\brief 构造函数.
	*/
	MapEntry(K&& key, const V& value) {
		m_key   = System::move(key);
		m_value = value;
	}
	
	/**
		\brief 构造函数.
	*/
	MapEntry(K&& key, V&& value) {
		m_key   = System::move(key);
		m_value = System::move(value);
	}
	
	/**
		\brief 等于函数.

		K 需实现 equals() 方法.

		@param other 一个 MapEntry.
		@return 如果接受者的键等于 other 的键, 返回真; 否则返回假.
	*/
	inline e_bool equals(const MapEntry& other) const {
		return m_key.equals(other.m_key);
	}

	/**
		\brief 比较函数.

		K 需实现 compare() 方法.

		@param other 一个 MapEntry.
		@return 接受者的键与 other 的键的比较结果.
	*/
	inline e_int compare(const MapEntry& other) const {
		return m_key.compare(other.m_key);
	}

	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	inline e_int hashCode() const {return m_key.hashCode();}
	
	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	String toString() const {
		return System::move(m_key.toString() + String{L":"} + m_value.toString());
	}
}; // MapEntry
} // Easy

#endif // _EASY_MAPENTRY_H_

