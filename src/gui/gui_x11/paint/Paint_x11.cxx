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

#include <dr/gui/x11/Paint_x11.hxx>
#include <dr/gui/x11/Widget_x11.hxx>
#include <dr/gui/x11/Visual_x11.hxx>

DR_GUI_NS_BEGIN


Paint_x11::~Paint_x11()
{
	XFreeGC(accDisplay(), gc);
}

void Paint_x11::setForeground(GuiColor color)
{
	XSetForeground(accDisplay(), gc, visual->mapColor(color));
}

void Paint_x11::setBackground(GuiColor color)
{
	XSetBackground(accDisplay(), gc, visual->mapColor(color));
}

void Paint_x11::setLineStyle(int style)
{
	XSetLineAttributes(accDisplay(), gc, 1, style ? LineDoubleDash : LineSolid, CapRound, JoinRound);
}

int Paint_x11::getFontAscent()
{
	return font->font_str->ascent;
}

int Paint_x11::getFontDescent()
{
	return font->font_str->descent;
}

int Paint_x11::getFontHeight()
{
	return font->font_str->ascent+font->font_str->descent;
}

void Paint_x11::getStringSize(Size *size, const String &str)
{
	int direction, ascent, descent;
	XCharStruct draw_info;
	BString str_nat(str.utf8());
	size_t l = str_nat.getSize();
	XTextExtents(font->font_str, str_nat.toStr(), l, &direction, &ascent, &descent, &draw_info);
	size->set(draw_info.width, draw_info.ascent-draw_info.descent);
}

void Paint_x11::setFont(Font *font_)
{
	font.setNoref((Font_x11 *)font_->getImpl());
}

void Paint_x11::setFont(Font_impl *font_)
{
	font.setDoref((Font_x11 *)font_);
}

Font_impl *Paint_x11::getFont()
{
	return font.getDoref();
}

void Paint_x11::drawRectangle(const Rect &rect)
{
	XDrawRectangle(accDisplay(), widget->impl_window, gc, rect.getL(), rect.getT(), rect.getW()-1, rect.getH()-1);
}

void Paint_x11::fillRectangle(const Rect &rect)
{
	XFillRectangle(accDisplay(), widget->impl_window, gc, rect.getL(), rect.getT(), rect.getW(), rect.getH());
}

void Paint_x11::fillClip()
{
	XFillRectangle(accDisplay(), widget->impl_window, gc, clip.getL(), clip.getT(), clip.getW(), clip.getH());
}

void Paint_x11::drawLine(const Point &p0, const Point &p1)
{
	XDrawLine(accDisplay(), widget->impl_window, gc, p0.x, p0.y, p1.x, p1.y);
}

void Paint_x11::drawLines(const Point *plist, int count)
{
	if (sizeof(XPoint) == sizeof(Point)) {
		XDrawLines(accDisplay(), widget->impl_window, gc, (XPoint *)plist, count, CoordModeOrigin);
	}
	else {
		XPoint xp[count];
		for (int i = count; --i >= 0; ) {
			xp[i].x = plist[i].x;
			xp[i].y = plist[i].y;
		}
		XDrawLines(accDisplay(), widget->impl_window, gc, xp, count, CoordModeOrigin);
	}
}

void Paint_x11::drawString(const Point &pos, const String &str_)
{
	BString str_nat(str_.ascii());
	size_t l = str_nat.getSize();

	XDrawString(accDisplay(), widget->impl_window, gc, pos.x, pos.y+font->font_str->max_bounds.ascent, str_nat.toStr(), l);
}


DR_GUI_NS_END
