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

#include <dr/Const.hxx>
#include <dr/SysError.hxx>
#include <dr/MsgSync.hxx>
#include <dr/Thread.msg.hxx>

#include "MsgSync_symbian.hxx"

DR_NS_BEGIN


DR_OBJECT_DEF(DR_NS_STR, MsgSync_symbian, MsgSync);

//DR_OBJECT_IMPL_SIMPLE(MsgSync_unix);
DR_EXPORT_MET const Static *MsgSync_symbian::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(&comp_static, DR_NS_STRP("MsgSync_symbian"), Super::getObjectStatic(), 0, NULL);
}


/* constructor/destructor */
MsgSync_symbian::MsgSync_symbian(Thread *thread_):
	DR_OBJECT_INIT(MsgSync_symbian, MsgSync, (thread_))
{
	TInt err;
	DR_LOG1(("%s: creating MsgSync_symbian: %p (thread %p)\n", DR_FUNCTION, this, thread_));

	if ((err = sync_mutex.CreateLocal()) != 0) {
		DR_THROWNEW(SysError(err));
	}
}

MsgSync_symbian::~MsgSync_symbian()
{
	DR_LOG1(("%s: destroying MsgSync_symbian: %p\n", DR_FUNCTION, this));
}

bool MsgSync_symbian::threadXchg(MsgSync *new_sync)
{
	return true;
}

/* thread sync interface */
void MsgSync_symbian::threadSleep()
{
	sync_mutex.Wait();
	method = 0;
	if (thread->msg_setWaiting()) {
		DR_LOG1(("%s: %p: selecting method %d\n", DR_FUNCTION, this, method));
		sync_status = KRequestPending;
		sync_mutex.Signal();
		User::WaitForRequest(sync_status);
		DR_LOG1(("%s: being woken\n", DR_FUNCTION));
	}
}

void MsgSync_symbian::threadWake()
{
	sync_mutex.Wait();
	thread->msg_unsetWaiting();
	switch (method) {
	case 0:
		if (sync_status == KRequestPending) {
			TRequestStatus *st = &sync_status;
			User::RequestComplete(st, KErrNone);
		}
		break;
	}
	sync_mutex.Signal();
}


MsgSync *MsgSync::createDefault(Thread *thread)
{
	return new MsgSync_symbian(thread);
}


DR_NS_END
