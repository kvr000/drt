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

#include <dr/testenv/testenv.hxx>
#include <dr/Ref.hxx>

#include <dr/sql/ConnectionPool.hxx>

DR_NS_USE
DR_SQL_NS_USE
DR_TESTENV_NS_USE


int main(void)
{
	ERef<ConnectionPool> connpool(new ConnectionPool("driver=drSql_mysql5;host=localhost;port=3306;db=dr_test;user=dr_test;pass=dr_test", 1));
	connpool->setMaxOldness(1);
	ERef<ConnectionHold> conn1(connpool->getConnectionPing());
	ERef<ConnectionHold> conn2(connpool->getConnectionPing());
	connpool->releaseConnection(conn1); conn1.setNull();
	test_sleep(1);
	conn1.setNoref(connpool->getConnectionPing());
	return 0;
}
