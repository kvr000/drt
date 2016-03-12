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

#ifndef dr__gui__struct__hxx__
# define dr__gui__struct__hxx__

#include <dr/types.hxx>

#include <dr/gui/def_gui.hxx>

DR_GUI_NS_BEGIN


/** implementation dependent color */
typedef Uint32 GuiPixel;

/** RGB color */
typedef Uint32 GuiColor;

/** create GuiColor from rgb tripple */
#define drRGB(r, g, b) ((GuiColor)(((r)<<16)|((g)<<8)|((b)<<0)))

/** coordination int */
typedef short GuiCoord;

/** size int */
typedef unsigned short GuiSize;

typedef DR_GUI_PUB struct Point
{
public:
	GuiCoord			x, y;

public:
	DR_RINLINE			Point(GuiCoord x_, GuiCoord y_)	: x(x_), y(y_) {}
	DR_RINLINE			Point(const None &)		{}

public:
	DR_RINLINE void			set(GuiCoord x_, GuiCoord y_)		{ x = x_; y = y_; }
	DR_RINLINE void			set(const Point &s)			{ x = s.x; y = s.y; }

} Point;


typedef struct Size
{
public:
	GuiSize				w, h;

public:
	DR_RINLINE			Size(GuiSize w_, GuiSize h_):	w(w_), h(h_) {}
	DR_RINLINE			Size(const None &)			{}

public:
	DR_RINLINE void			set(GuiSize w_, GuiSize h_)		{ w = w_; h = h_; }
	DR_RINLINE void			set(const Size &s)			{ w = s.w; h = s.h; }

} Size;


typedef DR_GUI_PUB struct Rect
{
public:
	GuiCoord			fx, fy;
	GuiCoord			sx, sy;

public:
	DR_RINLINE			Rect(GuiCoord x, GuiCoord y, GuiSize w, GuiSize h);
	DR_RINLINE			Rect(const Point &p, const Size &s);
	DR_RINLINE			Rect(const None &);
	DR_RINLINE			Rect(const Rect &s, const Null &)	{ fx = 0; fy = 0; sx = s.sx-s.fx; sy = s.sy-s.fy; }

public:
	DR_RINLINE void			set(GuiCoord x, GuiCoord y, GuiSize w, GuiSize h);
	DR_RINLINE void			setCoord(GuiCoord fx, GuiCoord fy, GuiCoord sx, GuiCoord sy);
	DR_RINLINE void			setCoordo(GuiCoord fx, GuiCoord fy, GuiCoord sx, GuiCoord sy);
	DR_RINLINE void			setLT(GuiCoord x, GuiCoord y)		{ fx = x; fy = y; }
	DR_RINLINE void			setLT(const Point &point)		{ fx = point.x; fy = point.y; }
	DR_RINLINE void			setRB(GuiCoord x, GuiCoord y)		{ sx = x+1; sy = y+1; }
	DR_RINLINE void			setRB(const Point &point)		{ sx = point.x+1; sy = point.y+1; }
	DR_RINLINE void			setRBo(GuiCoord x, GuiCoord y)		{ sx = x; sy = y; }
	DR_RINLINE void			setRBo(const Point &point)		{ sx = point.x; sy = point.y; }
	DR_RINLINE void			setSize(GuiSize w, GuiSize h)		{ sx = fx+w; sy = fy+w; }
	DR_RINLINE void			setSize(const Size &size)		{ sx = fx+size.w; sy = fy+size.h; }
	DR_RINLINE void			extend(const Rect &s);

	DR_RINLINE void			moveHoriz(int chg)			{ fx += chg; sx += chg; }
	DR_RINLINE void			moveVert(int chg)			{ fy += chg; sy += chg; }
	DR_RINLINE void			move(int hchg, int vchg)		{ fx += hchg; fy += vchg; sx += hchg; sy += vchg; }
	DR_RINLINE void			move(const Point &chg)			{ fx += chg.x; fy += chg.y; sx += chg.x; sy += chg.y; }
	DR_RINLINE void			move0()					{ sx -= fx; sy -= fy; fx = 0; fy = 0; }
	DR_RINLINE void			resizeRight(int chg)			{ sx += chg; }
	DR_RINLINE void			resizeBottom(int chg)			{ sy += chg; }
	DR_RINLINE void			resizeRB(int hchg, int vchg)		{ sx += hchg; sy += vchg; }
	DR_RINLINE void			resizeLeft(int chg)			{ fx -= chg; }
	DR_RINLINE void			resizeTop(int chg)			{ fy -= chg; }
	DR_RINLINE void			resizeLT(int hchg, int vchg)		{ fx -= hchg; fy -= vchg; }

	DR_RINLINE GuiCoord		getL() const				{ return fx; }
	DR_RINLINE GuiCoord		getT() const				{ return fy; }
	DR_RINLINE GuiCoord		getR() const				{ return sx-1; }
	DR_RINLINE GuiCoord		getB() const				{ return sy-1; }
	DR_RINLINE GuiCoord		getLo() const				{ return sx; }
	DR_RINLINE GuiCoord		getBo() const				{ return sy; }
	DR_RINLINE GuiCoord		getW() const				{ return sx-fx; }
	DR_RINLINE GuiCoord		getH() const				{ return sy-fy; }
	DR_RINLINE Point		getLT() const				{ return Point(fx, fy); }
	DR_RINLINE Point		getRB() const				{ return Point(sx-1, sy-1); }
	DR_RINLINE Point		getLB() const				{ return Point(fx, sy-1); }
	DR_RINLINE Point		getRT() const				{ return Point(sx-1, fy); }
	DR_RINLINE Point		getRTo() const				{ return Point(sx, sy); }
	DR_RINLINE Point		getWHi() const				{ return Point(sx-fx-1, sy-fy-1); }
	DR_RINLINE Size			getSize() const				{ return Size(sx-fx, sy-fy); }

	DR_RINLINE void			grow(GuiCoord i)			{ fx -= i; fy -= i; sx += i; sy += i; }

	DR_RINLINE void			extendP(GuiCoord s_fx, GuiCoord s_fy, GuiCoord s_sx, GuiCoord s_sy);
	DR_RINLINE void			extendS(GuiCoord s_fx, GuiCoord s_fy, GuiSize s_w, GuiSize s_h);

} Rect;



