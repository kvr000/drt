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

#include <dr/BString.hxx>
#include <dr/WString.hxx>
#include <dr/String.hxx>

#include <dr/tenv/tenv.hxx>

DR_NS_USE
DR_TENV_NS_USE

#define TEST_LOCK
#define TEST_OP
#define TEST_GSTR

#ifdef TEST_LOCK
TENV_NS(lock);
void test()
{
	char *b;
	BString s("bla");
	b = s.lock(9);
	strcpy(b, "diesesehy");
	s.unlock(6);
	DR_Assert(s == "dieses");
}
TENV_NSE(lock);
#endif

#ifdef TEST_OP
TENV_NS(op);
void test()
{
	BString s;
	s = "blediese"; s.removeLeft(3); DR_Assert(s == "diese");
	s = "blediese"; s.removeMid(3); DR_Assert(s == "ble");
	s = "blediese"; s.removeMid(3, 2); DR_Assert(s == "bleese");
	s = "    blekeke    "; s.trimSpaces(); DR_Assert(s == "blekeke");
	s = "        "; s.trimSpaces(); DR_Assert(s == "");
}
TENV_NSE(op);
#endif

#ifdef TEST_GSTR
TENV_NS(gstr);
void test()
{
	String s("testfile");
	s.wide().toStr();
	s.append(L"bl");
	s.utf8();
	s.wide();
	s.bcmp("bla");
	String sw("testw");
	s.utf8();
	s.wide();
	s.bcmp("bla");
}
TENV_NSE(gstr);
#endif

#ifdef TEST_STR
TENV_NS(str);
void test()
{
	{
		String sm(".");
		String ss("..");
		TENV_CHECK(sm.find(ss) < 0);
		TENV_CHECK(ss.find(sm) == 0);
		TENV_CHECK(ss.find(sm, 0) == 0);
		TENV_CHECK(ss.find(sm, 1) == 1);
	}
}
TENV_NSE(str);
#endif

int main()
{
	tenv_init();
#ifdef TEST_LOCK
	TENV_RUN(lock);
#endif
#ifdef TEST_OP
	TENV_RUN(op);
#endif
#ifdef TEST_GSTR
	TENV_RUN(gstr);
#endif
	return 0;
}
