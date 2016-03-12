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

#include <pthread.h>

#include <dr/Thread.hxx>
#include <dr/RWLock.hxx>

DR_NS_BEGIN


/**
 * POSIX threads Read-Write Lock implementation
 */
class DR_PUB RWLock_pthread: public RWLock
{
public:
	virtual void			rlock();
	virtual void			wlock();
	virtual void			unlock();

	pthread_rwlock_t		m;
	Thread *			wowner;

	DR_DINLINE			RWLock_pthread();
	DR_DINLINE			~RWLock_pthread();

	class DR_PUB Man
	{
	public:
		pthread_rwlockattr_t attr;
		DR_DINLINE Man() { pthread_rwlockattr_init(&attr); }
		DR_DINLINE ~Man() { pthread_rwlockattr_destroy(&attr); }
	};
	static Man man;
};

RWLock_pthread::Man RWLock_pthread::man;

DR_DINLINE RWLock_pthread::RWLock_pthread()
{
	wowner = NULL;
	pthread_rwlock_init(&m, &man.attr);
}

DR_DINLINE RWLock_pthread::~RWLock_pthread()
{
	pthread_rwlock_destroy(&m);
}

DR_EXPORT_MET RWLock *RWLock::create()
{
	return new RWLock_pthread();
}

DR_EXPORT_MET void RWLock_pthread::rlock()
{
	pthread_rwlock_rdlock(&m);
}

DR_EXPORT_MET void RWLock_pthread::wlock()
{
	pthread_rwlock_wrlock(&m);
	wowner = Thread::current();
}

DR_EXPORT_MET void RWLock_pthread::unlock()
{
	if (wowner && Thread::current() == wowner)
		wowner = NULL;
	pthread_rwlock_unlock(&m);
}


DR_NS_END
