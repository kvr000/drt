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
#include <ctype.h>
#include <limits.h>

#if (defined DR_OS_UNIX)
# include <dlfcn.h>
#elif (defined DR_OS_WNT)
# include <windows.h>
# ifdef  DR_LIBC_CYGWIN
#  include <wchar.h>
# else
#  include <strsafe.h>
# endif
#endif

#include <dr/x_kw.hxx>

#include <dr/Atomic.hxx>
#include <dr/Mutex.hxx>
#include <dr/Hash.hxx>

#include <dr/Subsystem.hxx>
#include <dr/BadSubsysExcept.hxx>

DR_NS_BEGIN


DR_ANON_BEGIN;
typedef struct
{
	String				name;
	Subsystem *			subsys;
	void *				library;
} subsystem_struct;
DR_ANON_END;

static THash<String, subsystem_struct> subsystems;
static Mutex *subsystems_lock;


void Subsystem::s_init()
{
	Mutex::createOnce(&subsystems_lock);
}

Subsystem *Subsystem::getSubsystem(const String &name, Subsystem *(*create_func)(const String &name))
{
	Subsystem *subsys = NULL;
	void *dl = NULL;

	MutexLocker subsys_locker(Mutex::createOnce(&subsystems_lock));

	xtry {
		if (THash<String, subsystem_struct>::Node *ss = subsystems.find(name)) {
			subsys = ss->v.subsys;
			subsys->ref();
			goto out;
		}
		if (create_func == NULL) {
#if (defined DR_OS_UNIX)
			char libfile[64];
			if ((unsigned)snprintf(libfile, sizeof(libfile), "lib%s.so", name.utf8().toStr()) >= sizeof(libfile))
				xthrownew(BadSubsysExcept(name, "too long name"));
			for (int ic = 0; libfile[ic] != '\0'; ic++)
				libfile[ic] = tolower(libfile[ic]);
			if ((dl = dlopen(libfile, RTLD_NOW|RTLD_GLOBAL)) == NULL)
				xthrownew(BadSubsysExcept(name, String("unable to load library ").append(libfile).append(": ").append(dlerror())));
			if ((unsigned)snprintf(libfile, sizeof(libfile), "createSS_%s", name.utf8().toStr()) >= sizeof(libfile))
				xthrownew(BadSubsysExcept(name, String("too long function name")));
			if ((create_func = (Subsystem *(*)(const String &))dlsym(dl, libfile)) == NULL)
				xthrownew(BadSubsysExcept(name, String("cannot find function ").append(libfile)));
#else
			union {
				WCHAR libfile[64];
				char libfunc[64];
			};
			if (
#ifdef DR_LIBC_CYGWIN
				snprintf(libfunc, sizeof(libfunc), "%s.dll", name.utf8().toStr()) >= (ssize_t)(sizeof(libfunc)/sizeof(libfunc[0]))
#else
				StringCbPrintfW(libfile, sizeof(libfile), L"%s.dll", name.wide().toStr()) != 0
#endif
			)
				xthrownew(BadSubsysExcept(name, "too long name"));
			if (
#ifdef DR_LIBC_CYGWIN
					(dl = LoadLibraryExA(libfunc, NULL, 0))
#else
					(dl = LoadLibraryExW(libfile, NULL, 0))
#endif
					== NULL) {
				xthrownew(BadSubsysExcept(name, "unable to load library "));
			}
			if (
#ifdef DR_LIBC_CYGWIN
					snprintf(libfunc, sizeof(libfunc), "createSS_%S", name.wide().toStr()) >= (ssize_t)(sizeof(libfunc)/sizeof(libfunc[0]))
#else
					StringCbPrintfA(libfunc, sizeof(libfile)/sizeof(libfile[0]), "createSS_%S", name.wide().toStr()) != 0
#endif
			)
				xthrownew(BadSubsysExcept(name, "too long function name"));
			if ((create_func = (Subsystem *(*)(const String &))
#ifdef DR_OS_WCE
				GetProcAddressA
#else
				GetProcAddress
#endif
				((HMODULE)dl, libfunc)) == NULL)
				xthrownew(BadSubsysExcept(name, "too long function name"));
#endif
		}
		{
			subsystem_struct sss;
			sss.name = name;
			if ((subsys = sss.subsys = create_func(name)) == NULL)
				xthrownew(BadSubsysExcept(name, "create_func returned NULL"));
			sss.library = dl;
			subsystems[name] = sss;
			dl = NULL;
		}
	}
	xcatchany {
		if (subsys)
			subsys->unref();
		if (dl) {
#if (defined DR_OS_UNIX)
			dlclose(dl);
#elif (defined DR_OS_WNT)
			FreeLibrary((HMODULE)dl);
#else
# error unknown system
#endif
		}
		xrethrowany;
	}
	xend;

out:
	subsys->busy_ref();
	return subsys;
}

void Subsystem::releaseSubsystem()
{
	MutexLocker subsys_locker(Mutex::createOnce(&subsystems_lock));

	busy_unref();
	/* never delete subsystem */
	/*subsystems[i].name.setNull();*/
}

DR_EXPORT_MET Subsystem::Subsystem()
	: busy_count(-1)
{
}

DR_EXPORT_MET Subsystem::~Subsystem()
{
}

DR_EXPORT_MET Object *Subsystem::create(const String &object)
{
	return NULL;
}

DR_EXPORT_MET void *Subsystem::create(const String &object, const String &iface_name)
{
	Object *c;
	void *iface;

	if ((c = create(object)) == NULL)
		return NULL;
	iface = c->getIfaceUnref(iface_name);
	return iface;
}


DR_NS_END
