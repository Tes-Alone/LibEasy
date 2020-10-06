/*
 * 文件名: EasyC.cpp 
 *
 * 日期/时间:
 *		开始: 2020年06月04日 13:57:46 周四
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#include "EasyC.hpp"
#include "Exception.hpp"

static e_void memmove_forward(e_void* des, e_void* src, e_int count);
static e_void memmove_backward(e_void* des, const e_void* src, e_int count);

e_void e_memmove(e_void* des, e_void* src, e_int count) {
	if (des > src) {
		memmove_forward(des, src, count);
	} else {
		memmove_backward(des, (const e_void*)src, count);
	}
}

e_void e_memcopy(e_void* des, const e_void* src, e_int count) {
	memmove_backward(des, src, count);
}

static e_void memmove_backward(e_void* des, const e_void* src, e_int count) {
	e_long* d = (e_long*) des;
	e_long* s = (e_long*) src;
	e_int l = count / 8;
	for (e_int i=0; i<l; i++) {
		d[i] = s[i];
	}
	e_byte* db = (e_byte*) des;
	e_byte* sb = (e_byte*) src;
	e_int tailOffset = l * 8;
	for (e_int i=tailOffset; i<count; i++) {
		db[i] = sb[i];
	}
}

static e_void memmove_forward(e_void* des, e_void* src, e_int count) {
	e_int headLen = count % 8;
	e_ubyte* wholeStartOfSrc = ((e_ubyte*)src) + headLen;
	e_ubyte* wholeStartOfdes = ((e_ubyte*)des) + headLen;
	e_int wholeLen = (count - headLen) / 8;
	e_long* srcLong = (e_long*)wholeStartOfSrc;
	e_long* desLong = (e_long*)wholeStartOfdes;
	for (e_int i=wholeLen-1; i>=0; i--) {
		desLong[i] = srcLong[i];
	}
	e_ubyte* srcByte = (e_ubyte*)src;
	e_ubyte* desByte = (e_ubyte*)des;
	for (e_int i=headLen-1; i>=0; i--) {
		desByte[i] = srcByte[i];
	}
}
