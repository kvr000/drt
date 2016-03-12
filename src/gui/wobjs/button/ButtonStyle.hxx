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

#ifndef dr__gui__ButtonStyle__hxx__
# define dr__gui__ButtonStyle__hxx__

#include <dr/gui/Style_Simple.hxx>
#include <dr/gui/Button.hxx>

DR_GUI_NS_BEGIN


class DR_GUI_PUB ButtonStyle: public Style_Simple
{
protected:
	enum State
	{
		S_None,
		S_Cancel,
		S_Pressed,
		S_KeyDown,
		S_MouseDown,
		S_MouseOut,
		S_MouseIn,
		S_KeyUp,
		S_MouseUp,
	}				state;

public:
	static ButtonStyle *		create(StyleFac *stylefac, Button *widget);

protected:
	DR_RINLINE 			ButtonStyle(StyleFac *stylefac, Button *widget);

public:
	DR_GUI_MET virtual int		handleEvent(int gui_orig, Event *evt);

public:
	DR_GUI_MET virtual void		paintBase(Paint *paint);

protected:
	DR_RINLINE Button *		getWidget();

protected:
	DR_GUI_MET bool			p_changeState(State newstate);
};


DR_GUI_NS_END

#endif
