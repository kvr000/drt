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

#include <dr/ThreadSimple.hxx>

#include <dr/testenv/testenv.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


#define TEST_SERIAL

#ifdef TEST_SERIAL
TESTNS(serial);

void sender_thread(Thread *orig, int serial)
{
	printf("sender_thread: %p\n", Thread::current());
	test_sleep(2);
	printf("sending serial %d\n", serial);
	orig->addAnswer(serial);
}

void test()
{
	Thread *sender;
	int serial;
	
	printf("init_thread: %p\n", Thread::current());

	serial = Thread::createMessageSerial();

	sender = ThreadSimple::go(Eslot(&sender_thread).a2Set(serial).a1Set<IRef<Thread> >(Thread::current()));

	test_sleep(1);

	Thread::processMessages(serial);

	sender->waitUnref();

	printf("done\n");
}
TESTNSE(serial);
#endif

int main()
{
	test_init();
#ifdef TEST_SERIAL
	TESTRUN(serial);
#endif
	return 0;
}
