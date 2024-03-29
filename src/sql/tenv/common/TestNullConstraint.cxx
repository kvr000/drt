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
#include <dr/sql/SqlBadNullConstraintException.hxx>

#include <dr/sql/tenv/TestNullConstraint.hxx>
#include "_gen/TestNullConstraint-all.hxx"

DR_SQL_TENV_NS_BEGIN


/*drt
 * class:	TestNullConstraint
 * ancestor:	dr::tenv::TenvObject
 *
 * at:	Ref<ConnectionHold>		db_conn;
 * at:	Ref<DbSample0>			db_sample0;
 */

DR_MET(public)
TestNullConstraint::TestNullConstraint(ConnectionHold *db_conn_):
	db_conn(db_conn_, true),
	db_sample0(DbSample0::accCached(db_conn_), true)
{
}

DR_MET(protected virtual)
TestNullConstraint::~TestNullConstraint()
{
}

DR_MET(public virtual)
void TestNullConstraint::test(const String &arg DR_INIT(= Null()))
{
	db_sample0->removeAll();

	for (int i = 0; i < 2; i++)
		db_sample0->insert(i, String::createNumber(i));
	TENV_CHECK_EXCEPTION(tref(db_conn->prepareStatement("INSERT INTO sample0 (i, s) VALUES (NULL, 'a')"))->executeUpdate(), dr::sql::SqlBadNullConstraintException);
}


DR_SQL_TENV_NS_END
