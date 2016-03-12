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

#ifndef dr__Mutex__hxx__
# define dr__Mutex__hxx__

#include <dr/Shared.hxx>

DR_NS_BEGIN


/**
 * Mutex interface
 */
class DR_PUB Mutex: public Shared
{
public:
	static Mutex *			create();
	DR_MINLINE static Mutex *	createOnce(Mutex **addr);
	static Mutex *			lockAddr(void *p);
	static void			unlockAddr(void *p);

public:
	virtual void			lock() = 0;
	virtual void			unlock() = 0;

protected:
	DR_MINLINE			Mutex()					{}

	DR_MINLINE virtual		~Mutex()				{}

	static Mutex *			p_createOnce(Mutex **addr);
};


DR_MINLINE Mutex *Mutex::createOnce(Mutex **addr)
{
	return *addr ? *addr : p_createOnce(addr);
}


/**
 * Mutex locker
 *
 * The class locks the mutex in the constructor and unlocks it in destructor.
 */
class DR_PUB MutexLocker
{
protected:
	Mutex *				mutex;

public:
	DR_RINLINE			MutexLocker(Mutex *mutex_)		: mutex(mutex_) { mutex->lock(); }
	DR_RINLINE			~MutexLocker()				{ mutex->unlock(); }
};


DR_NS_END

#endif
