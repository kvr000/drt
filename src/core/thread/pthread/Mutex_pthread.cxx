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

#include <dr/Mutex.hxx>

DR_NS_BEGIN


/**
 * POSIX threads Mutex implementation
 */
class DR_PUB Mutex_pthread: public Mutex
{
public:
	virtual void lock();
	virtual void unlock();

	pthread_mutex_t	m;

	DR_DINLINE Mutex_pthread();
	DR_DINLINE ~Mutex_pthread();

	class DR_PUB Man
	{
	public:
		pthread_mutexattr_t attr;
		DR_DINLINE Man() { pthread_mutexattr_init(&attr); pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); }
		DR_DINLINE ~Man() { pthread_mutexattr_destroy(&attr); }
	};
	static Man man;
};

Mutex_pthread::Man Mutex_pthread::man;

DR_DINLINE Mutex_pthread::Mutex_pthread()
{
	pthread_mutex_init(&m, &man.attr);
}

DR_DINLINE Mutex_pthread::~Mutex_pthread()
{
	pthread_mutex_destroy(&m);
}

DR_EXPORT_MET Mutex *Mutex::create()
{
	return new Mutex_pthread();
}

DR_EXPORT_MET void Mutex_pthread::lock()
{
	pthread_mutex_lock(&m);
}

DR_EXPORT_MET void Mutex_pthread::unlock()
{
	pthread_mutex_unlock(&m);
}


DR_NS_END
