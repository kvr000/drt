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

#include <e32std.h>

#include <dr/x_kw.hxx>

#include <dr/Const.hxx>
#include <dr/Thread.intr.hxx>
#include <dr/MsgSync.hxx>
#include <dr/SysError.hxx>

#include <dr/dev/Thread_impl.hxx>
#include <dr/dev/ThreadDummy.hxx>

#include <dr/dev/Thread_symbian.hxx>

DR_NS_BEGIN


DR_OBJECT_DEF(DR_NS_STR, Thread_symbian, Thread_impl);

//DR_OBJECT_IMPL_SIMPLE(Thread_symbian);
DR_EXPORT_MET const Static *Thread_symbian::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(&comp_static, Const::string(DR_NS_STRP("Thread_symbian")), Super::getObjectStatic(), 0, NULL);
}


TInt Thread_symbian::main_func(void *arg)
{
	Thread_symbian *me = (Thread_symbian *)arg;

	Dll::SetTls(me->main);
	xtry {
		me->runMain();
	}
	xcatch (Throwable, ex) {
		me->setThrown(ex.getReset());
	}
	xend;
	finish_func(me->main);
	return 0;
}

void Thread_symbian::finish_func(void *main_)
{
	Thread *main = (Thread *)main_;
	if (!main)
		return;
	main->unref();
}

Thread_symbian::Thread_symbian(Thread *main_):
	DR_OBJECT_INIT(Thread_symbian, Thread_impl, (main_)),
	tls_data(NULL),
	tls_length(0)
{
	TInt err;
	joinlock.CreateLocal();

	if (!main->manager)
		main->manager = MsgSync::createDefault(main);
	attached = true;
	setImpl();
	if ((err = thread_desc.Create(_L(""), &main_func, 0, 0, 0, this)) != 0) {
		resetImpl();
		DR_THROWNEW(SysError(err));
	}
}

Thread_symbian::Thread_symbian(Thread *main_, TInt current):
	DR_OBJECT_INIT(Thread_symbian, Thread_impl, (main_)),
	tls_data(NULL),
	tls_length(0)
{
	joinlock.CreateLocal();

	//thread_desc.Open(current);

	attached = false;
	if (!main->manager)
		main->manager = MsgSync::createDefault(main);
	setImpl();
}

Thread_symbian::~Thread_symbian()
{
	if (attached)
		thread_desc.Close();
	MM::free(tls_data);
}

void Thread_symbian::wait()
{
	if (attached) {
		joinlock.Wait();
		if (attached) {
			TRequestStatus req;
			thread_desc.Logon(req);
			attached = false;
		}
		joinlock.Signal();
	}
}

#if 0
DR_EXPORT_MTS Thread *Thread::p_create(const Eslot1<void *, void *> &run_, const Eslot1<void, void *> &destroy_, void *data_)
{
	Thread_symbian *r;

	r = new Thread_symbian;
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
	if ((cur = (Thread *)Dll::Tls()) == NULL) {
		cur = new ThreadDummy();
		DR_TRY {
			new Thread_symbian(cur, 0);
		}
		DR_CATCHANY {
			cur->unref();
			DR_RETHROWANY;
		}
		DR_ENDTRY;
		Dll::SetTls(cur);
	}
	return cur->impl;
}

DR_EXPORT_MTS Thread *Thread::current()
{
	Thread *cur;
	if ((cur = (Thread *)Dll::Tls()) == NULL) {
		cur = new ThreadDummy();
		DR_TRY {
			new Thread_symbian(cur, 0);
		}
		DR_CATCHANY {
			cur->unref();
			DR_RETHROWANY;
		}
		DR_ENDTRY;
		Dll::SetTls(cur);
	}
	return cur;
}

Thread *Thread_impl::preinitMainThread()
{
	ThreadDummy *cur;
	cur = (ThreadDummy *)malloc(sizeof(ThreadDummy));
	Dll::SetTls(cur);
	return cur;
}

Thread *Thread_impl::postinitMainThread(Thread *created)
{
	new (created) ThreadDummy();
	new Thread_symbian(created, 0);
	return created;
}

bool Thread_symbian::static_preinit_mutex()
{
	Thread_symbian::allocated_tls_ids_lock.CreateLocal();
	return true;
}

DR_EXPORT_MTS void *Thread::getMsgIface(const String &iface_name)
{
	return current()->manager->getIface(iface_name);
}

