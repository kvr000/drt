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

#include <dr/Mutex.hxx>

DR_NS_BEGIN


/**
 * Windows threads Mutex implementation
 */
class DR_PUB Mutex_wnt: public Mutex
{
public:
	virtual void			lock();
	virtual void			unlock();

	CRITICAL_SECTION		m;

	DR_DINLINE			Mutex_wnt();
	DR_DINLINE			~Mutex_wnt();
};


DR_DINLINE Mutex_wnt::Mutex_wnt()
{
	InitializeCriticalSection(&m);
}

DR_DINLINE Mutex_wnt::~Mutex_wnt()
{
	DeleteCriticalSection(&m);
}

DR_EXPORT_MET Mutex *Mutex::create()
{
	return new Mutex_wnt();
}

DR_EXPORT_MET void Mutex_wnt::lock()
{
	EnterCriticalSection(&m);
}

DR_EXPORT_MET void Mutex_wnt::unlock()
{
	LeaveCriticalSection(&m);
}


DR_NS_END
