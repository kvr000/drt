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
#include <errno.h>
#include <pthread.h>

#include <dr/x_kw.hxx>

#include <dr/Const.hxx>
#include <dr/Thread.intr.hxx>
#include <dr/MsgSync.hxx>
#include <dr/SysError.hxx>

#include <dr/dev/Thread_impl.hxx>
#include <dr/dev/ThreadDummy.hxx>

DR_NS_BEGIN


/**
 * POSIX threads Thread_impl implementation
 */
class DR_PUB Thread_pthread: public Thread_impl
{
public:
	virtual void			wait();

protected:
	pthread_t			pt;
	pthread_mutex_t			joinlock;
	bool				attached;

protected:
	DR_RINLINE			Thread_pthread(Thread *main);
	DR_RINLINE			Thread_pthread(Thread *main, pthread_t current);
	DR_RINLINE			~Thread_pthread();

protected:
	DR_NINLINE static void		createCurrent();

protected:
	static void *			main_func(void *);
	static void			finish_func(void *);

protected:
	static pthread_key_t		key_ptr;
	static void			key_ptr_create();

private:
	friend class Thread;
	friend class Thread_impl;
};


#if 0
DR_OBJECT_DEF(DR_NS_STR, Thread_pthread, Thread_impl);
//DR_OBJECT_IMPL_SIMPLE(Thread_pthread);
DR_EXPORT_MET const Static *Thread_pthread::getObjectStatic() const
{
	String comp_name_local(Const::string(DR_NS_STRP("Thread_pthread")));
	if (comp_static)
		return comp_static;
	return createObjectStatic(comp_name_local, &comp_static, Super::getObjectStatic(), 0, NULL);
}
#endif


pthread_key_t Thread_pthread::key_ptr = (pthread_key_t)(SintPtr)-1;

void Thread_pthread::key_ptr_create()
{
}

void *Thread_pthread::main_func(void *arg)
{
	Thread_pthread *me = (Thread_pthread *)arg;
	pthread_setspecific(key_ptr, me);

	xtry {
		me->runMain();
	}
	xcatch (Throwable, ex) {
		me->setThrown(ex.getAndNull());
	}
	xend;
	return NULL;
}

void Thread_pthread::finish_func(void *impl_)
{
	Thread_pthread *impl = (Thread_pthread *)impl_;
	pthread_setspecific(key_ptr, impl);
	impl->ref();
	if (impl->main)
		impl->main->unref();
	impl->threadPreDestroy();
	pthread_setspecific(key_ptr, NULL);
	impl->unref();
}

Thread_pthread::Thread_pthread(Thread *main_):
	Thread_impl(main_)
{
	int err;
	if ((err = pthread_mutex_init(&joinlock, NULL)) != 0)
		xthrownew(SysError(err));

	if (!main->manager)
		main->manager = MsgSync::createDefault(main);
	attached = true;
	setImpl();
	if ((err = pthread_create(&pt, NULL, &main_func, this)) != 0) {
		resetImpl();
		xthrownew(SysError(err));
	}
}

Thread_pthread::Thread_pthread(Thread *main_, pthread_t current):
	Thread_impl(None())
{
	int err;
	if ((err = pthread_mutex_init(&joinlock, NULL)) != 0)
		xthrownew(SysError(err));

	attached = false;
	if (!main->manager)
		main->manager = MsgSync::createDefault(main);
	setImpl();
}

Thread_pthread::~Thread_pthread()
{
	if (attached)
		pthread_detach(pt);
	pthread_mutex_destroy(&joinlock);
}

void Thread_pthread::wait()
{
	if (attached) {
		pthread_mutex_lock(&joinlock);
		if (attached) {
			int err;
			if ((err = pthread_join(pt, NULL)) != 0) {
				pthread_mutex_unlock(&joinlock);
				xthrownew(SysError(err));
			}
			attached = false;
		}
		pthread_mutex_unlock(&joinlock);
	}
}

