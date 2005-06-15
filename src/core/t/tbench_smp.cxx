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

#include <dr/Const.hxx>
#include <dr/ThreadSimple.hxx>
#include <dr/Atomic.hxx>

#include <dr/tenv/tenv.hxx>

DR_NS_USE
DR_TENV_NS_USE


#define TEST_NOATOMIC
#define TEST_ATOMIC
#define TEST_CMPXCHG

#define TEST_COUNT (1024*1024*16)
#define TEST_PARALLEL 8

static volatile SintPtr atomic_int = 0;
static volatile int count_check = 0;

static void runParallels(void (*routine)(volatile SintPtr *), bool check_zero, bool separate_mem)
{
	Ref<ThreadSimple> threads[TEST_PARALLEL];
	void **addr = (void **)MM::alloc(TEST_PARALLEL*sizeof(void *));
	memset(addr, 0, TEST_PARALLEL*sizeof(void *));
	for (size_t i = 0; i < TEST_PARALLEL; i++) {
		threads[i].setNoref(ThreadSimple::go((void *(*)(void *))routine, &addr[i*separate_mem]));
	}
	for (size_t i = 0; i < TEST_PARALLEL; i++) {
		threads[i]->wait();
	}
	if (check_zero) {
		for (size_t i = 0; i < TEST_PARALLEL; i++) {
			TENV_CHECK(addr[i] == NULL);
		}
	}
	MM::free(addr);
}

#ifdef TEST_NOATOMIC
TENV_NS(noatomic);

void worker(volatile SintPtr *val)
{
	for (int i = 0; i < TEST_COUNT; i++) {
		++*val;
	}
	for (int i = 0; i < TEST_COUNT; i++) {
		--*val;
	}
}

void run_same()
{
	runParallels(&worker, false, false);
}

void run_sep()
{
	runParallels(&worker, true, true);
}

void test()
{
	printf("noatomic_same : %g\n", tenv_bench(&run_same, 1)/1000000.0);
	printf("noatomic_sep  : %g\n", tenv_bench(&run_sep, 1)/1000000.0);
}

TENV_NSE(noatomic);
#endif

#ifdef TEST_ATOMIC
TENV_NS(atomic);

void worker(volatile SintPtr *val)
{
	for (int i = 0; i < TEST_COUNT; i++) {
		Atomic::inc(val);
	}
	for (int i = 0; i < TEST_COUNT; i++) {
		Atomic::dec(val);
	}
}

void run_same()
{
	runParallels(&worker, true, false);
}

void run_sep()
{
	runParallels(&worker, true, true);
}

void test()
{
	printf("atomic_same   : %g\n", tenv_bench(&run_same, 1)/1000000.0);
	printf("atomic_sep    : %g\n", tenv_bench(&run_sep, 1)/1000000.0);
}

TENV_NSE(atomic);
#endif

#ifdef TEST_ATOMIC
TENV_NS(cmpxchg);

void worker(volatile SintPtr *val)
{
	for (int i = 0; i < TEST_COUNT; i++) {
		for (SintPtr o = *val; !Atomic::cmpxchg((void *volatile *)val, (void *)o, (char *)o+1); o = *val) ;
	}
	for (int i = 0; i < TEST_COUNT; i++) {
		for (SintPtr o = *val; !Atomic::cmpxchg((void *volatile *)val, (void *)o, (char *)o-1); o = *val) ;
	}
}

void run_same()
{
	runParallels(&worker, true, false);
}

void run_sep()
{
	runParallels(&worker, true, true);
}

void test()
{
	printf("cmpxchg_same   :  %g\n", tenv_bench(&run_same, 1)/1000000.0);
	printf("cmpxchg_sep    : %g\n", tenv_bench(&run_sep, 1)/1000000.0);
}

TENV_NSE(cmpxchg);
#endif

int main()
{
	tenv_init();
#ifdef TEST_NOATOMIC
	TENV_RUN(noatomic);
#endif
#ifdef TEST_ATOMIC
	TENV_RUN(atomic);
#endif
#ifdef TEST_CMPXCHG
	TENV_RUN(cmpxchg);
#endif
	return 0;
}
