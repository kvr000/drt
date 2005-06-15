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

#include <dr/ThreadSimple.hxx>
#include <dr/String.hxx>

#include <dr/tenv/tenv.hxx>

DR_NS_USE
DR_TENV_NS_USE


#define TEST_FORMAT

#ifdef TEST_FORMAT
TENV_NS(format);

#define TEST_FORMAT_BREAK() printf("\n")
#define TEST_FORMAT_FLT(fstr) printf("%s: =%s="fstr"=\n", fstr, (const char *)String::formats(fstr, 53253240.0).ascii(), 53253240.0)
#define TEST_FORMAT_NUM(fstr) printf("%s: =%s="fstr"=\n", fstr, (const char *)String::formats(fstr, 1234).ascii(), 1234)
#define TEST_FORMAT_STR(fstr) printf("%s: =%s="fstr"=\n", fstr, (const char *)String::formats(fstr, "foobar").ascii(), "foobar")

void test()
{
	TEST_FORMAT_NUM("%d");
	TEST_FORMAT_NUM("%o");
	TEST_FORMAT_NUM("%x");
	TEST_FORMAT_NUM("%X");
	TEST_FORMAT_BREAK();
	//TEST_FORMAT_NUM("%#d");
	TEST_FORMAT_NUM("%#o");
	TEST_FORMAT_NUM("%#x");
	TEST_FORMAT_NUM("%#X");
	TEST_FORMAT_BREAK();
	TEST_FORMAT_NUM("%.6d");
	TEST_FORMAT_NUM("%.6o");
	TEST_FORMAT_NUM("%.6x");
	TEST_FORMAT_NUM("%.6X");
	TEST_FORMAT_BREAK();
	//TEST_FORMAT_NUM("%#.6d");
	TEST_FORMAT_NUM("%#.6o");
	TEST_FORMAT_NUM("%#.6x");
	TEST_FORMAT_NUM("%#.6X");
	TEST_FORMAT_BREAK();
	TEST_FORMAT_NUM("%6.6d");
	TEST_FORMAT_NUM("%6.6o");
	TEST_FORMAT_NUM("%6.6x");
	TEST_FORMAT_NUM("%6.6X");
	TEST_FORMAT_BREAK();
	TEST_FORMAT_NUM("%8.6d");
	TEST_FORMAT_NUM("%8.6o");
	TEST_FORMAT_NUM("%8.6x");
	TEST_FORMAT_NUM("%8.6X");
	TEST_FORMAT_BREAK();
	TEST_FORMAT_NUM("%06d");
	TEST_FORMAT_NUM("%06o");
	TEST_FORMAT_NUM("%06x");
	TEST_FORMAT_NUM("%06X");
	TEST_FORMAT_BREAK();
	TEST_FORMAT_NUM("%-6d");
	TEST_FORMAT_NUM("%-#6o");
	TEST_FORMAT_NUM("%-#6x");
	TEST_FORMAT_NUM("%-#6X");
	TEST_FORMAT_BREAK();

	TEST_FORMAT_FLT("%-6.2f");
	TEST_FORMAT_FLT("%-6.2e");
	TEST_FORMAT_FLT("%-106.202g");
	TEST_FORMAT_BREAK();

	TEST_FORMAT_STR("%s");
	TEST_FORMAT_BREAK();
}
TENV_NSE(format);
#endif

int main()
{
	tenv_init();
#ifdef TEST_FORMAT
	TENV_RUN(format);
#endif
	return 0;
}
