/*
 * File Name:	Date.cpp
 * Date/Time:
 * 	Start: 	2019-11-28 16:58 week:4
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#include <windows.h>

#include "String.hpp"
#include "String.tcc"
#include "Integer.hpp"
#include "Date.hpp"

namespace Easy {

Date::Date() {
	milliseconds = 0;
	year 	  = 0;
	month 	  = 0;
	day 	  = 0;
	hour      = 0;
	minute    = 0;
	second    = 0;
	dayOfWeek = 0;
}

Date Date::now() {
	SYSTEMTIME st;
    GetLocalTime(&st);
	Date d{};
	d.milliseconds = st.wMilliseconds;
	d.dayOfWeek    = st.wDayOfWeek;
	d.hour		   = st.wHour;
	d.minute	   = st.wMinute;
	d.month	       = st.wMonth;
	d.second	   = st.wSecond;
	d.year	       = st.wYear;
	d.day		   = st.wDay;
	return d;
}

e_bool Date::equals(const Date& other) const {
	return  milliseconds == other.milliseconds &&
			dayOfWeek    == other.dayOfWeek    && 
			year   == other.year   && 
			month  == other.month  &&
			day    == other.day    &&
			hour   == other.hour   &&
			minute == other.minute &&
			second == other.second;
}

e_int Date::compare(const Date& other) const {
	if (year > other.year)
		return 1;
	if (year < other.year)
		return -1;

	if (month > other.month)
		return 1;
	if (month < other.month)
		return -1;

	if (day > other.day)
		return 1;
	if (day < other.day)
		return -1;
	
	if (hour > other.hour)
		return 1;
	if (hour < other.hour)
		return -1;

	if (minute > other.minute)
		return 1;
	if (minute < other.minute)
		return -1;
	
	if (second > other.second)
		return 1;
	if (second < other.second)
		return -1;
	
	if (milliseconds > other.milliseconds)
		return 1;
	if (milliseconds < other.milliseconds)
		return -1;
	
	return 0;
}

String Date::toString() const {
	String format{L"%4s-%02s-%02s %02s:%02s:%02s %s"};
	return System::move(String::format(format, Integer{year},
									Integer{month},
									Integer{day},
									Integer{hour},
									Integer{minute},
									Integer{second},
									Integer{dayOfWeek}));
}

static const e_char CNWeek[7][3] = {
	L"周日", L"周一", L"周二", L"周三", L"周四",
	L"周五", L"周六"
};

static const e_char USWeek[7][4] = {
	L"Sun", L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat"
};

String Date::format(e_int locale) const {
	String format, week;
	if (locale == Date::CN) {
		format = String{L"%4s年%02s月%02s日 %02s:%02s:%02s %s"};
		week   = String{CNWeek[dayOfWeek]};
		return System::move(String::format(format, Integer{year},
									Integer{month},
									Integer{day},
									Integer{hour},
									Integer{minute},
									Integer{second},
									week));
	} else if (locale == Date::US) {
		format = String{L"%4s/%02s/%02s %02s:%02s:%02s %s"};
		week   = String{USWeek[dayOfWeek]};
		return System::move(String::format(format, Integer{year},
									Integer{day},
									Integer{month},
									Integer{hour},
									Integer{minute},
									Integer{second},
									week));
	} else {
		throw IllegalArgumentException{__func__, __LINE__};
	}
}

e_int Date::hashCode() const {
	return Math::abs(Integer::hashCode(year) + 
			Integer::hashCode(month) +
			Integer::hashCode(day) +
			Integer::hashCode(hour) +
			Integer::hashCode(minute) +
			Integer::hashCode(second) +
			Integer::hashCode(dayOfWeek));
}

} // Easy