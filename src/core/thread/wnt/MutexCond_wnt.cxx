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

#include <dr/Assert.hxx>
#include <dr/MutexCond.hxx>

DR_NS_BEGIN


/**
 * Windows threads MutexCond implementation
 */
class DR_PUB MutexCond_wnt: public MutexCond
{
public:
	virtual void			lock();
	virtual void			unlock();
	virtual void			wait();
	virtual void			signal();
	virtual void			broadcast();

	CRITICAL_SECTION		mutex;
	HANDLE				semaphore;
	Refcnt				waiting;

	DR_DINLINE			MutexCond_wnt();
	DR_DINLINE			~MutexCond_wnt();
};


DR_DINLINE MutexCond_wnt::MutexCond_wnt():
	semaphore(NULL)
{
	InitializeCriticalSection(&mutex);
	semaphore = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);
	waiting = 0;
}

DR_DINLINE MutexCond_wnt::~MutexCond_wnt()
{
	if (semaphore)
		CloseHandle(semaphore);
	DeleteCriticalSection(&mutex);
}

DR_EXPORT_MET MutexCond *MutexCond::create()
{
	return new MutexCond_wnt();
}

DR_EXPORT_MET void MutexCond_wnt::lock()
{
	EnterCriticalSection(&mutex);
}

DR_EXPORT_MET void MutexCond_wnt::unlock()
{
	LeaveCriticalSection(&mutex);
}

DR_EXPORT_MET void MutexCond_wnt::wait()
{
	waiting++;
	LeaveCriticalSection(&mutex);
	WaitForSingleObject(semaphore, INFINITE);
	EnterCriticalSection(&mutex);
}

DR_EXPORT_MET void MutexCond_wnt::signal()
{
	if (waiting > 0) {
		waiting--;
		ReleaseSemaphore(semaphore, 1, NULL);
	}
}

DR_EXPORT_MET void MutexCond_wnt::broadcast()
{
	if (waiting > 0) {
		ReleaseSemaphore(semaphore, waiting, NULL);
		waiting = 0;
	}
}


DR_NS_END
