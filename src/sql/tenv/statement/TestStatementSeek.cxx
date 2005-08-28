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
 * include:	dr/sql/tenv/DbSample0.hxx
 * include:	dr/tenv/TenvObject.hxx
 *
 * ns:		dr::sql::tenv
 */

#include <dr/x_kw.hxx>
#include <dr/tenv/tenv.hxx>
#include <dr/Exception.hxx>

#include <dr/sql/tenv/TestStatementSeek.hxx>
#include "_gen/TestStatementSeek-all.hxx"

DR_SQL_TENV_NS_BEGIN


/*drt
 * class:	TestStatementSeek
 * ancestor:	dr::tenv::TenvObject
 *
 * at:	Ref<DbSample0>			db_sample0;
 */

DR_MET(public)
TestStatementSeek::TestStatementSeek(ConnectionHold *db_conn_):
	db_sample0(DbSample0::accCached(db_conn_), true)
{
}

DR_MET(protected virtual)
TestStatementSeek::~TestStatementSeek()
{
}

DR_MET(public virtual)
void TestStatementSeek::test(const String &arg DR_INIT(= Null()))
{
	db_sample0->removeAll();

	for (int i = 0; i < 8; i++)
		db_sample0->insert(i, String::createNumber(i));

	ERef<dr::sql::ResultSet> rs(db_sample0->openOrdered());
	Sint64 val;
	String str;
	rs->bindResult(0, &val);
	rs->bindResult(1, &str);
	rs->store();
	TENV_CHECK(rs->next()); TENV_CHECK(val == 0); TENV_CHECK(str == "0");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 1); TENV_CHECK(str == "1");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 2); TENV_CHECK(str == "2");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 3); TENV_CHECK(str == "3");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 4); TENV_CHECK(str == "4");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 5); TENV_CHECK(str == "5");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 6); TENV_CHECK(str == "6");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 7); TENV_CHECK(str == "7");
	TENV_CHECK(!rs->next());
	rs->seek(0);
	TENV_CHECK(rs->next()); TENV_CHECK(val == 0); TENV_CHECK(str == "0");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 1); TENV_CHECK(str == "1");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 2); TENV_CHECK(str == "2");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 3); TENV_CHECK(str == "3");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 4); TENV_CHECK(str == "4");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 5); TENV_CHECK(str == "5");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 6); TENV_CHECK(str == "6");
	TENV_CHECK(rs->next()); TENV_CHECK(val == 7); TENV_CHECK(str == "7");
	TENV_CHECK(!rs->next());
}


DR_SQL_TENV_NS_END
