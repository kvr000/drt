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

#include <dr/Const.hxx>
#include <dr/Subsystem.hxx>
#include <dr/Thread.hxx>
#include <dr/BadSubsysException.hxx>

#include <dr/gui/Gui.hxx>
#include <dr/gui/Widget.hxx>
#include <dr/gui/Visual.hxx>
#include <dr/gui/StyleFactory_Simple.hxx>
#include <dr/gui/dev/Gui_impl.hxx>

#include <dr/x_kw.hxx>

DR_GUI_NS_BEGIN


static String gui_impls[2] = 
{
	Const::string("drGui_wnt"),
	Const::string("drGui_x11"),
};

DR_OBJECT_DEF(DR_GUI_NS_STR, Gui, Object);

DR_OBJECT_IMPL_SIMPLE(Gui);

DR_EXPORT_MET Gui::Gui():
	DR_OBJECT_INIT(Gui, Object, ()),
	nowindow(Null()),
	quited(Null()),
	impl(NULL),
	window_count(0),
	object_list(NULL)
{
}

DR_EXPORT_MET Gui::~Gui()
{
	l_close();
}

DR_EXPORT_MET void Gui::setOptions(const String &options_)
{
	options = options_;
}

DR_EXPORT_MET void Gui::open(const Eslot1<bool, Gui *> &check_impl)
{
	unsigned i;
	Ref<Exception> ex_saved;
	int ex_prio = -1;

	for (i = 0; i < sizeof(gui_impls)/sizeof(gui_impls[0]); i++) {
		Subsystem *sys = NULL;
		Gui_impl *impl_ = NULL;
		xtry {
			sys = Subsystem::getSubsystem(gui_impls[i], NULL);
		}
		xcatch (Exception, ex) {
			if (ex_prio < 0) {
				ex_saved.setNoref(ex.getAndNull());
				ex_prio = 0;
			}
			goto next;
		}
		xend;
		xtry {
			impl_ = (Gui_impl *)sys->create(String(Null()), Gui_impl::comp_name);
		}
		xcatch (Exception, ex) {
			if (ex_prio < 2) {
				ex_saved.setNoref(ex.getAndNull());
				ex_prio = 2;
			}
			goto next;
		}
		xend;
		xtry {
			impl_->open(this);
		}
		xcatch (Exception, ex) {
			if (ex_prio < 4) {
				ex_saved.setNoref(ex.getAndNull());
				ex_prio = 4;
			}
			goto next;
		}
		xend;
		impl = impl_;
		xtry {
			if (!check_impl.isNull() && !check_impl(this))
				xthrownew(BadSubsysException(gui_impls[i], "Not satisfied with implementation"));
			sys->releaseSubsystem();
			goto have_impl;
		}
		xcatch (Exception, ex) {
			impl = NULL;
			if (ex_prio < 6) {
				ex_saved.setNoref(ex.getAndNull());
				ex_prio = 6;
			}
			goto next;
		}
		xend;
next:
		if (sys)
			sys->releaseSubsystem();
		if (impl_)
			impl_->unref();
	}
	if (!ex_saved)
		ex_saved.setNoref(new BadSubsysException("", "No subsystem available"));
	xthrowmove(ex_saved.getAndNull());

have_impl:
	if (nowindow.isNull())
		nowindow = Eslot0<void>(this, &Gui::close).aAdd<Gui *>().rIgnore<bool>();
}

DR_EXPORT_MET void Gui::close()
{
	if (!quited.isNull())
		quited(this);
}

DR_EXPORT_MET void Gui::flush()
{
	impl->flush();
}

DR_EXPORT_MET StyleFactory *Gui::accStyleFactory()
{
	if (!style_fac)
		style_fac.setNoref(new StyleFactory_Simple(this));
	return style_fac.getNoref();
}

DR_EXPORT_MET void Gui::runUntilQuit()
{
	int serial_quit;

	serial_quit = Thread::createMessageSerial();

	quited = Eslot1<void, int>(Thread::current(), &Thread::addAnswer).a1Set(serial_quit).aAdd<Gui *>();
	Thread::processMessages(serial_quit);
}

DR_EXPORT_MET void Gui::removeObject(Widget *obj)
{
	if (obj->child_prev == NULL) {
		DR_Assert(object_list == obj);
		if ((object_list = obj->child_next) != NULL)
			obj->child_next->child_prev = NULL;
	}
	else {
		if ((obj->child_prev->child_next = obj->child_next) != NULL)
			obj->child_next->child_prev = obj->child_prev;
	}
	if (obj->isWidget()) {
		if (--window_count == 0) {
			if (!nowindow.isNull())
				nowindow(this);
		}
	}
}

DR_EXPORT_MET void Gui::insertObject(Widget *obj)
{
	if (obj->isWidget())
		window_count++;

	if ((obj->child_next = object_list) != NULL)
		object_list->child_prev = obj;
	obj->child_prev = NULL;
	object_list = obj;
}

DR_EXPORT_MET void Gui::l_close()
{
	nowindow.setNull();
	while (object_list)
		object_list->gui_destroy();

	if (impl) {
		visuals.resize(0);
		impl->unref();
		impl = NULL;
	}
	style_fac.setNull();
}


DR_GUI_NS_END
