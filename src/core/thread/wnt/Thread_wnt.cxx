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
#include <stdio.h>

#include <dr/x_kw.hxx>

#include <dr/Const.hxx>
#include <dr/Thread.intr.hxx>
#include <dr/MsgSync.hxx>
#include <dr/SysError.hxx>

#include <dr/dev/Thread_impl.hxx>
#include <dr/dev/ThreadDummy.hxx>

DR_NS_BEGIN


/**
 * Windows threads Thread_impl implementation
 */
class DR_PUB Thread_wnt: public Thread_impl
{
	DR_OBJECT_DECL_SIMPLE(Thread_wnt, Thread_impl);

public:
	virtual void			wait();

protected:
	HANDLE				pt;
	CRITICAL_SECTION		joinlock;
	bool				attached;

protected:
	DR_RINLINE			Thread_wnt(Thread *main);
	DR_RINLINE			Thread_wnt(Thread *main, HANDLE current);
	DR_RINLINE			~Thread_wnt();

protected:
	static DWORD WINAPI		main_func(void *);
	static void			finish_func(void *);

protected:
	static DWORD			key_ptr;

private:
	friend class Thread;
	friend class Thread_impl;
};


DR_OBJECT_DEF(DR_NS_STR, Thread_wnt, Thread_impl);

//DR_OBJECT_IMPL_SIMPLE(Thread_wnt);
DR_EXPORT_MET const Static *Thread_wnt::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(Const::string(DR_NS_STRP("Thread_wnt")), &comp_static, Super::getObjectStatic(), 0, NULL);
}


DWORD Thread_wnt::key_ptr = (unsigned)-1;

DWORD WINAPI Thread_wnt::main_func(void *arg)
{
	Thread_wnt *me = (Thread_wnt *)arg;

	TlsSetValue(key_ptr, me->main);
	xtry {
		me->runMain();
	}
	xcatch (Throwable, ex) {
		me->setThrown(ex.getAndNull());
	}
	xend;
	finish_func(me->main);
	return 0;
}

void Thread_wnt::finish_func(void *main_)
{
	Thread *main = (Thread *)main_;
	if (!main)
		return;
	main->unref();
}

Thread_wnt::Thread_wnt(Thread *main_):
	DR_OBJECT_INIT(Thread_wnt, Thread_impl, (main_))
{
	InitializeCriticalSection(&joinlock);

	if (!main->manager)
		main->manager = MsgSync::createDefault(main);
	attached = true;
	setImpl();
	if ((pt = CreateThread(NULL, 0, &main_func, this, 0, NULL)) == NULL) {
		resetImpl();
		DR_THROWNEW(SysError(GetLastError()));
	}
}

Thread_wnt::Thread_wnt(Thread *main_, HANDLE current):
	DR_OBJECT_INIT(Thread_wnt, Thread_impl, (main_)),
	pt(current)
{
	InitializeCriticalSection(&joinlock);

	attached = false;
	if (!main->manager)
		main->manager = MsgSync::createDefault(main);
	setImpl();
}

Thread_wnt::~Thread_wnt()
{
	if (attached)
		CloseHandle(pt);
}

void Thread_wnt::wait()
{
	if (attached) {
		EnterCriticalSection(&joinlock);
		if (attached) {
			int err;
			if ((err = WaitForSingleObject(pt, INFINITE)) != WAIT_OBJECT_0) {
				LeaveCriticalSection(&joinlock);
				DR_THROWNEW(SysError(GetLastError()));
			}
			attached = false;
		}
		LeaveCriticalSection(&joinlock);
	}
}

#if 0
DR_EXPORT_MTS Thread *Thread::p_create(const Eslot1<void *, void *> &run_, const Eslot1<void, void *> &destroy_, void *data_)
{
	Thread_wnt *r;

	r = new Thread_wnt;
	r->run_slot = run_;
	r->destroy_slot = destroy_;
	r->data = data_;

	r->ref();
	if (!r->run()) {
		delete r;
		r = NULL;
	}
	return r;
}
#endif

DR_EXPORT_MTS Thread_impl *Thread_impl::currentImpl()
{
	Thread *cur;
	if ((cur = (Thread *)TlsGetValue(Thread_wnt::key_ptr)) == NULL) {
		cur = new ThreadDummy();
		DR_TRY {
			new Thread_wnt(cur, GetCurrentThread());
		}
		DR_CATCHANY {
			cur->unref();
			DR_RETHROWANY;
		}
		DR_ENDTRY;
		TlsSetValue(Thread_wnt::key_ptr, cur);
	}
	return cur->impl;
}

DR_EXPORT_MTS Thread *Thread::current()
{
	Thread *cur;
	if ((cur = (Thread *)TlsGetValue(Thread_wnt::key_ptr)) == NULL) {
		cur = new ThreadDummy();
		DR_TRY {
			new Thread_wnt(cur, GetCurrentThread());
		}
		DR_CATCHANY {
			cur->unref();
			DR_RETHROWANY;
		}
		DR_ENDTRY;
		TlsSetValue(Thread_wnt::key_ptr, cur);
	}
	return cur;
}

Thread *Thread_impl::preinitMainThread()
{
	ThreadDummy *cur;
	Thread_wnt::key_ptr = TlsAlloc();
	cur = (ThreadDummy *)malloc(sizeof(ThreadDummy));
	TlsSetValue(Thread_wnt::key_ptr, cur);
	return cur;
}

Thread *Thread_impl::postinitMainThread(Thread *created)
{
	new (created) ThreadDummy();
	new Thread_wnt(created, GetCurrentThread());
	return created;
}

DR_EXPORT_MTS void *Thread::getMsgIface(const String &iface_name)
{
	return current()->manager->getIface(iface_name);
}

DR_EXPORT_MET void Thread::start()
{
	ref();
	DR_TRY {
		impl = new Thread_wnt(this);
	}
	DR_CATCHANY {
		unref();
		DR_RETHROWANY;
	}
	DR_ENDTRY;
}

DR_EXPORT_MET void Thread::startUnref()
{
	DR_TRY {
		impl = new Thread_wnt(this);
	}
	DR_CATCHANY {
		unref();
		DR_RETHROWANY;
	}
	DR_ENDTRY;
	unref();
}

DR_EXPORT_MET void Thread::startThrowUnref()
{
	ref();
	DR_TRY {
		impl = new Thread_wnt(this);
	}
	DR_CATCHANY {
		unref();
		unref();
		DR_RETHROWANY;
	}
	DR_ENDTRY;
}

DR_EXPORT_MTS long Thread::createSpecific()
{
	DWORD k;
	if ((k = TlsAlloc()) == 0xffffffff) {
		throw SysError(GetLastError());
	}
	return k;
}

DR_EXPORT_MTS void Thread::freeSpecific(long key)
{
	TlsFree(key);
}

DR_EXPORT_MTS void *Thread::getSpecific(long key)
{
	return TlsGetValue(key);
}

DR_EXPORT_MTS void Thread::setSpecific(long key, void *val)
{
	TlsSetValue(key, val);
}


DR_NS_END
