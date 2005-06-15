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
 * include:	dr/sql/SqlLayer.hxx
 * include:	dr/sql/ResultSet.hxx
 *
 * ns:		dr::sql::tenv
 */

#include <dr/x_kw.hxx>
#include <dr/Exception.hxx>

#include <dr/sql/tenv/DbSample0.hxx>
#include "_gen/DbSample0-all.hxx"

DR_SQL_TENV_NS_BEGIN


/*drt
 * class:	DbSample0
 * type:	sqllayer
 *
 * sql:	removeAll
 * 		DELETE FROM sample0
 *
 * sql:	remove
 * 		DELETE FROM sample0 WHERE i = ?
 *
 * sql:	insert
 * 		INSERT INTO sample0 (i, s) VALUES ( ?, ? )
 *
 * sql:	find
 * 		SELECT s FROM sample0 WHERE i = ?
 *
 * sql:	listOrdered
 * 		SELECT i, s FROM sample0 ORDER BY i
 *
 */

DR_MET(protected virtual)
DbSample0::~DbSample0()
{
}

DR_MET(public virtual)
Sint64 DbSample0::removeAll()
{
	if (!st_removeAll)
		prep_removeAll();
	st_removeAll->executeUpdate();
	return st_removeAll->getAffectedRows();
}

DR_MET(public virtual)
void DbSample0::insert(int i, const String &s)
{
	if (!st_insert)
		prep_insert();
	st_insert->bindParam(0, i);
	st_insert->bindParam(1, s);
	st_insert->executeUpdate();
}

DR_MET(public virtual)
int DbSample0::remove(int i)
{
	if (!st_remove)
		prep_remove();
	st_remove->bindParam(0, i);
	st_remove->executeUpdate();
	return st_remove->getAffectedRows();
}

DR_MET(public virtual)
String DbSample0::find(int i)
{
	if (!st_find)
		prep_find();
	st_find->bindParam(0, i);
	ERef<ResultSet> rs(st_find->executeQuery());
	return rs->next() ? rs->getString(0) : Null();

}

DR_MET(public virtual)
ResultSet *DbSample0::openOrdered()
{
	if (!st_listOrdered || st_listOrdered->isBusy())
		prep_listOrdered();
	return st_listOrdered->executeQuery();
}


DR_SQL_TENV_NS_END
