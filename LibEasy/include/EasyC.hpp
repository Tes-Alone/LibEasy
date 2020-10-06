/*
 * 文件名: EasyC.hpp 
 *
 * 日期/时间:
 *		开始: 2020年06月04日 13:53:52 周四
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_EASYC_H_
#define _EASY_EASYC_H_

#include "BaseTypes.hpp"

using namespace Easy;

e_void e_memmove(e_void* des, e_void* src, e_int count);
e_void e_memcopy(e_void* des, const e_void* src, e_int count);

#endif // _EASY_EASYC_H_
