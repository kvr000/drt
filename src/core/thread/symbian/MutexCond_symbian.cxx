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

#include <e32base.h>

#include <dr/Assert.hxx>
#include <dr/SysError.hxx>

#include <dr/MutexCond.hxx>

DR_NS_BEGIN


/**
 * Symbian threads MutexCond implementation
 */
class DR_PUB MutexCond_symbian: public MutexCond
{
public:
	virtual void			lock();
	virtual void			unlock();
	virtual void			wait();
	virtual void			signal();
	virtual void			broadcast();

	RMutex				mutex;
	RSemaphore			semaphore;
	Refcnt				waiting;

	DR_DINLINE			MutexCond_symbian();
	DR_DINLINE			~MutexCond_symbian();
};


DR_DINLINE MutexCond_symbian::MutexCond_symbian()
{
	TInt err;
	if ((err = mutex.CreateLocal()) != 0)
		DR_THROWNEW(SysError(err));
	if ((err = semaphore.CreateLocal(0)) != 0)
		DR_THROWNEW(SysError(err));
	waiting = 0;
}

DR_DINLINE MutexCond_symbian::~MutexCond_symbian()
{
}

DR_EXPORT_MET MutexCond *MutexCond::create()
{
	return new MutexCond_symbian();
}

DR_EXPORT_MET void MutexCond_symbian::lock()
{
	mutex.Wait();
}

DR_EXPORT_MET void MutexCond_symbian::unlock()
{
	mutex.Signal();
}

DR_EXPORT_MET void MutexCond_symbian::wait()
{
	waiting++;
	mutex.Signal();
	semaphore.Wait();
	mutex.Wait();
}

DR_EXPORT_MET void MutexCond_symbian::signal()
{
	if (waiting > 0) {
		waiting--;
		semaphore.Signal();
	}
}

DR_EXPORT_MET void MutexCond_symbian::broadcast()
{
	if (waiting > 0) {
		semaphore.Signal(waiting);
		waiting = 0;
	}
}


DR_NS_END
