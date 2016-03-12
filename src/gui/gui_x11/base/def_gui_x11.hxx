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

#ifndef dr__gui__x11__drdef_gui_x11__hxx__
# define dr__gui__x11__drdef_gui_x11__hxx__

#include <dr/defs.hxx>

#ifdef DR_COMPILING_GUI_X11
# define DR_GUI_X11_FUN DR_EXPORT_FUN
# define DR_GUI_X11_MET DR_EXPORT_MET
# define DR_GUI_X11_MTS DR_EXPORT_MTS
# define DR_GUI_X11_DAT DR_EXPORT_DAT
# define DR_GUI_X11_DTC DR_EXPORT_DTC
# define DR_GUI_X11_CLS DR_EXPORT_CLS
#else
# define DR_GUI_X11_FUN DR_IMPORT_FUN
# define DR_GUI_X11_MET DR_IMPORT_MET
# define DR_GUI_X11_MTS DR_IMPORT_MTS
# define DR_GUI_X11_DAT DR_IMPORT_DAT
# define DR_GUI_X11_DTC DR_IMPORT_DTC
# define DR_GUI_X11_CLS DR_IMPORT_CLS
#endif


#endif
