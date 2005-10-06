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
#include <dr/Exception.hxx>

#include <mysql/mysql.h>

#include <dr/sql/SqlException.hxx>

#include <dr/sql/mysql5/Connection_mysql5.hxx>
#include <dr/sql/mysql5/Statement_mysql5.hxx>
#include <dr/sql/mysql5/Statement_mysql5_direct.hxx>

#include <dr/sql/mysql5/SqlManager_mysql5.hxx>

DR_SQL_MYSQL5_NS_BEGIN

DR_OBJECT_DEF(DR_SQL_MYSQL5_NS_STR, SqlManager_mysql5, SqlManager);
DR_OBJECT_IMPL_SIMPLE(SqlManager_mysql5);


Connection *SqlManager_mysql5::openConnection(THash<String, String> *args)
{
	BString host((*args)["host"].utf8());
	BString port((*args)["port"].utf8());
	BString db((*args)["db"].utf8());
	BString user((*args)["user"].utf8());
	BString pass((*args)["pass"].utf8());

	if (MYSQL *handle = mysql_init(NULL)) {
		bool locks = atoi((*args)["locks"].utf8());
#if 0
		my_bool reconnect = atoi((*args)["reconnect"].utf8());
#if MYSQL_VERSION_ID >= 0x50100
		if (reconnect) {
			mysql_options(handle, MYSQL_OPT_RECONNECT, &reconnect);
		}
#else
		handle->reconnect = reconnect;
#endif
#endif
		if (mysql_real_connect(handle, host.toStr(), user.toStr(), pass.toStr(), db.toStr(), atoi(port.toStr()), NULL, CLIENT_FOUND_ROWS)) {
			xtry {
				const char *charvars[] = { "character_set_server", "character_set_database", "character_set_connection", "character_set_client", "character_set_results" };
				String charset((*args)["charset"]);
				if (charset.isNullEmpty())
					charset = "UTF8";
				if (mysql_set_character_set(handle, charset.utf8()) != 0) {
					Connection_mysql5::throwSqlExcept(mysql_sqlstate(handle), mysql_error(handle));
				}
				ERef<Connection_mysql5> c(new dr::sql::mysql5::Connection_mysql5(handle));
				handle = NULL;
				tref(new Statement_mysql5_direct(c, String("SET CHARSET '")+charset+"'"))->executeUpdate();
				for (size_t i = 0; i < sizeof(charvars)/sizeof(charvars[0]); i++) {
					tref(new Statement_mysql5_direct(c, String("SET ")+charvars[i]+"='"+charset+"'"))->executeUpdate();
				}
#if 0
				c->auto_reconnect = reconnect;
#endif
				c->use_locks = locks;
				return c.getAndNull();
			}
			xcatchany {
				if (handle != NULL)
					mysql_close(handle);
				xrethrowany;
			}
			xend;
		}
		else {
			xtry {
				Connection_mysql5::throwSqlExcept(mysql_sqlstate(handle), mysql_error(handle));
			}
			xcatchany {
				mysql_close(handle);
				xrethrowany;
			}
			xend;
		}
	}
	xthrownew(SqlException(-1, "failed to create mysql object"));
	return NULL;
}

Object *Sql_SS_mysql5::create(const String &object)
{
	if (object == SqlManager::comp_name)
		return (SqlManager *)new SqlManager_mysql5();
	return NULL;
}

void *Sql_SS_mysql5::create(const String &object, const String &iface)
{
	if (!object.isNull())
		return Subsystem::create(object, iface);
	if (iface == SqlManager::comp_name)
		return (SqlManager *)new SqlManager_mysql5();
	return NULL;
}

Object *Sql_SS_mysql5::create(int object)
{
	return NULL;
}

void *Sql_SS_mysql5::create(int object, int iface)
{
	return NULL;
}

Sql_SS_mysql5::Sql_SS_mysql5()
{
}

Sql_SS_mysql5::~Sql_SS_mysql5()
{
}

extern "C" DR_EXPORT_MTS Subsystem *createSS_dr_sql_mysql5(const String &name)
{
	return new Sql_SS_mysql5;
}


DR_SQL_MYSQL5_NS_END
