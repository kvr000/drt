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

#ifdef DR_OS_UNIX
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#endif
#ifdef DR_OS_SYMBIAN
#include <unistd.h>
#endif
#ifdef DR_OS_WNT
#include <windows.h>
#ifndef STDOUT_ERROR_HANDLE
# define STDOUT_ERROR_HANDLE ((DWORD)-12)
#endif
#endif

#include <dr/types.hxx>
#include <dr/Assert.hxx>

#include <dr/testenv/testenv.hxx>

DR_TESTENV_NS_BEGIN


#if (defined DR_OS_UNIX) && (!defined DR_OS_WNT)

void test_handle_req(int n)
{
	(void)n;
	Fatal::printFailed("");
}

#endif

void test_init()
{
#if (defined DR_OS_WCE)
	Fatal::behavOutput(CreateFileW(L"test.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL));
#endif
	Fatal::setDebug("ts");
#if (defined DR_OS_UNIX) && (!defined DR_OS_WNT)
	signal(SIGINT, &test_handle_req);
#endif
}

long test_bench(const Eslot0<void> &run, long count)
{
	int i;
#if (defined DR_OS_UNIX) && (!defined DR_OS_WNT)
	struct rusage ru0, ru1;
	getrusage(RUSAGE_SELF, &ru0);
#elif (defined DR_OS_WNT)
	DWORD t0, t1;
	t0 = GetTickCount();
#else
#endif
	for (i = 0; i < count; i++)
		run();
#if (defined DR_OS_UNIX) && (!defined DR_OS_WNT)
	getrusage(RUSAGE_SELF, &ru1);
	return (ru1.ru_utime.tv_sec-ru0.ru_utime.tv_sec)*1000000+(int)(ru1.ru_utime.tv_usec-ru0.ru_utime.tv_usec)+(ru1.ru_stime.tv_sec-ru0.ru_stime.tv_sec)*1000000+(int)(ru1.ru_stime.tv_usec-ru0.ru_stime.tv_usec);
#elif (defined DR_OS_WNT)
	t1 = GetTickCount();
	return (long)(t1-t0)*1000;
#else
	return 0;
#endif
}

void test_abort()
{
	Fatal::printFailed("");
	abort();
}

DR_TESTENV_PUB void test_sleep(long time)
{
#ifdef DR_OS_WNT
	Sleep(time*1000);
#else
	sleep(time);
#endif
}


DR_TESTENV_NS_END
