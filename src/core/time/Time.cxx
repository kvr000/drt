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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Except.hxx>

#include <dr/Time.hxx>

DR_NS_BEGIN


SysTime Time::getTime()
{
	return time(NULL);
}

SysTime Time::convertUTime(int year, int mon, int day, int hour, int min, int sec, int timezone_min)
{
	struct tm t;
	t.tm_year = year-1900;
	t.tm_mon = mon;
	t.tm_mday = day+1;
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = sec;
	time_t tt = timegm(&t);
	if (tt < 0)
		return tt;
	return tt+timezone_min*60;
}

void Time::timeToUtcCalendar(SysTime tvalue, int *year, int *mon, int *day, int *hour, int *min, int *sec)
{
	time_t tt = tvalue;
	struct tm t;
	if (gmtime_r(&tt, &t) == NULL) {
		xthrownew(Except());
	}
	*year = t.tm_year+1900;
	*mon = t.tm_mon;
	*day = t.tm_mday-1;
	*hour = t.tm_hour;
	*min = t.tm_min;
	*sec = t.tm_sec;
}


DR_NS_END
