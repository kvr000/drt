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

#include <dr/Assert.hxx>
#include <dr/MutexCond.hxx>

DR_NS_BEGIN


/**
 * POSIX threads MutexCond implementation
 */
class DR_PUB MutexCond_pthread: public MutexCond
{
public:
	virtual void			lock();
	virtual void			unlock();
	virtual void			wait();
	virtual void			signal();
	virtual void			broadcast();

	pthread_mutex_t			mut;
	pthread_cond_t			cond;

	DR_DINLINE			MutexCond_pthread();
	DR_DINLINE			~MutexCond_pthread();

	class DR_PUB Man
	{
	public:
		bool				inited;
		pthread_mutexattr_t		mut_attr;
		pthread_condattr_t		cond_attr;
		DR_DINLINE			Man()					{ if (inited) return; inited = true;  pthread_mutexattr_init(&mut_attr); pthread_mutexattr_settype(&mut_attr, PTHREAD_MUTEX_RECURSIVE); pthread_condattr_init(&cond_attr); }
		DR_DINLINE			~Man()					{ pthread_condattr_destroy(&cond_attr); pthread_mutexattr_destroy(&mut_attr); }
	};
	static Man man;
};

MutexCond_pthread::Man MutexCond_pthread::man;

#define CHECKER(x) do { int ret = x; (void) ret; DR_Assert(ret == 0); } while (0)

DR_DINLINE MutexCond_pthread::MutexCond_pthread()
{
	CHECKER(pthread_mutex_init(&mut, &man.mut_attr));
	CHECKER(pthread_cond_init(&cond, &man.cond_attr));
}

DR_DINLINE MutexCond_pthread::~MutexCond_pthread()
{
	CHECKER(pthread_cond_destroy(&cond));
	CHECKER(pthread_mutex_destroy(&mut));
}

DR_EXPORT_MET MutexCond *MutexCond::create()
{
#if 0
	if (!MutexCond_pthread::man.inited)
		new (&MutexCond_pthread::man) MutexCond_pthread::Man;
#else
	DR_Assert(MutexCond_pthread::man.inited);
#endif
	return new MutexCond_pthread();
}

DR_EXPORT_MET void MutexCond_pthread::lock()
{
	CHECKER(pthread_mutex_lock(&mut));
}

DR_EXPORT_MET void MutexCond_pthread::unlock()
{
	CHECKER(pthread_mutex_unlock(&mut));
}

DR_EXPORT_MET void MutexCond_pthread::wait()
{
	CHECKER(pthread_cond_wait(&cond, &mut));
}

DR_EXPORT_MET void MutexCond_pthread::signal()
{
	CHECKER(pthread_cond_signal(&cond));
}

DR_EXPORT_MET void MutexCond_pthread::broadcast()
{
	CHECKER(pthread_cond_broadcast(&cond));
}


DR_NS_END
