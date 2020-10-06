/*
 * File Name:	Date.hpp
 * Date/Time:
 * 	Start: 	2019-11-28 16:58 week:4
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_DATE_H_
#define _EASY_DATE_H_

#include "BaseTypes.hpp"
#include "String.hpp"

namespace Easy {

/**
 *	\class Date
 *	
 * 	\brief Date/Time.
 * 
 * @author Tes Alone
 * @since 1.0
*/
class Date {

private:
	e_short milliseconds;
	e_short year;
	e_short month;
	e_short dayOfWeek;
	e_short day;
	e_short hour;
	e_short minute;
	e_short second;
public:

	/**
		\brief Locale flag for China.
	*/
	const static e_int CN = 1;
	
	/**
		\brief Locale flag for US.
	*/
	const static e_int US = 2;
	
	/**
	 * \brief Get current Date/Time.
	 * 
	 * @return current Date/Time.
	*/
	static Date now();

	/**
	 * \brief ctor.
	 * 
	 * Method just set all fields to 0.
	*/
	Date();

	/**
	 * \brief Check equality.
	 * 
	 * Compare receiver's all fields and other, if they are all equals return true,
	 * otherwise return false.
	 * 
	 * @param other .
	 * @return result.
	*/
	e_bool equals(const Date& other) const;	

	/**
	 * \brief Check order.
	 * 
	 * Compare from year to millisecond, if receiver is bigger, return 1, both equals
	 * return 0, otherwise return -1.
	 * 
	 * @param other .
	 * @return result.
	*/
	e_int  compare(const Date& other) const;
	
	/**
	 * \brief get hash code.
	 * 
	 * Get receiver`s hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	e_int hashCode() const;
	
	/**
	 * \brief To string represent．
	 * 
	 * @returns receiver`s string represent.
	*/
	String toString() const;
	
	/**
		\brief Format date/time info.
		
		For Date::CN, format is 'YYYY年MM月DD日 HH:MM:SS 周*',
		and '*' can be from '一' to '六' and '日'.
		
		For Date::US, format is 'YYYY/DD/MM HH:MM:SS *',
		and '*' can be from '0' to '6', and '0' for Sunday.
		
		@param locale locale info.
		@return receiver's formatted string represent.
		
		@throw if locale is not Date::CN or Date::US, throws IllegalArgumentException;
	*/
	String format(e_int locale) const;
	
	/**
	 * \brief Check whether is a leap-year.
	 * 
	 * Check whether receiver is a leap-year.
	 * 
	 * @return if receiver is a leap-year, return true, otherwise return false.
	 * */
	e_bool isLeapYear() const {
		return ((year%4==0) && ((year%100!=0) || (year%400==0)));
	}

	/**
	 * \brief Get milliseconds.
	 * 
	 * @return milliseconds, betweens 0 ~ 999.
	*/
	inline e_short getMilliseconds() const {return milliseconds;}
	
	/**
	 * \brief Set milliseconds.
	 * 
	 * @param millis the desired milliseconds, betweens 0 ~ 999.
	 * @throw if millis<0 or millis>999, throws IllegalArgumentsException.
	*/
	inline e_void setMilliseconds(e_short millis) {
		if (millis<0 || millis>999) 
			throw IllegalArgumentException{__func__, __LINE__};
		milliseconds = millis;
	}
	
	/**
	 * \brief Get year.
	 * 
	 * @return year.
	*/
	inline e_short getYear() const {return year;}
	
	/**
	 * \brief Set year.
	 * 
	 * Method do not do any checking for y.
	 * 
	 * @param y desired year.
	*/
	inline e_void setYear(e_short y) {year = y;}
	
	/**
	 * \brief Get month.
	 * 
	 * @return month, betweens 1 ~ 12.
	*/
	inline e_short getMonth() const  {return month;}
	
	/**
	 * \brief Set month.
	 * 
	 * @param mo desired mobth, betweens 1 ~ 12.
	 * @throw if mo<1 or mo>12, throws IllegalArgumentException.
	*/
	inline e_void setMonth(e_short mo) {
		if (mo<1 || mo>12)
			throw IllegalArgumentException{__func__, __LINE__};
		month = mo;
	}
	
	/**
	 * \brief Get day of weak.
	 * 
	 * @return day of week, betweens 0 ~ 6, 0 fo Sunday.
	*/
	inline e_short getDayOfWeek() const {return dayOfWeek;}
	
	/**
	 * \brief Set day of week.
	 * 
	 * @param dow desired day of week, betweens 0 ~ 6, 0 for Sunday.
	 * @throw if dow<0 of dow>6, throws IllegalArgumentException.
	*/
	inline e_void setDayOfWeek(e_short dow) {
		if (dow<0 || dow>6)
			throw IllegalArgumentException{__func__, __LINE__};
		dayOfWeek = dow;
	}
	
	/**
	 * \brief Get day of month.
	 * 
	 * @return day of month, betweens 1 ~ 31.
	*/
	inline e_short getDay() const {return day;}
	
	/**
	 * \brief Set day of month.
	 * 
	 * <b>Method care about month and day of the month's consistency, e.g. 
	 * a leap-year has a 29 days February</b>
	 * 
	 * @param d desired day of month, betweens 1 ~ 31.
	 * @throw if d is not a illegal day of the month, throws IllegalArgumentException.
	*/
	e_void setDay(e_short d) {
		switch (month) {
			case 4:
			case 6:
			case 9:
			case 11:
			{
				if (d<1 || d>30)
					throw IllegalArgumentException{__func__, __LINE__};
			}
			break;
			case 2:
			{
				if (isLeapYear() && (d<1||d>29))
					throw IllegalArgumentException{__func__, __LINE__};
				if (!isLeapYear() && (d<1||d>28))
					throw IllegalArgumentException{__func__, __LINE__};
			}
			break;
			default:
			{
				if (d<1 || d>31)
					throw IllegalArgumentException{__func__, __LINE__};
			}
			break;
		}
		
		day = d;
	}
	
	/**
	 * \brief Get hour.
	 * 
	 * @return hour, betweens 0 ~ 23.
	*/
	inline e_short getHour() const {return hour;}
	
	/**
	 * \brief Set hour.
	 * 
	 * @param h desired hour, betweens 0 ~ 23.
	 * @throw if h<0 or h>23, throws IllegalArgumentException.
	 */
	inline e_void setHour(e_short h) {
		if (h<0 || h>23)
			throw IllegalArgumentException{__func__, __LINE__};
		hour = h;
	}
	
	/**
	 * \brief Get minute.
	 * 
	 * @return minute, betweens 0 ~ 59.
	*/
	inline e_short getMinute() const {return minute;}

	/**
	 * \brief Set minute.
	 * 
	 * @param mi desired minute, betweens 0 ~ 59.
	 * @throw if mi<0 or mi>59, throws IllegalArgumentException.
	*/
	inline e_void setMinute(e_short mi) {
		if (mi<0 || mi>59)
			throw IllegalArgumentException{__func__, __LINE__};
		minute = mi;
	}
	
	/**
	 * \brief Get second.
	 * 
	 * @return second, betweens 0 ~ 59.
	*/
	inline e_short getSecond() const {return second;}

	/**
	 * \brief Set second.
	 * 
	 * @param s desired second, betweens 0 ~ 59.
	 * @throw if s<0 or s>59, throws IllegalArgumentException.
	*/
	inline e_void setSecond(e_short s) {
		if (s<0 || s>59)
			throw IllegalArgumentException{__func__, __LINE__};
		second = s;
	}
}; // Date
} // Easy

#endif // _EASY_DATE_H_

