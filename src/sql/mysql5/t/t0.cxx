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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Exception.hxx>
#include <dr/tenv/tenv.hxx>

#include <dr/sql/tenv/CommonTest.hxx>

DR_SQL_NS_USE

static Ref<dr::sql::ConnectionPool> db_conn_pool;


TENV_NS(init);
void test()
{
	db_conn_pool.setNoref(new ConnectionPool("driver=dr::sql::mysql5;host=localhost;port=3306;db=dr_test;user=dr_test;pass=dr_test", 1));
}
TENV_NSE(init)

TENV_NS(common);
void test()
{
	tref(new dr::sql::tenv::CommonTest(db_conn_pool))->test();
}
TENV_NSE(common);

int main(void)
{
	TENV_RUN(init);
	TENV_RUN(common);
	db_conn_pool.setNull();

	return 0;
}