DR_RINLINE Rect::Rect(GuiCoord x_, GuiCoord y_, GuiSize w_, GuiSize h_):
	fx(x_), fy(y_),
	sx(fx+w_), sy(fy+h_)
{
}

DR_RINLINE Rect::Rect(const Point &p, const Size &s):
	fx(p.x), fy(p.y),
	sx(fx+s.w), sy(fy+s.h)
{
}

DR_RINLINE Rect::Rect(const None &)
{
}

DR_RINLINE void Rect::set(GuiCoord x_, GuiCoord y_, GuiSize w_, GuiSize h_)
{
	fx = x_;
	fy = y_;
	sx = fx+w_;
	sy = fy+h_;
}

DR_RINLINE void Rect::setCoord(GuiCoord fx_, GuiCoord fy_, GuiCoord sx_, GuiCoord sy_)
{
	fx = fx_;
	fy = fy_;
	sx = sx_+1;
	sy = sy_+1;
}

DR_RINLINE void Rect::setCoordo(GuiCoord fx_, GuiCoord fy_, GuiCoord sx_, GuiCoord sy_)
{
	fx = fx_;
	fy = fy_;
	sx = sx_;
	sy = sy_;
}

DR_RINLINE void Rect::extend(const Rect &s)
{
	if (s.fx < fx)
		fx = s.fx;
	if (s.fy < fy)
		fy = s.fy;
	if (s.sx > sx)
		sx = s.sx;
	if (s.sy > sy)
		sy = s.sy;
}

DR_RINLINE void Rect::extendS(GuiCoord s_fx, GuiCoord s_fy, GuiSize s_w, GuiSize s_h)
{
	if (s_fx < fx)
		fx = s_fx;
	if (s_fy < fy)
		fy = s_fy;
	if (s_fx+(GuiCoord)s_w > sx)
		sx = s_fx+s_w;
	if (s_fy+(GuiCoord)s_h > sy)
		sy = s_fy+s_h;
}

DR_RINLINE void Rect::extendP(GuiCoord s_fx, GuiCoord s_fy, GuiCoord s_sx, GuiCoord s_sy)
{
	if (s_fx < fx)
		fx = s_fx;
	if (s_fy < fy)
		fy = s_fy;
	if (s_sx > sx)
		sx = s_sx;
	if (s_sy > sy)
		sy = s_sy;
}


DR_GUI_NS_END

#endif
