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

#ifndef dr__ScalarPtr__hxx__
# define dr__ScalarPtr__hxx__

#include <dr/types.hxx>
#include <dr/String.hxx>
#include <dr/Blob.hxx>
#include <dr/Object.hxx>
#include <dr/Atomic.hxx>

DR_NS_BEGIN


class ScalarPtr
{
public:
	enum Type {
		T_None			= 0,
		T_Bool,
		T_Sint8,
		T_Sint16,
		T_Sint32,
		T_Sint64,
		T_Uint8,
		T_Uint16,
		T_Uint32,
		T_Uint64,
		T_Float,
		T_Double,
		T_LongDouble,
		T_String,
		T_Blob,
		T_Object,
	};

public:
	DR_CONSTRUCT			ScalarPtr(bool *ptr):			val_type(T_Bool), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Sint8 *ptr):			val_type(T_Sint8), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Sint16 *ptr):			val_type(T_Sint16), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Sint32 *ptr):			val_type(T_Sint32), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Sint64 *ptr):			val_type(T_Sint64), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Uint8 *ptr):			val_type(T_Uint8), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Uint16 *ptr):			val_type(T_Uint16), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Uint32 *ptr):			val_type(T_Uint32), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Uint64 *ptr):			val_type(T_Uint64), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(float *ptr):			val_type(T_Float), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(double *ptr):			val_type(T_Double), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(long double *ptr):		val_type(T_LongDouble), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(String *ptr):			val_type(T_String), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Blob *ptr):			val_type(T_Blob), val_ptr(ptr) {}
	DR_CONSTRUCT			ScalarPtr(Object **ptr):		val_type(T_Object), val_ptr(ptr) {}

public:
	DR_RINLINE Type			getType() const				{ return val_type; }

	/**
	 * Returns the common type that the value handles
	 * The common types are:
	 * T_None, T_Bool, T_Sint64, T_Uint64, T_Double, T_String, T_Blob, T_Object
	 */
	DR_RINLINE Type			getCommonType() const			{ switch (val_type) { case T_None: return T_None; case T_Bool: return T_Bool; case T_Sint8: case T_Sint16: case T_Sint32: case T_Sint64: return T_Sint64; case T_Uint8: case T_Uint16: case T_Uint32: case T_Uint64: return T_Uint64; case T_Float: case T_Double: case T_LongDouble: return T_Double; case T_String: return T_String; case T_Blob: return T_Blob; case T_Object: return T_Object; } }

	DR_RINLINE void *		getPtr() const				{ return val_ptr; }

protected:
	Type				val_type;
	void *				val_ptr;
};


DR_NS_END

#endif
