/**
 * drt library
 *
 * drt multiplatform development toolkit
 *
 * ----------------------------------------------------------------------------------
 *
 * Copyright (C) 2004-2008 Zbyněk Vyškovský
 *
 * ----------------------------------------------------------------------------------
 *
 * LICENSE:
 *
 * This file is part of drt
 *
 * drt is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * drt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with drt; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @copyright	2004-2008 Zbyněk Vyškovský
 * @link	mailto:kvr@matfyz.cz
 * @link	http://kvr.matfyz.cz/drt/
 * @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
 **/

#include <time.h>
#ifdef DR_LIBC_VC
# include <windows.h>
#else
#include <sys/time.h>
#endif

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Exception.hxx>

#include <dr/Time.hxx>

#ifdef DR_LIBC_VC
static void gettimeofday(struct timeval *tv, struct timeval *unused)
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	dr::Uint64 epoch_time = ((((dr::Uint64)ft.dwHighDateTime<<32)|ft.dwLowDateTime)-116444736000000000ULL)/10;
	tv->tv_sec = epoch_time/1000000;
	tv->tv_usec = epoch_time%1000000;
}
#endif

DR_NS_BEGIN


SysTime Time::getTime()
{
	if (SEC_BASE == 1) {
		return time(NULL)*SEC_BASE;
	}
	else {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec*SEC_BASE+tv.tv_usec/(1000000/SEC_BASE);
	}
}

SysTime Time::getTimePrecSec()
{
	return time(NULL)*SEC_BASE;
}

void Time::setInvalid(SysTime *time)
{
	*time = INVAL_TIME;
}

SysTime Time::convertUtcTime(int year, int mon, int day, int hour, int min, int sec, int timezone_min)
{
	struct tm t;
	t.tm_year = year-1900;
	t.tm_mon = mon;
	t.tm_mday = day+1;
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = sec;
#ifdef DR_LIBC_VC
	__time64_t tt = _mkgmtime64(&t);
#else
	time_t tt = timegm(&t);
#endif
	if (tt < 0)
		return tt;
	return (tt+timezone_min*60)*SEC_BASE;
}

SysTime Time::convertUtcTimeMsec(int year, int mon, int day, int hour, int min, int sec, int msec, int timezone_min)
{
	struct tm t;
	t.tm_year = year-1900;
	t.tm_mon = mon;
	t.tm_mday = day+1;
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = sec;
#ifdef DR_LIBC_VC
	__time64_t tt = _mkgmtime64(&t);
#else
	time_t tt = timegm(&t);
#endif
	if (tt < 0)
		return tt;
	return fromSeconds(tt+timezone_min*60)+interFromMseconds(msec);
}

void Time::timeToUtcCalendar(SysTime tvalue, int *year, int *mon, int *day, int *hour, int *min, int *sec)
{
	time_t tt = (time_t)toSeconds(tvalue);
	struct tm t;
	if (
#ifdef DR_LIBC_VC
		_gmtime64_s(&t, &tt) != 0
#else
		gmtime_r(&tt, &t) == NULL
#endif
		) {
		xthrownew(Exception());
	}
	*year = t.tm_year+1900;
	*mon = t.tm_mon;
	*day = t.tm_mday-1;
	*hour = t.tm_hour;
	*min = t.tm_min;
	*sec = t.tm_sec;
}

void Time::timeToUtcCalendarMsec(SysTime tvalue, int *year, int *mon, int *day, int *hour, int *min, int *sec, int *msec)
{
	time_t tt = (time_t)toSeconds(tvalue);
	struct tm t;
	if (
#ifdef DR_LIBC_VC
		_gmtime64_s(&t, &tt) != 0
#else
		gmtime_r(&tt, &t) == NULL
#endif
		) {
		xthrownew(Exception());
	}
	*year = t.tm_year+1900;
	*mon = t.tm_mon;
	*day = t.tm_mday-1;
	*hour = t.tm_hour;
	*min = t.tm_min;
	*sec = t.tm_sec;
	*msec = fractMsecs(tvalue);
}

bool Time::sleepTime(Time::SysTime sleep_time)
{
#ifdef DR_LIBC_VC
	Sleep(Time::interToMsecondsUp(sleep_time));
	return true;
#else
	struct timespec ts;
	ts.tv_sec = Time::interToSeconds(sleep_time);
	ts.tv_nsec = Time::interToNsecondsUp(sleep_time)%1000000000;
	return nanosleep(&ts, NULL) == 0;
#endif
}

bool Time::sleepSec(Sint64 secs)
{
#ifdef DR_LIBC_VC
	Sleep(secs*1000);
	return true;
#else
	struct timespec ts;
	ts.tv_sec = secs;
	ts.tv_nsec = 0;
	return nanosleep(&ts, NULL) == 0;
#endif
}

bool Time::sleepNSec(Sint64 nsecs)
{
#ifdef DR_LIBC_VC
	Sleep((nsecs+999999)/1000000);
	return true;
#else
	struct timespec ts;
	ts.tv_sec = nsecs/1000000000;
	ts.tv_nsec = nsecs%1000000000;
	return nanosleep(&ts, NULL) == 0;
#endif
}


DR_NS_END