DR_EXPORT_MET void Thread::start()
{
	ref();
	DR_TRY {
		impl = new Thread_symbian(this);
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
		impl = new Thread_symbian(this);
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
		impl = new Thread_symbian(this);
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
	Thread_symbian::allocated_tls_ids_lock.Wait();
	for (size_t i = 0; i < Thread_symbian::allocated_tls_ids_len; i++) {
		if (Thread_symbian::allocated_tls_ids[i] != -1) {
			int base = 0;
			UintPtr ids = Thread_symbian::allocated_tls_ids[i];
#if DR_SIZE_PTR > 4
			if ((Uint32)ids == 0xffffffff) {
				ids >>= 32;
				base += 32;
			}
#endif
			if ((Uint16)ids == 0xffff) {
				ids >>= 16;
				base += 16;
			}
			if ((Uint8)ids == 0xff) {
				ids >>= 8;
				base += 8;
			}
			if ((ids&0xf) == 0xf) {
				ids >>= 4;
				base += 4;
			}
			switch (ids&0xf) {
			case 0:
			case 2:
			case 4:
			case 6:
			case 8:
			case 10:
			case 12:
			case 14:
				base += 0;
				break;
			case 1:
			case 5:
			case 3:
			case 9:
			case 13:
				base += 1;
				break;
			case 7:
				base += 3;
				break;
			case 11:
				base += 2;
				break;
			case 15:
				// unreachable
				break;
			}
			Thread_symbian::allocated_tls_ids[i] |= (UintPtr)1<<base;
			return i*sizeof(Thread_symbian::allocated_tls_ids)*8+base;
		}
	}

	size_t add_length = Thread_symbian::allocated_tls_ids_len == 0 ? 4 : Thread_symbian::allocated_tls_ids_len;
	SintPtr *new_all = (SintPtr *)MM::realloc(Thread_symbian::allocated_tls_ids, (Thread_symbian::allocated_tls_ids_len+add_length)*sizeof(Thread_symbian::allocated_tls_ids[0]));
	if (!new_all) {
		DR_THROWNEW(SysError(KErrNoMemory));
	}
	Thread_symbian::allocated_tls_ids = new_all;
	memset(Thread_symbian::allocated_tls_ids+Thread_symbian::allocated_tls_ids_len, 0, add_length*sizeof(Thread_symbian::allocated_tls_ids[0]));
	Thread_symbian::allocated_tls_ids[Thread_symbian::allocated_tls_ids_len] = 1;
	Thread_symbian::allocated_tls_ids_len += add_length;
	add_length -= Thread_symbian::allocated_tls_ids_len;
	Thread_symbian::allocated_tls_ids_lock.Signal();
	return add_length*sizeof(Thread_symbian::allocated_tls_ids[0])*8;
}

DR_EXPORT_MTS void Thread::freeSpecific(long key)
{
	Thread_symbian::allocated_tls_ids_lock.Wait();
	Thread_symbian::allocated_tls_ids[key/8/sizeof(Thread_symbian::allocated_tls_ids[0])] &= ~((UintPtr)1<<(key&(8*sizeof(Thread_symbian::allocated_tls_ids[0]-1))));
	Thread_symbian::allocated_tls_ids_lock.Signal();
}

DR_EXPORT_MTS void *Thread::getSpecific(long key)
{
	Thread_symbian *impl = (Thread_symbian *)current()->impl;
	if ((size_t)key >= impl->tls_length)
		return NULL;
	return impl->tls_data[key];
}

DR_EXPORT_MTS void Thread::setSpecific(long key, void *val)
{
	Thread_symbian *impl = (Thread_symbian *)current()->impl;
	if ((size_t)key >= impl->tls_length) {
		size_t add_length = impl->tls_length == 0 ? 16 : impl->tls_length;
		void **new_data = (void **)MM::realloc(impl->tls_data, (impl->tls_length+add_length)*sizeof(impl->tls_data[0]));
		if (!new_data) {
			DR_THROWNEW(SysError(KErrNoMemory));
		}
		memset(new_data+impl->tls_length, 0, impl->tls_length*sizeof(impl->tls_data[0]));
		impl->tls_length += add_length;
		impl->tls_data = new_data;
	}
	impl->tls_data[key] = val;
}


DR_NS_END
