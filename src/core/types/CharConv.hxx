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

#ifndef dr__CharConv__hxx__
# define dr__CharConv__hxx__

#include <dr/def.hxx>
#include <dr/enumops.hxx>

DR_NS_BEGIN


class DR_PUB CharConv
{
public:
	enum Encoding
	{
		E_Unknown		= 0,

		E_Iso01			= 1,
		E_Iso02			= 2,
		E_Iso03			= 3,
		E_Iso04			= 4,
		E_Iso05			= 5,
		E_Iso06			= 6,
		E_Iso07			= 7,
		E_Iso08			= 8,
		E_Iso09			= 9,

		E_Utf8			= 32,
		E_Binary		= 33,

		EF_Ascii7		= 64,
	};
};

DR_ENUM_OPS(CharConv::Encoding);


DR_NS_END

#endif
