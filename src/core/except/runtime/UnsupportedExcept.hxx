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

#ifndef dr__UnsupportedExcept__hxx__
# define dr__UnsupportedExcept__hxx__

#include <dr/RuntimeExcept.hxx>

DR_NS_BEGIN


/**
 * Unsupported exception
 *
 * Caused by passing unsupported object to some functionality
 */
class DR_PUB UnsupportedExcept: public RuntimeExcept
{
	DR_THROW_DECL_SIMPLE(UnsupportedExcept, RuntimeExcept);

public:
	/** construct UnsupportedExcept:
	 *
	 * @param whom
	 * 	specifies whom the originator does not support
	 * @param originator
	 * 	specifies who doesn't support operation
	 * @param option
	 * 	unsupported option
	 * @param value
	 * 	unsupported value
	 */
	DR_CONSTRUCT			UnsupportedExcept(const Object *whom, const String &originator, const String &option = Null(), const String &value = Null());

public:
	virtual String			stringify() const;

protected:
	Ref<Object>			whom;
	String				originator;
	String				option;
	String				value;
};


DR_NS_END

#endif
