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

#include <stdio.h>

#include <dr/Const.hxx>
#include <dr/gui/Paint.hxx>
#include <dr/gui/StyleFactory.hxx>
#include <dr/gui/dev/Widget_impl.hxx>

#include <dr/gui/Button_Simple.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, Button_Simple, Button);
DR_OBJECT_IMPL_SIMPLE(Button_Simple);

DR_EXPORT_MET Button_Simple::Button_Simple(Gui *gui_):
	Button(gui),
	state(S_None)
{
}

DR_EXPORT_MET Button_Simple::Button_Simple(Widget *parent_):
	Button(parent_),
	state(S_None)
{
}

DR_EXPORT_MET bool Button_Simple::p_changeState(State newstate)
{
	if (state == newstate)
		return false;

	switch (newstate) {
	case S_Cancel:
		break;

	case S_KeyUp:
		if (state != S_KeyDown)
			return false;
		newstate = S_Pressed;
		break;

	case S_MouseUp:
		if (state != S_MouseDown) {
			if (state == S_MouseOut)
				state = S_None;
			return false;
		}
		newstate = S_Pressed;
		break;

	case S_MouseIn:
		if (state != S_MouseOut)
			return false;
		newstate = S_MouseDown;
		break;

	case S_MouseOut:
		if (state != S_MouseDown)
			return false;
		break;

	default:
		break;
	}
	Rect rect(geometry);
	rect.move0();
	main_impl->redraw(rect);

	if (newstate == S_Pressed) {
		newstate = S_None;
		if (const Eslot0<void> *n = findNotifierPressed())
			(*n)();
	}
	state = newstate;

	return true;
}

DR_EXPORT_MET int Button_Simple::handleEvent(Event *evt)
{
	int r = -1;

	switch (evt->type) {
	case Event::ET_ButtonDown:
		{
			Event_MouseClick *e = (Event_MouseClick *)evt;
			if (e->button == 0)
				p_changeState(S_MouseDown);
		}
		r = true;
		goto out;

	case Event::ET_ButtonUp:
		{
			Event_MouseClick *e = (Event_MouseClick *)evt;
			if (e->button == 0)
				p_changeState(S_MouseUp);
		}
		r = true;
		goto out;

	case Event::ET_KeyPress:
		{
			Event_Key *e = (Event_Key *)evt;
			if (e->sym == Key::S_Space)
				p_changeState(S_KeyDown);
			else if (e->sym == Key::S_Escape)
				p_changeState(S_Cancel);
			else
				break;
		}
		r = true;
		goto out;

	case Event::ET_KeyRelease:
		{
			Event_Key *e = (Event_Key *)evt;
			if (e->sym == Key::S_Space)
				p_changeState(S_KeyUp);
		}
		r = true;
		goto out;

	case Event::ET_Enter:
		{
			p_changeState(S_MouseIn);
		}
		r = true;
		goto out;

	case Event::ET_Leave:
		{
			p_changeState(S_None);
		}
		r = true;
		goto out;

	default:
		r = Super::handleEvent(evt);
		break;
	}

out:
	return r;
}

DR_EXPORT_MET void Button_Simple::paintBase(Paint *painter)
{
	Rect rect(geometry);
	rect.move0();
	Point ed[5] = { rect.getRT(), rect.getRB(), rect.getLB(), rect.getLT(), rect.getRT() };


	painter->setForeground(style_fac->clr_background);
	painter->fillClip();
	if (state == S_KeyDown || state == S_MouseDown) {
		painter->setForeground(style_fac->clr_light_edge);
		painter->drawLines(ed, 3);
		painter->setForeground(style_fac->clr_dark_edge);
		painter->drawLines(ed+2, 3);
	}
	else {
		painter->setForeground(style_fac->clr_dark_edge);
		painter->drawLines(ed, 3);
		painter->setForeground(style_fac->clr_light_edge);
		painter->drawLines(ed+2, 3);
	}
	if (isEnabled()) {
		painter->setForeground(style_fac->clr_font);
	}
	else {
		painter->setForeground(style_fac->clr_disabled);
	}
	painter->drawString(Point(2, 2), name);
	if (isFocused())
		style_fac->drawFocusMark(painter, this);
}


DR_GUI_NS_END
