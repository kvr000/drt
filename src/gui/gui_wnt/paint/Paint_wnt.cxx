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

#include <dr/gui/wnt/Paint_wnt.hxx>
#include <dr/gui/wnt/Widget_wnt.hxx>
#include <dr/gui/wnt/Visual_wnt.hxx>

DR_GUI_NS_BEGIN


Paint_wnt::~Paint_wnt()
{
	if (gc)
		EndPaint(widget->impl_window, &ps);
}

bool Paint_wnt::initContext()
{
	if (!(gc = BeginPaint(widget->impl_window, &ps)))
		return false;
	clip.setCoord((GuiCoord)ps.rcPaint.left, (GuiCoord)ps.rcPaint.top, (GuiCoord)ps.rcPaint.right, (GuiCoord)ps.rcPaint.bottom);
	return true;
}

void Paint_wnt::setForeground(GuiColor color)
{
	foreground_color = colorToWnt(color);
	DeleteObject(SelectObject(gc, CreatePen(line_style ? PS_DASH : PS_SOLID, 1, foreground_color)));
}

void Paint_wnt::setBackground(GuiColor color)
{
	background_color = colorToWnt(color);
	DeleteObject(SelectObject(gc, CreatePen(line_style ? PS_DASH : PS_SOLID, 1, background_color)));
}

void Paint_wnt::setLineStyle(int style)
{
	line_style = style;
	DeleteObject(SelectObject(gc, CreatePen(line_style ? PS_DASH : PS_SOLID, 1, foreground_color)));
}

void Paint_wnt::setFont(Font *font_)
{
	font.setNoref((Font_wnt *)font_->getImpl());
}

void Paint_wnt::setFont(Font_impl *font_)
{
	if (*font != font_)
		font.setDoref((Font_wnt *)font_);
}

Font_impl *Paint_wnt::getFont()
{
	return font.getDoref();
}

int Paint_wnt::getFontHeight()
{
	TEXTMETRIC tm;
	GetTextMetrics(gc, &tm);
	return tm.tmHeight;
}

int Paint_wnt::getFontAscent()
{
	TEXTMETRIC tm;
	GetTextMetrics(gc, &tm);
	return tm.tmAscent;
}

int Paint_wnt::getFontDescent()
{
	TEXTMETRIC tm;
	GetTextMetrics(gc, &tm);
	return tm.tmDescent;
}

void Paint_wnt::getStringSize(Size *size, const String &str)
{
	SIZE wsize;
	WString str_nat(str.wide());
	GetTextExtentPointW(gc, str_nat.toStr(), str_nat.getSize(), &wsize);
	size->set(wsize.cx, wsize.cy);
}

void Paint_wnt::drawRectangle(const Rect &rect)
{
	POINT p[5] = { { rect.getL(), rect.getT() }, { rect.getR(), rect.getT() }, { rect.getR(), rect.getB() }, { rect.getL(), rect.getB() }, { rect.getL(), rect.getT() } };
	Polyline(gc, p, 5);
}

void Paint_wnt::fillRectangle(const Rect &rect)
{
	HBRUSH br = CreateSolidBrush(foreground_color);
	if (sizeof(RECT) == sizeof(Rect)) {
		FillRect(gc, (RECT *)&rect, br);
	}
	else {
		RECT r = { rect.getL(), rect.getT(), rect.getR(), rect.getB() };
		FillRect(gc, &r, br);
	}
	DeleteObject(br);
}

void Paint_wnt::fillClip()
{
	HBRUSH br = CreateSolidBrush(foreground_color);
	RECT r = { clip.getL(), clip.getT(), clip.getR(), clip.getB() };
	FillRect(gc, &r, br);
	DeleteObject(br);
}

void Paint_wnt::drawLine(const Point &p0, const Point &p1)
{
	POINT points[2] = { { p0.x, p0.y }, { p1.x, p1.y } };
	Polyline(gc, points, 2);
}

void Paint_wnt::drawLines(const Point *plist, int count)
{
	if (sizeof(POINT) == sizeof(Point)) {
		Polyline(gc, (POINT *)plist, count);
	}
	else {
		if (POINT *xp = count <= 16 ? (POINT *)alloca(count*sizeof(POINT)) : (POINT *)MM::allocS(count*sizeof(POINT))) {
			for (ssize_t i = count; --i >= 0; ) {
				xp[i].x = plist[i].x;
				xp[i].y = plist[i].y;
			}
			Polyline(gc, xp, count);
			if (count > 16)
				MM::freeS(xp, count*sizeof(POINT));
		}
	}
}

void Paint_wnt::drawString(const Point &pos, const String &str)
{
	WString str_nat(str.wide());
	RECT r = { clip.getL(), clip.getT(), clip.getR(), clip.getB() };

	SetBkMode(gc, TRANSPARENT);
	SetTextColor(gc, foreground_color);
	//TextOutW(gc, pos.x, pos.y, str_nat.toStr(), str_nat.getSize());
	ExtTextOutW(gc, pos.x, pos.y, 0, &r, str_nat.toStr(), str_nat.getSize(), NULL);
}


DR_GUI_NS_END
