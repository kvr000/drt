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

#include <dr/x_kw.hxx>
#include <dr/Mem.hxx>
#include <dr/MutexCond.hxx>
#include <dr/DataNotFoundException.hxx>

#include <dr/testenv/testenv.hxx>

#include <dr/ThreadSimple.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


#define TEST_TMCACHE

#ifdef TEST_TMCACHE
TESTNS(tmcache);

static MutexCond *wcond = MutexCond::create();
unsigned running = 0;
RList<Thread> exited;

static void allocateBuffers()
{
	void *allocs[1024-32];
#if 1
	for (size_t i = 1; i < sizeof(allocs)/sizeof(allocs[0]); i++) {
		allocs[i] = MM::alloc(i);
	}
	for (size_t i = 1; i < sizeof(allocs)/sizeof(allocs[0]); i++) {
		MM::free(allocs[i]);
	}
#endif
	MM::flushThreadCache();
	wcond->lock();
	running--;
	//exited.append(IRef<Thread>(Thread::current()));
	wcond->signal();
	wcond->unlock();
}

void test()
{
	for (int i = 0; i < 16*1024*1024; i++) {
		wcond->lock();
		while (running > 8) {
			wcond->wait();
		}
		while (exited.count())
			exited.removeFirst();
		ThreadSimple::go(Eslot0<void>(&allocateBuffers))->unref();
		running++;
		wcond->unlock();
		if ((i+1)%10000 == 0)
			Fatal::plog("ran thread %d\n", i);
	}
	while (running > 0 || exited.count()) {
		wcond->lock();
		if (running > 0) {
			wcond->wait();
		}
		while (exited.count())
			exited.removeFirst()->wait();
		wcond->unlock();
	}
}

TESTNSE(tmcache);
#endif

DR_TESTENV_MAIN()
{
	test_init();
	MM::enableThreadCache(1);
#ifdef TEST_TMCACHE
	TESTRUN(tmcache);
#endif
	return 0;
}
