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

#ifndef dr__CastExcept__hxx__
# define dr__CastExcept__hxx__

#include <dr/RuntimeException.hxx>

DR_NS_BEGIN


/**
 * Cast Exception
 *
 * Caused by requesting unsupported interface.
 */
class DR_PUB CastException: public RuntimeException
{
	DR_THROW_DECL_SIMPLE(CastException, RuntimeException);

public:
	/** construct CastException:
	 *
	 * @param obj
	 * 	object casted, the reference is lost
	 * @param iface
	 * 	requested interface
	 */
	DR_CONSTRUCT			CastException(Object *obj, const String &iface);

	/**
	 * throws and instance of CastException
	 *
	 * @param obj
	 * 	object casted, the reference is lost
	 * @param iface
	 * 	requested interface
	 */
	static void			throwInstance(Object *obj, const String &iface);

public:
	virtual String			stringify() const;

protected:
	Ref<Object>			obj;
	String				iface;
};


DR_NS_END

#endif
