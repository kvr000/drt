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

#ifndef dr__sql__ResultSet__hxx__
# define dr__sql__ResultSet__hxx__

#include <dr/sql/def.hxx>

#include <dr/Object.hxx>
#include <dr/Blob.hxx>
#include <dr/Variant.hxx>

#include <dr/sql/Date.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE


class ResultSet: public Object
{
	DR_OBJECT_DECL_SIMPLE(ResultSet, Object);

public:
	virtual void			bindResultReset() = 0;
	virtual void			bindResult(unsigned column, Sint8 *value) = 0;
	virtual void			bindResult(const String &column, Sint8 *value) = 0;
	virtual void			bindResult(unsigned column, Sint16 *value) = 0;
	virtual void			bindResult(const String &column, Sint16 *value) = 0;
	virtual void			bindResult(unsigned column, Sint32 *value) = 0;
	virtual void			bindResult(const String &column, Sint32 *value) = 0;
	virtual void			bindResult(unsigned column, Sint64 *value) = 0;
	virtual void			bindResult(const String &column, Sint64 *value) = 0;
	virtual void			bindResult(unsigned column, double *value) = 0;
	virtual void			bindResult(const String &column, double *value) = 0;
	virtual void			bindResult(unsigned column, String *value) = 0;
	virtual void			bindResult(const String &column, String *value) = 0;
	virtual void			bindResult(unsigned column, Blob *value) = 0;
	virtual void			bindResult(const String &column, Blob *value) = 0;
	virtual void			bindResult(unsigned column, Date *value) = 0;
	virtual void			bindResult(const String &column, Date *value) = 0;
	virtual void			bindResult(unsigned column, Variant **value) = 0;
	virtual void			bindResult(const String &column, Variant **value) = 0;
	virtual void			bindNullHandler(unsigned column, void (*handler)(void *value)) = 0;
	virtual void			bindNullHandler(const String &column, void (*handler)(void *value)) = 0;

public:
	virtual bool			fetchRow() = 0;
	virtual void			store() = 0;

public:
	virtual bool			isNull(unsigned column) = 0;
	virtual bool			isNull(const String &column) = 0;
	virtual Sint64			getInt(unsigned column) = 0;
	virtual Sint64			getInt(const String &column) = 0;
	virtual double			getDouble(unsigned column) = 0;
	virtual double			getDouble(const String &column) = 0;
	virtual Blob			getBlob(unsigned column) = 0;
	virtual Blob			getBlob(const String &column) = 0;
	virtual String			getString(unsigned column) = 0;
	virtual String			getString(const String &column) = 0;
	virtual Date			getDate(unsigned column) = 0;
	virtual Date			getDate(const String &column) = 0;
	virtual Variant *		getVariant(unsigned column) = 0;
	virtual Variant *		getVariant(const String &column) = 0;
};


DR_SQL_NS_END

#endif
