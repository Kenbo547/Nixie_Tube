
#ifndef __DS3231_H__
#define __DS3231_H__

#include <stdio.h>

#define RTC_WADDR 0b11010000
#define RTC_RADDR 0b11010001

struct rtc_time
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
};

struct rtc_date
{
	uint8_t wday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};

/*
 * Clock initialization
 */
void rtc3231_init(void);

/**
 * Reading time from clock
 * @time: time structure with reading data
 */
void rtc3231_read_time(struct rtc_time *time);

/**
 * Reading date from clock
 * @date: date structure with reading data
 */
void rtc3231_read_date(struct rtc_date *date);

/**
 * Reading date and time from clock
 * @time: time structure with reading data
 * @date: date structure with reading data
 */
void rtc3231_read_datetime(struct rtc_time *time, struct rtc_date *date);

/**
 * Writing new time in clock
 * @time: time structure for writing data
 */
void rtc3231_write_time(struct rtc_time *time);

/**
 * Writing new date in clock
 * @date: date structure for writing data
 */
void rtc3231_write_date(struct rtc_date *date);


#endif
