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

#ifndef dr__gui__Font__hxx__
# define dr__gui__Font__hxx__

#include <dr/Object.hxx>
#include <dr/String.hxx>

#include <dr/gui/def_gui.hxx>

#include <dr/gui/dev/Font_impl.hxx>

DR_GUI_NS_BEGIN


class Gui;

class DR_GUI_PUB Font: public Object
{
	DR_OBJECT_DECL_SIMPLE(Font, Object);
public:
	enum Type
	{
		FT_Bitmap,
		FT_TrueType,
	};

public:
	DR_GUI_MET			Font(Gui *gui_, Type type_, const String &name_)	: type(type_), name(name_), gui(NULL) { impl = NULL; }
	DR_GUI_MET			~Font();


public:
	DR_RINLINE Font_impl *		getImpl()				{ return impl ? (Font_impl *)impl->ref() : p_getImpl(); }

protected:
	DR_GUI_MET Font_impl *		p_getImpl();

protected:
	Type				type;
	String				name;

	Font_impl *			impl;

	Gui *				gui;

public:
	friend class Font_impl;
};


DR_GUI_NS_END

#endif
