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

#ifndef dr__gui__x11__Visual_x11__hxx__
# define dr__gui__x11__Visual_x11__hxx__

#include <X11/Xlib.h>

#include <dr/gui/x11/X11wrap/redef_x11.hxx>

#include <dr/gui/Visual.hxx>

#include <dr/gui/x11/Gui_x11.hxx>

DR_GUI_NS_BEGIN


class Visual_x11: public Visual
{
public:
	VisualID			visual_id;

public:
	DR_CONSTRUCT			Visual_x11(Gui_x11 *gui, X11_Visual *visual);
};


DR_GUI_NS_END

#endif
