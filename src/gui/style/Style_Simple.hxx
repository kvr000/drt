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

#ifndef dr__gui__Style_Simple__hxx__
# define dr__gui__Style_Simple__hxx__

#include <dr/gui/Style.hxx>

DR_GUI_NS_BEGIN


class DR_GUI_PUB Style_Simple: public Style
{
	enum
	{
		GO_MAIN = Style::GO_NEXT,
		GO_NEXT,
	};

public:
	static Style_Simple *		create(StyleFac *stylefac, Widget *widget);

protected:
	DR_RINLINE			Style_Simple(StyleFac *stylefac, Widget *widget);
	DR_RINLINE virtual		~Style_Simple();

public:
	virtual bool			init();
	virtual bool			onShow(bool show);
	virtual bool			onSetFocus(Style *style);

	virtual Paint *			paintStart();
	virtual void			paintBase(Paint *paint);

	virtual void 			changeName(const String &name);

	virtual Widget_impl *		getMain();

public:
	virtual int			handleEvent(int style_orig, Event *evt);
	int				handleEventSelf(int style_orig, Event *evt);

public:
	virtual Widget_impl *		gui_getParent(int style_orig);
	virtual void			gui_getGeometry(int style_orig, Rect *geom);
	virtual Widget_impl *		gui_getGeometryAndParent(int style_orig, Rect *geom);

	virtual int			gui_getFocused();

protected:
	Widget_impl *			simple_impl;
};


DR_GUI_NS_END

#endif
