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

#ifndef dr__Time__hxx__
# define dr__Time__hxx__

#include <dr/types.hxx>

DR_NS_BEGIN


/**
 * Time functions
 * 
 * the class is intended to convert memory into basic types
 */
class DR_PUB Time
{
public:
	typedef Sint64			SysTime;

public:
	static const SysTime SEC_BASE	= 1000;
	static const SysTime INVAL_TIME	= (((SysTime)-1)<<(sizeof(SysTime)*8-1));

public:
	/**
	 * gets the current time
	 */
	static SysTime			getTime();

	/**
	 * gets the current time with second precision
	 */
	static SysTime			getTimePrecSec();

public:
	static void			setInvalid(SysTime *time);

public:
	/**
	 * Compute time from Epoch seconds (i.e. 1970-01-01 00:00:00 UTC)
	 */
	static SysTime			fromSeconds(SysTime seconds)		{ return seconds != INVAL_TIME ? seconds*SEC_BASE : INVAL_TIME; }

	/**
	 * Convert time to Epoch seconds (i.e. 1970-01-01 00:00:00 UTC)
	 */
	static SysTime			toSeconds(SysTime time)			{ return time != INVAL_TIME ? time/SEC_BASE : INVAL_TIME; }

	/**
	 * Convert time to Epoch seconds (i.e. 1970-01-01 00:00:00 UTC)
	 */
	static SysTime			toSecondsUp(SysTime time)		{ return time != INVAL_TIME ? (time+SEC_BASE-1)/SEC_BASE : INVAL_TIME; }

	/**
	 * Compute time from Epoch seconds (i.e. 1970-01-01 00:00:00 UTC)
	 */
	static SysTime			fromMseconds(SysTime mseconds)		{ return mseconds != INVAL_TIME ? mseconds*SEC_BASE/1000 : INVAL_TIME; }

	/**
	 * Convert time to Epoch seconds (i.e. 1970-01-01 00:00:00 UTC)
	 */
	static SysTime			toMseconds(SysTime time)		{ return time != INVAL_TIME ? time*1000/SEC_BASE : INVAL_TIME; }

	/**
	 * Get miliseconds fraction from time
	 */
	static unsigned			fractMsecs(SysTime time)		{ return ((time%SEC_BASE)*1000)/SEC_BASE; }

	/**
	 * Get microseconds fraction from time
	 */
	static unsigned			fractUsecs(SysTime time)		{ return ((time%SEC_BASE)*1000000)/SEC_BASE; }

	/**
	 * Get nanoseconds fraction from time
	 */
	static unsigned			fractNsecs(SysTime time)		{ return ((time%SEC_BASE)*1000000000)/SEC_BASE; }

	/**
	 * Get interval time in seconds
	 */
	static SysTime			interToSeconds(SysTime time)		{ return time/SEC_BASE; }

	/**
	 * Get interval time in seconds, rounding up
	 */
	static SysTime			interToSecondsUp(SysTime time)		{ return (time+SEC_BASE-1)/SEC_BASE; }

	/**
	 * Get interval time in miliseconds
	 */
	static SysTime			interToMseconds(SysTime time)		{ return time*1000/SEC_BASE; }

	/**
	 * Get interval time in miliseconds, rounding up
	 */
	static SysTime			interToMsecondsUp(SysTime time)		{ return (time*1000+SEC_BASE-1)/SEC_BASE; }

	/**
	 * Get interval time in miliseconds
	 */
	static SysTime			interToUseconds(SysTime time)		{ return time*1000000/SEC_BASE; }

	/**
	 * Get interval time in miliseconds, rounding up
	 */
	static SysTime			interToUsecondsUp(SysTime time)		{ return (time*1000000+SEC_BASE-1)/SEC_BASE; }

	/**
	 * Get interval time in miliseconds
	 */
	static SysTime			interToNseconds(SysTime time)		{ return time*1000000000/SEC_BASE; }

	/**
	 * Get interval time in miliseconds, rounding up
	 */
	static SysTime			interToNsecondsUp(SysTime time)		{ return (time*1000000000+SEC_BASE-1)/SEC_BASE; }


	/**
	 * Get interval value from seconds
	 */
	static SysTime			interFromSeconds(SysTime time)		{ return (time*SEC_BASE); }

	/**
	 * Get interval value from seconds, rounding up
	 */
	static SysTime			interFromSecondsUp(SysTime time)	{ return (time*SEC_BASE); }

	/**
	 * Get interval value from miliseconds
	 */
	static SysTime			interFromMseconds(SysTime time)		{ return (time*SEC_BASE)/1000; }

	/**
	 * Get interval value from miliseconds, rounding up
	 */
	static SysTime			interFromMsecondsUp(SysTime time)	{ return (time*SEC_BASE+999)/1000; }

	/**
	 * Get interval value from microseconds
	 */
	static SysTime			interFromUseconds(SysTime time)		{ return (time*SEC_BASE)/1000000; }

	/**
	 * Get interval value from microseconds, rounding up
	 */
	static SysTime			interFromUsecondsUp(SysTime time)	{ return (time*SEC_BASE+999999)/1000000; }

	/**
	 * Get interval value from microseconds
	 */
	static SysTime			interFromNseconds(SysTime time)		{ return (time*SEC_BASE)/1000000000; }

	/**
	 * Get interval value from microseconds, rounding up
	 */
	static SysTime			interFromNsecondsUp(SysTime time)	{ return (time*SEC_BASE+999999999)/1000000000; }


	/**
	 * Compute time from date values.
	 *
	 * All values are zero based.
	 */
	static SysTime			convertUtcTime(int year, int mon, int day, int hour, int min, int sec, int timezone_min);
	static SysTime			convertUtcTimeMsec(int year, int mon, int day, int hour, int min, int sec, int msec, int timezone_min);

	/**
	 * Convert time to data values.
	 *
	 * All values are zero base.
	 */
	static void			timeToUtcCalendar(SysTime tvalue, int *year, int *mon, int *day, int *hour, int *min, int *sec);
	static void			timeToUtcCalendarMsec(SysTime tvalue, int *year, int *mon, int *day, int *hour, int *min, int *sec, int *msec);
};

typedef Time::SysTime SysTime;


DR_NS_END

#endif
