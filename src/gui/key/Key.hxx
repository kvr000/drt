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

#ifndef dr__gui__Key__hxx__
# define dr__gui__Key__hxx__

DR_GUI_NS_BEGIN


class DR_GUI_PUB Key: private Base
{
public:
	enum Sym
	{
		S_None		= 0xff00,
		S_Backspace	= 0xff08,
		S_Tab		= 0xff09,
		S_Escape	= 0xff1b,

		S_Up		= 0xff20,
		S_Down		= 0xff21,
		S_Left		= 0xff22,
		S_Right		= 0xff23,

		S_Space		= 0x0020,
	};

	enum Modifier
	{
		M_ShiftR	= 0x0001,
		M_ShiftL	= 0x0002,
		M_Shift		= M_ShiftR|M_ShiftL,
		M_ControlL	= 0x0004,
		M_ControlR	= 0x0008,
		M_Control	= M_ControlL|M_ControlR,
		M_MetaL		= 0x0010,
		M_MetaR		= 0x0020,
		M_Meta		= M_MetaL|M_MetaR,
	};
};


DR_GUI_NS_END

#endif
