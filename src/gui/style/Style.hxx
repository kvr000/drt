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

#if 0

#ifndef dr__gui__Style__hxx__
# define dr__gui__Style__hxx__

#include <dr/String.hxx>

#include <dr/gui/Event.hxx>

DR_GUI_NS_BEGIN


class Widget_impl;
class StyleFac;
class Widget;
class Paint;

class DR_GUI_PUB Style: public Base
{
protected:
	enum GUI_ORIG
	{
		GO_NEXT = 0,
	};

protected:
	/* constructor */		Style(StyleFac *stylefac, Widget *widget);

protected:
	virtual				~Style();

public:
	virtual bool			isDummy();

public:	/* gui user interface */
	virtual bool			init() = 0;
	virtual void			setFocus();

	virtual Paint *			paintStart() = 0;
	virtual void			paintBase(Paint *paint);
	virtual void			paintEnd(Paint *paint);

	virtual bool			onShow(bool show);
	virtual bool			onLooseFocus(Style *target);
	virtual bool			onSetFocus(Style *target);

	virtual void 			changeName(const String &name);

	virtual Widget_impl *		getMain() = 0;

public: /* widget implementation interface */
	virtual int	 		handleEvent(int style_orig, Event *evt);

	DR_RINLINE int			handleEventSelf(int style_orig, Event *evt);
	DR_RINLINE int			handleEventCheck(Event *evt);

	virtual Widget_impl *		gui_getParent(int style_orig) = 0;
	virtual void			gui_getGeometry(int style_orig, Rect *geom) = 0;
	virtual Widget_impl *		gui_getGeometryAndParent(int style_orig, Rect *geom) = 0;

	virtual int			gui_getFocused() = 0;

public: /* read-only */
	StyleFac *			stylefac;
	Widget *			widget;

	friend class Widget;
};


DR_GUI_NS_END

#endif

#endif
