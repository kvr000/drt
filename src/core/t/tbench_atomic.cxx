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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <dr/Const.hxx>
#include <dr/ThreadSimple.hxx>
#include <dr/Atomic.hxx>

#include <dr/testenv/testenv.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


#define TEST_ATOMIC_NOCHECK
#define TEST_ATOMIC_CHECK

#define TEST_ATOMIC_CNT (1024*1024*8)

static volatile SintPtr atomic_int = 0;
static volatile int count_check = 0;

#ifdef TEST_ATOMIC_NOCHECK
TESTNS(atomic_nocheck);

void run_noatomic()
{
	for (int i = 0; i < TEST_ATOMIC_CNT; i++) {
		++atomic_int;
		--atomic_int;
	}
}

void run_atomic()
{
	for (int i = 0; i < TEST_ATOMIC_CNT; i++) {
		Atomic::inc(&atomic_int);
		Atomic::dec(&atomic_int);
	}
}

void test()
{
	atomic_int = 0;
	printf("nocheck_noatomic: %g (%ld)\n", test_parbench(-1, &run_noatomic, 1)/1000000.0, (long)atomic_int);
	atomic_int = 0;
	printf("nocheck_atomic: %g (%ld)\n", test_parbench(-1, &run_atomic, 1)/1000000.0, (long)atomic_int);
}

TESTNSE(atomic_nocheck);
#endif

#ifdef TEST_ATOMIC_CHECK
TESTNS(atomic_check);

void run_noatomic()
{
	for (int i = 0; i < TEST_ATOMIC_CNT; i++) {
		if (++atomic_int == 0)
			count_check++;
		if (--atomic_int == -1)
			count_check++;
	}
}

void run_atomic()
{
	for (int i = 0; i < TEST_ATOMIC_CNT; i++) {
		if (!Atomic::incr(&atomic_int))
			count_check++;
		if (!Atomic::decr(&atomic_int))
			count_check++;
	}
}

void test()
{
	atomic_int = 0;
	printf("check_noatomic: %g (%ld)\n", test_parbench(-1, &run_noatomic, 1)/1000000.0, (long)atomic_int);
	atomic_int = 0;
	printf("check_atomic: %g (%ld)\n", test_parbench(-1, &run_atomic, 1)/1000000.0, (long)atomic_int);
}

TESTNSE(atomic_check);
#endif

int main()
{
	test_init();
#ifdef TEST_ATOMIC_NOCHECK
	TESTRUN(atomic_nocheck);
#endif
#ifdef TEST_ATOMIC_CHECK
	TESTRUN(atomic_check);
#endif
	return 0;
}
