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

#include <dr/Array.hxx>

#include <dr/sql/ConnectionPool.hxx>

#include <dr/sql/ConnectionHold.hxx>

DR_SQL_NS_BEGIN

DR_OBJECT_DEF(DR_SQL_NS_STR, ConnectionHold, Connection);
DR_OBJECT_IMPL_SIMPLE(ConnectionHold);


ConnectionHold::ConnectionHold(Connection *connection_):
	connection(connection_, true)
{
}

ConnectionHold::~ConnectionHold()
{
}

bool ConnectionHold::ping()
{
	return connection->ping();
}

void ConnectionHold::reconnect()
{
	connection->reconnect();
}

void ConnectionHold::commit()
{
	return connection->commit();
}

void ConnectionHold::rollback()
{
	return connection->rollback();
}

Statement *ConnectionHold::createStatement(const String &sql)
{
	return connection->createStatement(sql);
}

Statement *ConnectionHold::prepareStatement(const String &sql)
{
	return connection->prepareStatement(sql);
}

bool ConnectionHold::prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int lock_type0, const String *lock_table0, ...)
{
	va_list vl;
	va_start(vl, lock_table0);
	SArray<int> types;
	SArray<String> tables;
	types.append(lock_type0);
	tables.append(*lock_table0);
	for (;;) {
		int l = va_arg(vl, int);
		if (l < 0)
			break;
		types.append(l);
		tables.append(*va_arg(vl, const String *));
	}
	return connection->prepareLockStatements(lock_mem, unlock_mem, &types[0], &tables[0], types.count());
}

bool ConnectionHold::prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int *lock_type, const String *lock_table, size_t count)
{
	return connection->prepareLockStatements(lock_mem, unlock_mem, lock_type, lock_table, count);
}

Object *ConnectionHold::accDbLayer(const String &name)
{
	return db_layers[name].getNoref();
}

Object *ConnectionHold::getDbLayer(const String &name)
{
	return db_layers[name].getDoref();
}

void ConnectionHold::addDbLayer(const String &name, Object *db_layer)
{
	db_layers[name].setDoref(db_layer);
}

void ConnectionHold::addDbLayer(Object *db_layer)
{
	addDbLayer(db_layer->classname(), db_layer);
}


DR_SQL_NS_END