#if 0
DR_EXPORT_MTS Thread *Thread::p_create(const Eslot1<void *, void *> &run_, const Eslot1<void, void *> &destroy_, void *data_)
{
	Thread_pthread *r;

	r = new Thread_pthread;
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

void Thread_pthread::createCurrent()
{
	Thread_pthread *cur_impl = (Thread_pthread *)malloc(sizeof(Thread_pthread));
	memset(cur_impl, 0, sizeof(Thread_pthread));
	cur_impl->threadPreInit();
	pthread_setspecific(Thread_pthread::key_ptr, cur_impl);
	ThreadDummy *cur = NULL;
	xtry {
		MM::initingThread();
		cur = new ThreadDummy();
		new (cur_impl) Thread_pthread(cur, pthread_self());
	}
	xcatchany {
		pthread_setspecific(Thread_pthread::key_ptr, NULL);
		if (cur)
			cur->unref();
		free(cur_impl);
		xrethrowany;
	}
	xend;
}

DR_EXPORT_MTS Thread_impl *Thread_impl::currentImpl()
{
	Thread_impl *cur_impl;
	while ((cur_impl = (Thread_pthread *)pthread_getspecific(Thread_pthread::key_ptr)) == NULL) {
		Thread_pthread::createCurrent();
	}
	return cur_impl;
}

DR_EXPORT_MTS Thread *Thread::current()
{
	Thread_impl *cur_impl;
	while ((cur_impl = (Thread_pthread *)pthread_getspecific(Thread_pthread::key_ptr)) == NULL) {
		Thread_pthread::createCurrent();
	}
	return cur_impl->main;
}

Thread *Thread_impl::preinitMainThread()
{
	ThreadDummy *cur;
	pthread_key_create(&Thread_pthread::key_ptr, &Thread_pthread::finish_func);
	cur = (ThreadDummy *)malloc(sizeof(ThreadDummy));
	memset(cur, 0, sizeof(ThreadDummy));
	Thread_pthread *cur_impl = (Thread_pthread *)malloc(sizeof(Thread_pthread));
	memset(cur_impl, 0, sizeof(Thread_pthread));
	pthread_setspecific(Thread_pthread::key_ptr, cur_impl);
	MM::initingThread();
	cur_impl->main = cur;
	cur_impl->setImpl();
	return cur;
}

Thread *Thread_impl::postinitMainThread(Thread *created)
{
	new (created) ThreadDummy(None());
	new (created->impl) Thread_pthread(created, pthread_self());
	return created;
}

DR_EXPORT_MTS void *Thread::getMsgIface(const String &iface_name)
{
	return current()->manager->getIface(iface_name);
}

DR_EXPORT_MET void Thread::start()
{
	ref();
	xtry {
		impl = new Thread_pthread(this);
	}
	xcatchany {
		unref();
		xrethrowany;
	}
	xend;
}

DR_EXPORT_MET void Thread::startUnref()
{
	xtry {
		impl = new Thread_pthread(this);
	}
	xcatchany {
		unref();
		xrethrowany;
	}
	xend;
	unref();
}

DR_EXPORT_MET void Thread::startThrowUnref()
{
	ref();
	xtry {
		impl = new Thread_pthread(this);
	}
	xcatchany {
		unref();
		unref();
		xrethrowany;
	}
	xend;
}

DR_EXPORT_MTS long Thread::createSpecific()
{
	pthread_key_t key;

	if (pthread_key_create(&key, NULL) < 0)
		return -1;
	return (long)key;
}

DR_EXPORT_MTS void Thread::freeSpecific(long key)
{
	pthread_key_delete((pthread_key_t)key);
}

DR_EXPORT_MTS void *Thread::getSpecific(long key)
{
	return pthread_getspecific((pthread_key_t)key);
}

DR_EXPORT_MTS void Thread::setSpecific(long key, void *val)
{
	pthread_setspecific((pthread_key_t)key, val);
}


DR_NS_END
