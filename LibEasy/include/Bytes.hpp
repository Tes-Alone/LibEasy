/*
 * 文件名: Bytes.hpp 
 *
 * 日期/时间:
 *		开始: 2020年08月19日 22:20:41 周三
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_BYTES_H_
#define _EASY_BYTES_H_

#include "BaseTypes.hpp"
#include "BasicArray.hpp"

namespace Easy {

/**
 * \class Bytes
 * 
 * \brief A pure tool class deals with converting between basic types and bytes.
 * */
class Bytes {
	
public:
	static e_short toShort(const UByteArray& bytes) {
		e_short result = 0;
		result |= bytes[1] << 8;
		result |= bytes[0];
		return result;
	}
	
	static e_ushort toUShort(const UByteArray& bytes) {
		e_ushort result = 0;
		result |= bytes[1] << 8;
		result |= bytes[0];
		return result;
	}
	
	static e_uint toUInt(const UByteArray& bytes) {
		e_uint result = 0;
		result |= bytes[3] << 24;
		result |= bytes[2] << 16;
		result |= bytes[1] << 8;
		result |= bytes[0];
		return result;
	}
	
	static e_int toInt(const UByteArray& bytes) {
		e_int result = 0;
		result |= bytes[3] << 24;
		result |= bytes[2] << 16;
		result |= bytes[1] << 8;
		result |= bytes[0];
		return result;
	}
	
	static e_ulong toULong(const UByteArray& bytes) {
		e_ulong result = 0;
		result |= (e_long)bytes[7] << 56;
		result |= (e_long)bytes[6] << 48;
		result |= (e_long)bytes[5] << 40;
		result |= (e_long)bytes[4] << 32;
		result |= bytes[3] << 24;
		result |= bytes[2] << 16;
		result |= bytes[1] << 8;
		result |= bytes[0];
		return result;
	}
	
	static e_long toLong(const UByteArray& bytes) {
		e_long result = 0;
		result |= (e_long)bytes[7] << 56;
		result |= (e_long)bytes[6] << 48;
		result |= (e_long)bytes[5] << 40;
		result |= (e_long)bytes[4] << 32;
		result |= bytes[3] << 24;
		result |= bytes[2] << 16;
		result |= bytes[1] << 8;
		result |= bytes[0];
		return result;
	}
	
	static ByteArray toBytes(e_short s) {
		ByteArray result{2};
		result[0] = s & 0xff;
		result[1] = (s >> 8) & 0xff;
		return System::move(result);
	}
	
	static ByteArray toBytes(e_ushort s) {
		ByteArray result{2};
		result[0] = s & 0xff;
		result[1] = (s >> 8) & 0xff;
		return System::move(result);
	}
	
	static UByteArray toUBytes(e_short s) {
		UByteArray result{2};
		result[0] = s & 0xff;
		result[1] = (s >> 8) & 0xff;
		return System::move(result);
	}
	
	static UByteArray toUBytes(e_ushort s) {
		UByteArray result{2};
		result[0] = s & 0xff;
		result[1] = (s >> 8) & 0xff;
		return System::move(result);
	}
	
	static ByteArray toBytes(e_int i) {
		ByteArray result{4};
		result[0] = i & 0xff;
		result[1] = (i >> 8) & 0xff;
		result[2] = (i >> 16) & 0xff;
		result[3] = (i >> 24) & 0xff;
		return System::move(result);
	}
	
	static UByteArray toUBytes(e_int i) {
		UByteArray result{4};
		result[0] = i & 0xff;
		result[1] = (i >> 8) & 0xff;
		result[2] = (i >> 16) & 0xff;
		result[3] = (i >> 24) & 0xff;
		return System::move(result);
	}
	
	static ByteArray toBytes(e_uint i) {
		ByteArray result{4};
		result[0] = i & 0xff;
		result[1] = (i >> 8) & 0xff;
		result[2] = (i >> 16) & 0xff;
		result[3] = (i >> 24) & 0xff;
		return System::move(result);
	}
	
	static UByteArray toUBytes(e_uint i) {
		UByteArray result{4};
		result[0] = i & 0xff;
		result[1] = (i >> 8) & 0xff;
		result[2] = (i >> 16) & 0xff;
		result[3] = (i >> 24) & 0xff;
		return System::move(result);
	}
	
	static ByteArray toBytes(e_long l) {
		ByteArray result{8};
		for (e_int i=0; i<result.length(); i++) {
			result[i] = (l >> (i * 8)) & 0xff;
		}
		return System::move(result);
	}
	
	static UByteArray toUBytes(e_long l) {
		UByteArray result{8};
		for (e_int i=0; i<result.length(); i++) {
			result[i] = (l >> (i * 8)) & 0xff;
		}
		return result;
	}
	
	static ByteArray toBytes(e_ulong l) {
		ByteArray result{8};
		for (e_int i=0; i<result.length(); i++) {
			result[i] = (l >> (i * 8)) & 0xff;
		}
		return System::move(result);
	}
	
	static UByteArray toUBytes(e_ulong l) {
		UByteArray result{8};
		for (e_int i=0; i<result.length(); i++) {
			result[i] = (l >> (i * 8)) & 0xff;
		}
		return result;
	}
	
private:
	Bytes()  = delete;
	~Bytes() = delete;
	Bytes(const Bytes& copy) = delete;
	Bytes& operator=(const Bytes& copy) = delete;
	Bytes(Bytes&& copy) = delete;
	Bytes& operator=(Bytes&& copy) = delete;
}; // BytesHelper
	
} // namespace Easy


#endif // _EASY_BYTES_H_
