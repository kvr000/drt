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

#include <dr/tenv/tenv.hxx>

DR_NS_USE
DR_TENV_NS_USE


TENV_NS(strind);

void run_find()
{
	String strings[] = { "bla", "kra", "sehy", "bflm" };
	String bla_str("bla");
	StringIndex si(sizeof(strings)/sizeof(strings[0]), strings);
	for (int i = 0; i < 32*1024*1024; i++) {
		si.find(bla_str);
	}
}

void run_findXchg()
{
	String strings[] = { "bla", "kra", "sehy", "bflm" };
	String bla_str("bla");
	StringIndex si(sizeof(strings)/sizeof(strings[0]), strings);
	for (int i = 0; i < 32*1024*1024; i++) {
		si.findXchg(bla_str);
	}
}

void test()
{
	printf("findXchg: %g\n", tenv_bench(&run_findXchg, 1)/1000000.0);
	printf("find: %g\n", tenv_bench(&run_find, 1)/1000000.0);
}

TENV_NSE(strind);

int main()
{
	tenv_init();
	TENV_RUN(strind);
	return 0;
}
