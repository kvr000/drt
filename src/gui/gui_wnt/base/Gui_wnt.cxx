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

#include <windows.h>

#include <dr/x_kw.hxx>

#include <dr/Const.hxx>
#include <dr/Subsystem.hxx>
#include <dr/BadSubsysException.hxx>
#include <dr/MsgSync.hxx>

#include <dr/gui/wnt/Gui_wnt.hxx>
#include <dr/gui/wnt/Widget_wnt.hxx>
#include <dr/gui/wnt/Visual_wnt.hxx>
#include <dr/gui/wnt/Font_wnt.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, Gui_wnt, Gui_impl);

DR_OBJECT_IMPL_SIMPLE(Gui_wnt);

Gui_wnt::Gui_wnt(Gui_SS_wnt *ss):
	DR_OBJECT_INIT(Gui_wnt, Gui_impl, ()),
	global(ss),
	app_instance(NULL),
	window_class(0)
{
	global->busy_ref();

	weak_ref = new WeakRef<Gui_wnt>(this);
}

Gui_wnt::~Gui_wnt()
{
	default_font.setNull();
	if (window_class)
		UnregisterClassW((WCHAR *)(LONG_PTR)window_class, app_instance);
	if (msg_sync) {
		msg_sync->unregisterWntMsgReceiver();
		msg_sync.setNull();
	}
	weak_ref->reset();
	global->busy_unref();
}

void Gui_wnt::open(Gui *gui_)
{
	Eslot0<void> islot(Eslot1<void, WeakRef<Gui_wnt> *>(&msg_notify).a1Set<IRef<WeakRef<Gui_wnt> > >(weak_ref));

	gui = gui_;

	xtry {
		msg_sync = (MsgSync_WntMsg *)Thread::getMsgIface(MsgSync_WntMsg::iface_name);
	}
	xcatch (Exception, ex) {
		xthrownew(BadSubsysException(ex.getAndNull(), Const::string("Gui_SS_wnt"), "failed to get MsgSync_WntMsg interface"));
	}
	xend;
	xtry {
		msg_sync->registerWntMsgReceiver(islot);
	}
	xcatchany {
		msg_sync->unregisterWntMsgReceiver();
		msg_sync.getAndNull();
		xrethrowany;
	}
	xend;

	{
		default_visual.setNoref(new Visual_wnt(this));
		gui->visuals.appendDoref(default_visual);
	}

	{
#if 1
		LOGFONT font_log;
		memset(&font_log, 0, sizeof(font_log));
#ifdef CLEARTYPE_QUALITY
		font_log.lfQuality = CLEARTYPE_QUALITY;
#else
		font_log.lfQuality = 0;
#endif

		HFONT font = CreateFontIndirect(&font_log);
#else
		HFONT font = CreateFontW(80, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
#endif
		if (!font) {
			DR_THROWNEW(BadSubsysException(Const::string("Gui_SS_wnt"), "failed to create font"));
		}
		default_font.setNoref(new Font_wnt(this, font));
	}

	app_instance = GetModuleHandleW(NULL);

#ifdef DR_OS_WCE
	{
		WNDCLASSW wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc = &Gui_wnt::wndproc_handler;
		wc.cbWndExtra = sizeof(void *);
		wc.hInstance = app_instance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
		wc.lpszClassName = L"drgui_wnt_window";
		window_class = RegisterClassW(&wc);
	}
#else
	{
		WNDCLASSEXW wc;
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc = &Gui_wnt::wndproc_handler;
		wc.cbWndExtra = sizeof(void *);
		wc.hInstance = app_instance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = L"drgui_wnt_window";
		window_class = RegisterClassExW(&wc);
	}
#endif

	return;
}

void Gui_wnt::flush()
{
}

Object *Gui_SS_wnt::create(const String &object)
{
	if (object == Gui_impl::comp_name)
		return (Gui_impl *)new Gui_wnt(this);
	return NULL;
}

void *Gui_SS_wnt::create(const String &object, const String &iface)
{
	if (!object.isNull())
		return Subsystem::create(object, iface);
	if (iface == Gui_impl::comp_name)
		return (Gui_impl *)new Gui_wnt(this);
	return NULL;
}

Object *Gui_SS_wnt::create(int object)
{
	return NULL;
}

void *Gui_SS_wnt::create(int object, int iface)
{
	return NULL;
}

Gui_SS_wnt::Gui_SS_wnt()
{
}

Gui_SS_wnt::~Gui_SS_wnt()
{
}


DR_GUI_NS_END

DR_NS_USE
DR_GUI_NS_USE

extern "C" __declspec(dllexport) dr::Subsystem *createSS_dr_gui_wnt(const String &name)
{
	return new dr::gui::Gui_SS_wnt;
}

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
