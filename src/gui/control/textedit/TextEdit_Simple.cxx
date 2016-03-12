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

#include <dr/gui/TextEdit_Simple.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, TextEdit_Simple, TextEdit);
DR_OBJECT_IMPL_SIMPLE(TextEdit_Simple);

DR_EXPORT_MET TextEdit_Simple::TextEdit_Simple(Gui *gui_):
	TextEdit(gui),
	pos_row(0),
	pos_col(0)
{
}

DR_EXPORT_MET TextEdit_Simple::TextEdit_Simple(Widget *parent_):
	TextEdit(parent_),
	pos_row(0),
	pos_col(0)
{
}

DR_EXPORT_MET void TextEdit_Simple::setText(const String &text_)
{
	text = text_;
	Rect r(geometry);
	r.move0();
	main_impl->redraw(r);
}

DR_EXPORT_MET String TextEdit_Simple::getText()
{
	return text;
}

DR_EXPORT_MET int TextEdit_Simple::handleEvent(Event *evt)
{
	int r;

	switch (evt->type) {
	case Event::ET_KeyPress:
		{
			Event_Key *e = (Event_Key *)evt;
			text.insert(pos_col, e->str);
			pos_col++;
			redraw();
		}
		r = 0;
		goto out;

	case Event::ET_KeyRelease:
		r = 0;
		goto out;

	default:
		r = Super::handleEvent(evt);
		break;
	}

out:
	return r;
}

DR_EXPORT_MET void TextEdit_Simple::paintBase(Paint *painter)
{
	Rect rect(geometry);
	rect.move0();
	//Point ed[5] = { rect.getRT(), rect.getRB(), rect.getLB(), rect.getLT(), rect.getRT() };

	painter->setForeground(style_fac->clr_background);
	painter->fillClip();
	painter->setForeground(style_fac->clr_font);
	painter->drawString(Point(0, 0), text);
}


DR_GUI_NS_END
