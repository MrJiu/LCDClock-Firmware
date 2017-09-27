/*
 * time.c
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#include <stm32f3xx.h>
#include <stm32f303xc_it.h>

#include <dreamos-rt/time.h>

#define _COMPILING_NEWLIB
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

#include <errno.h>
#undef errno
extern int errno;

/*
 * SysTick-based application run time clock.
 */

__attribute__((section(".ccm"))) uint32_t millis_counter = 0;

__attribute__((constructor)) void systick_init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}

__attribute__((section(".ccmcode"))) void SysTick_Handler(void)
{
	millis_counter++;
}

__attribute__((section(".ccmcode"), weak)) void yield(void) {}

uint32_t millis(void)
{
	return millis_counter;
}

uint32_t micros(void)
{
	uint32_t ms = millis_counter;
	uint32_t ticks;
	do
		ticks = SysTick->LOAD - SysTick->VAL;
	while (ms != millis_counter);

	return ms * 1000 + (ticks / (SystemCoreClock / 1000000));
}

unsigned sleep(unsigned seconds)
{
	int32_t ms_end = millis_counter + seconds * 1000;

	while (ms_end - (int32_t)millis_counter > 0)
		yield();

	return 0;
}

int usleep(useconds_t useconds)
{
	uint32_t ms = millis_counter;
	uint32_t ticks;
	do
		ticks = SysTick->VAL;
	while (ms != millis_counter && ticks > 20);
	ms = millis_counter;

	int32_t ms_end = ms + (useconds / 1000);
	int32_t ticks_end = ticks - (useconds % 1000) * (SystemCoreClock / 1000000);
	if (ticks_end < 0)
	{
		ms_end++;
		ticks_end += SysTick->LOAD;
	}

	while (ms_end - (int32_t)millis_counter > 0)
		yield();
	while ((int32_t)SysTick->VAL - ticks_end > 0)
		yield();

	return 0;
}

/*
 * Real time clock
 */

char *strptime(const char *restrict buf, const char *restrict format, struct tm *restrict timeptr);
void yield(void);

static inline void rtc_enable(void)
{
	__disable_irq();
	RTC->WPR = 0xca;
	__DSB();
	RTC->WPR = 0x53;
	__DSB();
	__enable_irq();
}

static inline void rtc_disable(void)
{
	__DSB();
	RTC->WPR = 0xff;
	__DSB();
}

__attribute__((constructor)) void rtc_init(void)
{
	// If we don't have a clock ready, set it to the compile time of this file.
	if (!(RTC->ISR & RTC_ISR_INITS))
	{
		rtc_enable();
		RTC->CR |= RTC_CR_COE;
		rtc_disable();

		struct tm tm = {0};
		struct timeval timeval = {0};
		strptime(__DATE__ " " __TIME__, "%b %d %Y %H:%M:%S", &tm);
		timeval.tv_sec = mktime(&tm);
		settimeofday(&timeval, NULL);
	}
}

clock_t _times(struct tms *tm)
{
	if (tm)
	{
		tm->tms_utime = millis() / (1000 / CLK_TCK);
		tm->tms_stime = 0;
		tm->tms_cutime = 0;
		tm->tms_cstime = 0;
	}

	return 0;
}

#define BKP_CENTURY	BKP0R
#define BKP_TZ		BKP1R
#define BKP_DST		BKP2R

static inline int bcd2int(uint8_t bcd)
{
	uint8_t high_nibble = (bcd >> 4) & 0xf;
	uint8_t low_nibble = bcd & 0xf;

	if (high_nibble > 9 || low_nibble > 9)
		return -EDOM;

	return high_nibble * 10 + low_nibble;
}

static inline int int2bcd(uint8_t val)
{
	if (val > 99)
		return -EDOM;

	uint8_t high_nibble = val / 10;
	uint8_t low_nibble = val % 10;

	return high_nibble << 4 | low_nibble;
}

int _gettimeofday(struct timeval *tm, void *tz)
{
	if (!(RTC->ISR & RTC_ISR_INITS))
	{
		errno = ENOTSUP;
		return -1;
	}

	if (tm)
	{
		while (!(RTC->ISR & RTC_ISR_RSF))
			yield();

		uint32_t SSR = RTC->SSR;
		uint32_t TR = RTC->TR;
		uint32_t DR = RTC->DR;
		uint32_t PREDIV = RTC->PRER & RTC_PRER_PREDIV_S;

		struct tm t =
		{
				.tm_sec = bcd2int(TR & 0xff),
				.tm_min = bcd2int((TR & 0xff00) >> 8),
				.tm_hour = bcd2int((TR & 0x3f0000) >> 16) + ((TR & RTC_TR_PM) ? 12 : 0),
				.tm_mday = bcd2int(DR & 0xff),
				.tm_mon = bcd2int((DR & 0x1f00) >> 8) - 1,
				.tm_year = bcd2int((DR & 0xff0000) >> 16) + RTC->BKP_CENTURY * 100,
				.tm_isdst = (RTC->CR & RTC_CR_BCK) ? 1 : 0
		};

		tm->tv_sec = mktime(&t);
		tm->tv_usec = (PREDIV - SSR) * 1000000 / (PREDIV + 1);
	}

	if (tz)
	{
		struct timezone *z = tz;
		z->tz_minuteswest = RTC->BKP_TZ;
		z->tz_dsttime = RTC->BKP_DST;
	}

	return 0;
}

int settimeofday(const struct timeval *tm, const struct timezone *tz)
{
	rtc_enable();

	if (tm)
	{
		RTC->ISR |= RTC_ISR_INIT;
		while (!(RTC->ISR & RTC_ISR_INITF))
			yield();

		RTC->PRER = 32767;

		struct tm t = {0};
		if (!gmtime_r(&(tm->tv_sec), &t))
		{
			rtc_disable();
			return -1;
		}

		SET_FIELD(RTC->CR, RTC_CR_FMT | RTC_CR_BCK, (t.tm_isdst) ? RTC_CR_BCK : 0); // 24-hour format
		RTC->DR =
				int2bcd(t.tm_year % 100) << 16 |
				(t.tm_wday ?: 7) << 13 |
				int2bcd(t.tm_mon + 1) << 8 |
				int2bcd(t.tm_mday);
		RTC->TR =
				int2bcd(t.tm_hour) << 16 |
				int2bcd(t.tm_min) << 8 |
				int2bcd(t.tm_sec);
		RTC->BKP_CENTURY = t.tm_year / 100;

		RTC->ISR &= ~(RTC_ISR_INIT | RTC_ISR_RSF);
	}

	if (tz)
	{
		RTC->BKP_TZ = tz->tz_minuteswest;
		RTC->BKP_DST = tz->tz_dsttime;
	}

	rtc_disable();
	return 0;
}
