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

#ifndef dr__sql__SqlConnectionHold_hxx__
# define dr__sql__SqlConnectionHold_hxx__

#include <dr/Ref.hxx>
#include <dr/Hash.hxx>
#include <dr/Time.hxx>

#include <dr/sql/Connection.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE


class ConnectionHold: public Connection
{
	DR_OBJECT_DECL_SIMPLE(ConnectionHold, Connection);

public:
	DR_CONSTRUCT			ConnectionHold(Connection *connection);

protected:
	virtual				~ConnectionHold();

public:
	DR_RINLINE Connection *	getConnection()				{ return connection.getDoref(); }
	DR_RINLINE Connection *	accConnection()				{ return connection.getNoref(); }
	DR_RINLINE void			setConnection(Connection *c)		{ connection.setDoref(c); }

public:
	virtual bool			ping();
	virtual void			reconnect();
	virtual void			commit();
	virtual void			rollback();

public:
	virtual Statement *		createStatement(const String &sql);
	virtual Statement *		prepareStatement(const String &sql);
	virtual bool			prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int lock_type0, const String *lock_table0, ...);
	virtual bool			prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int *lock_type, const String *lock_tables, size_t count);

public:
	/**
	 * looks for existing Db layer
	 *
	 * @return NULL
	 * 	if the Db layer is not found
	 * @return otherwise
	 * 	the Db layer (not referenced)
	 */
	virtual Object *		accDbLayer(const String &name);

	/**
	 * looks for existing Db layer
	 *
	 * @return NULL
	 * 	if the Db layer is not found
	 * @return otherwise
	 * 	the Db layer
	 */
	virtual Object *		getDbLayer(const String &name);

	/**
	 * registers Db layer
	 * @note db_layer must not address this connection holder, but the real
	 * connection
	 *
	 * @param db_layer
	 * 	the Db layer
	 */
	virtual void			addDbLayer(Object *db_layer);

	/**
	 * registers Db layer
	 * @note db_layer must not address this connection holder, but the real
	 * connection
	 *
	 * @param name
	 * 	the name under which the Db layer will be registered
	 * @param db_layer
	 * 	the Db layer
	 */
	virtual void			addDbLayer(const String &name, Object *db_layer);

protected:
	Ref<Connection>			connection;
	THash<String, Ref<Object> >	db_layers;
};


DR_SQL_NS_END

#endif
