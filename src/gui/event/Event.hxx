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

#ifndef dr__gui__Event__hxx__
# define dr__gui__Event__hxx__

#include <dr/String.hxx>

#include <dr/gui/struct.hxx>
#include <dr/gui/Key.hxx>

DR_GUI_NS_BEGIN


class Widget;

class DR_GUI_PUB Event: public Base
{
public:
	enum EventType
	{
		ET_Invalid,
		ET_KeyPress,
		ET_KeyRelease,
		ET_ButtonDown,
		ET_ButtonUp,
		ET_DoubleClick,
		ET_TrippleClick,
		ET_MouseMove,
		ET_Enter,
		ET_Leave,
		ET_Repaint,
		ET_FocusIn,
		ET_FocusOut,
		ET_MoveFocus,
		ET_Close,
	};

public:
	DR_RINLINE			Event(const None &)			{ cancel = false; }
	DR_RINLINE			Event(EventType type_)			: type(type_) { cancel = false; }
	DR_RINLINE			Event(EventType type_, Widget *target_)	: type(type_), target(target_) { cancel = false; }
	DR_RINLINE virtual		~Event()				{}

public:
	EventType			type;
	Widget *			target;
	bool				cancel;
};


class DR_GUI_PUB Event_Repaint: public Event
{
public:
	DR_RINLINE Event_Repaint()						: Event(ET_Repaint) {}
};


class DR_GUI_PUB Event_Close: public Event
{
public:
	DR_RINLINE Event_Close()						: Event(ET_Close) {}
};


class DR_GUI_PUB Event_Focus: public Event
{
public:
	DR_RINLINE			Event_Focus(const None &i)		: Event(i) {}
	DR_RINLINE			Event_Focus(EventType type_)		: Event(type_) {}

public:
	bool				redirected;
};


class DR_GUI_PUB Event_MoveFocus: public Event
{
public:
	DR_RINLINE			Event_MoveFocus()			: Event(ET_MoveFocus) {}
	DR_RINLINE			Event_MoveFocus(bool forward_)		: Event(ET_MoveFocus), forward(forward_) {}

public:
	bool				forward;
};


class DR_GUI_PUB Event_Key: public Event
{
public:
	DR_RINLINE 			Event_Key(EventType type_)		: Event(type_) {}
	DR_RINLINE			Event_Key(const None &n)		: Event(n) {}

public:
	Key::Sym			sym;
	String				str;
	bool				repeat;
};


class DR_GUI_PUB Event_MouseClick: public Event
{
public:
	DR_RINLINE			Event_MouseClick(EventType type_)	: Event(type_) {}
	DR_RINLINE			Event_MouseClick(const None &n)		: Event(n) {}

public:
	int				button;
};


class DR_GUI_PUB Event_MouseMove: public Event
{
public:
	DR_RINLINE			Event_MouseMove()			: Event(ET_MouseMove), pos(None()) {}

public:
	Point				pos;
};


DR_GUI_NS_END

#endif
