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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Hash.hxx>
#include <dr/UnsupportedExcept.hxx>
#include <dr/Subsystem.hxx>
#include <dr/String.hxx>

#include <dr/sql/dev/SqlManager.hxx>
#include <dr/sql/SqlConnection.hxx>

DR_SQL_NS_BEGIN

DR_OBJECT_DEF(DR_SQL_NS_STR, SqlConnection, Object);
DR_OBJECT_IMPL_SIMPLE(SqlConnection);


SqlConnection *SqlConnection::openConnection(const String &conn_str, SqlManager **manager)
{
	Ref<SqlConnection> conn;
	THash<String, String> pars;
	String driver;

	ssize_t pos = 0;
	for (ssize_t newpos = 0; pos < (ssize_t)conn_str.getLength(); pos = newpos+1) {
		ssize_t eq;
		if ((newpos = conn_str.find(";", pos)) < 0)
			newpos = conn_str.getLength();
		if ((eq = conn_str.find("=", pos)) < 0)
			xthrownew(UnsupportedExcept(NULL, "", conn_str.mid(pos, newpos-1), ""));
		pars[conn_str.mid(pos, eq-pos)] = conn_str.mid(eq+1, newpos-eq-1);
	}
	if ((driver = pars["driver"]).isEmpty())
		xthrownew(UnsupportedExcept(NULL, "", "driver", "not specified"));

	Subsystem *sys = Subsystem::getSubsystem(driver, NULL);
	xtry {
		if (!*manager) {
			*manager = (SqlManager *)sys->create(String(Null()), SqlManager::comp_name);
		}
		conn.setNoref((*manager)->openConnection(&pars));
		sys->releaseSubsystem();
	}
	xcatchany {
		sys->releaseSubsystem();
		xrethrowany;
	}
	xend;
	return conn.getAndNull();
}

SqlConnection *SqlConnection::openConnection(const String &connect_str)
{
	Ref<SqlManager> manager;
	return openConnection(connect_str, manager.mem());
}


DR_SQL_NS_END
