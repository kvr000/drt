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

#include <dr/Const.hxx>
#include <dr/String.hxx>
#include <dr/ThreadSimple.hxx>

#include <dr/testenv/testenv.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


#define TEST_IFACE_STRING
#define TEST_IFACE_DYNCAST

#define TEST_IFACE_CNT (1024*1024*64)

class DR_EXPORT_CLS TestIf0: public Iface
{
	DR_IFACE_DECL(TestIf0);
};

class DR_EXPORT_CLS TestIf1: public Iface
{
	DR_IFACE_DECL(TestIf1);
};

class DR_EXPORT_CLS TestIf2: public Iface
{
	DR_IFACE_DECL(TestIf2);
};

class DR_EXPORT_CLS TestIfD: public TestIf1, public TestIf2
{
	DR_IFACE_DECL(TestIfD);
};

DR_IFACE_DEF("", TestIf0);
DR_IFACE_IMPL_SIMPLE(TestIf0);
DR_IFACE_DEF("", TestIf1);
DR_IFACE_IMPL_SIMPLE(TestIf1);
DR_IFACE_DEF("", TestIf2);
DR_IFACE_IMPL_SIMPLE(TestIf2);
DR_IFACE_DEF("", TestIfD);
DR_IFACE_IMPL_IFACE2(TestIfD, TestIf1, TestIf2);


class DR_EXPORT_CLS TestObj0: public Object, public TestIf0, public TestIfD
{
	DR_OBJECT_DECL(TestObj0, Object);
	DR_REDIR_BEHAV();
};

class DR_EXPORT_CLS TestObjTop: public TestObj0
{
	DR_OBJECT_DECL_SIMPLE(TestObjTop, TestObj0);
	DR_REDIR_BEHAV();
};

DR_OBJECT_DEF("", TestObj0, Object);
DR_OBJECT_IMPL_IFACE2(TestObj0, TestIf0, TestIfD);

DR_OBJECT_DEF("", TestObjTop, TestObj0);
DR_OBJECT_IMPL_SIMPLE(TestObjTop);

void check_res(void *);

#ifdef TEST_IFACE_STRING
TESTNS(iface_string);

void run_ref()
{
	TestIf0 *to = new TestObjTop;
	for (int i = 0; i < TEST_IFACE_CNT; i++) {
		TestIf1 *t = (TestIf1 *)to->getIface(TestIf1::iface_name);
		check_res(t);
		t->unref();
	}
	to->unref();
}

void run_unref()
{
	Iface *to = (Object *)new TestObjTop;
	for (int i = 0; i < TEST_IFACE_CNT; i++) {
		to = (Iface *)to->getIfaceUnref(TestIf1::iface_name);
		check_res(to);
	}
	to->unref();
}

void test()
{
	printf("iface_string_ref: %g\n", test_bench(&run_ref, 1)/1000000.0);
	printf("iface_string_unref: %g\n", test_bench(&run_unref, 1)/1000000.0);
}

TESTNSE(iface_string);
#endif

#ifdef TEST_IFACE_DYNCAST
TESTNS(iface_dyncast);

void run_ifup()
{
	TestIf1 *to = new TestObjTop;
	for (int i = 0; i < TEST_IFACE_CNT; i++) {
		TestIfD *t = dynamic_cast<TestIfD *>(to);
		check_res(t);
	}
	to->unref();
}

void run_objtop()
{
	Object *to = new TestObjTop;
	for (int i = 0; i < TEST_IFACE_CNT; i++) {
		TestObjTop *t = dynamic_cast<TestObjTop *>(to);
		check_res(t);
	}
	to->unref();
}

void run_objone()
{
	Object *to = new TestObjTop;
	for (int i = 0; i < TEST_IFACE_CNT; i++) {
		TestObj0 *t = dynamic_cast<TestObj0 *>(to);
		check_res(t);
	}
	to->unref();
}

void run_complex()
{
	TestIf0 *to = new TestObjTop;
	for (int i = 0; i < TEST_IFACE_CNT; i++) {
		TestIf1 *t = dynamic_cast<TestIf1 *>(to);
		check_res(t);
	}
	to->unref();
}

void test()
{
	printf("iface_dyncast_ifup: %g\n", test_bench(&run_ifup, 1)/1000000.0);
	printf("iface_dyncast_objtop: %g\n", test_bench(&run_objtop, 1)/1000000.0);
	printf("iface_dyncast_objone: %g\n", test_bench(&run_objone, 1)/1000000.0);
	printf("iface_dyncast_complex: %g\n", test_bench(&run_complex, 1)/1000000.0);
}

TESTNSE(iface_dyncast);
#endif

void check_res(void *)
{
}

int main()
{
	test_init();
#ifdef TEST_IFACE_STRING
	TESTRUN(iface_string);
#endif
#ifdef TEST_IFACE_DYNCAST
	TESTRUN(iface_dyncast);
#endif
	return 0;
}
