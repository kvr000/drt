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

#include <dr/Alloc.hxx>
#include <dr/Shared.hxx>
#include <dr/Mutex.hxx>
#include <dr/MutexCond.hxx>
#include <dr/ThreadSimple.hxx>

#include <dr/testenv/TestObject.hxx>
#include <dr/testenv/testenv.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


void signalThread(MutexCond *mc)
{
	ERef<TestObject> obj(new TestObject(0));
	mc->lock();
	mc->signal();
	mc->unlock();
}

void destroyThreadTestObject(void *obj_)
{
	TestObject *obj = (TestObject *)obj_;

	Fatal::plog("destroying %ld\n", obj->getId());
	obj->unref();
}

int main(void)
{
	test_init();

	ERef<Mutex> m = Mutex::create();
	m->lock();
	m->unlock();

	ERef<MutexCond> mc = MutexCond::create();

	mc->lock();
	Thread *t = ThreadSimple::go(Eslot(&signalThread).a1Set(mc));
	mc->wait();
	printf("woke up\n");
	mc->unlock();
	t->waitUnref();
	return 0;
}
