/*
 * 文件名: TimeCounter.hpp 
 *
 * 日期/时间:
 *		开始: 2020年05月17日 23:01:10 周日
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_TIMECOUNTER_H_
#define _EASY_TIMECOUNTER_H_

#include <Windows.h>
#include "BaseTypes.hpp"
#include "SharedPointer.hpp"

namespace Easy {

/**
 * \class TimeCounter
 * 
 * \brief Time counter.
 * 
 * Counting elapsed time, by milliseconds.
 * */
class TimeCounter : public EnableSharedFromThis<TimeCounter>{
	
LARGE_INTEGER _start;
LARGE_INTEGER _stop;
LARGE_INTEGER _frequency;

public:
	/**
	 * \brief ctor.
	 * 
	 * Do some initialization.
	 * */
	inline TimeCounter() {
		::QueryPerformanceFrequency(&_frequency);
	}
	
	/**
	 * \brief start counting.
	 * 
	 * Start counting.
	 * */
	inline e_void start() {
		::QueryPerformanceCounter(&_start);
	}
	
	/**
	 * \brief stop counting.
	 * 
	 * Stop counting.
	 * */
	inline e_void stop() {
		::QueryPerformanceCounter(&_stop);
	}
	
	/**
	 * \brief get elapsed time.
	 * 
	 * Get elapsed time.
	 * 
	 * @return Elapsed time from start() to stop().
	 * */
	e_long getTimeMillis() {
		return (_stop.QuadPart - _start.QuadPart) * 1000.0 / _frequency.QuadPart;
	}
private:
	TimeCounter(TimeCounter& copy)=delete;
	TimeCounter(TimeCounter&& move)=delete;
	TimeCounter& operator=(TimeCounter& copy)=delete;
	TimeCounter& operator=(TimeCounter&& move)=delete;
}; // TimeCounter

} // Easy
#endif // _EASY_TIMECOUNTER_H_
