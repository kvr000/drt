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

/*drt
 * include:	dr/sql/tenv/def.hxx
 * include:	dr/tenv/TenvObject.hxx
 * include:	dr/sql/ConnectionPool.hxx
 *
 * ns:		dr::sql::tenv
 */

#include <dr/x_kw.hxx>
#include <dr/Exception.hxx>
#include <dr/sql/tenv/TestUnique.hxx>
#include <dr/sql/tenv/TestStatementSeek.hxx>

#include <dr/sql/tenv/CommonTest.hxx>
#include "_gen/CommonTest-all.hxx"

DR_SQL_TENV_NS_BEGIN


/*drt
 * class:	CommonTest
 * ancestor:	dr::tenv::TenvObject
 *
 * at:	Ref<ConnectionPool>		db_pool;
 * at:	Ref<ConnectionHold>		db_conn;
 */

DR_MET(public)
CommonTest::CommonTest(ConnectionPool *db_pool_):
	db_pool(db_pool_, true),
	db_conn(db_pool->getConnectionPing(), false)
{
}

DR_MET(protected virtual)
CommonTest::~CommonTest()
{
}

DR_MET(public virtual)
void CommonTest::test(const String &arg DR_INIT(= Null()))
{
	tref(new TestUnique(db_conn))->test();
	tref(new TestStatementSeek(db_conn))->test();
	db_conn->commit();
}


DR_SQL_TENV_NS_END
