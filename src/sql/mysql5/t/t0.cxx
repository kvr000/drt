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

#include <dr/sql/Connection.hxx>
#include <dr/sql/Statement.hxx>
#include <dr/sql/ResultSet.hxx>

DR_SQL_NS_USE


String i_str(Const::string("i"));
String s_str(Const::string("s"));

int main(void)
{
	ERef<Connection> conn(Connection::openConnection("driver=drSql_mysql5;host=localhost;port=3306;db=dr_test;user=dr_test;pass=dr_test"));
	ERef<Statement> sql(conn->createStatement("select i, s from sample0 where i between ? and ? order by i"));
	for (int i = 0; ; i++) {
		//sql->setOffsetLimit(i*10, 10);
		sql->prepare();
		sql->bindParam(0, 1);
		sql->bindParam(1, 40);
		ERef<ResultSet> rs(sql->executeQuery());
		Sint32 vi = -999;
		String vs;
		rs->bindResult(i_str, &vi);
		rs->bindResult(s_str, &vs);
		if (rs->fetchNext()) {
			do {
				printf("b=%4d i=%4d s=%s z=%s\n", vi, (int)rs->getInt(i_str), rs->getString(s_str).utf8().toStr(), vs.utf8().toStr());
			} while (rs->fetchNext());
			break;
		}
		else {
			break;
		}
	}

	xtry {
		tref(conn->prepareStatement("insert into sample0 (i, s) values(4, 'he')"))->executeUpdate();
	}
	xcatch (Exception, ex) {
		printf("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;

	return 0;
}
