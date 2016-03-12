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

#include <dr/x_kw.hxx>

#include <dr/Const.hxx>
#include <dr/SysError.hxx>
#include <dr/MsgSync.hxx>
#include <dr/Thread.msg.hxx>

#include "MsgSync_wnt.hxx"

DR_NS_BEGIN


DR_OBJECT_DEF(DR_NS_STR, MsgSync_wnt, MsgSync);

//DR_OBJECT_IMPL_SIMPLE(MsgSync_unix);
DR_EXPORT_MET const Static *MsgSync_wnt::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(Const::string(DR_NS_STRP("MsgSync_wnt")), &comp_static, Super::getObjectStatic(), (char *)(void *)(MsgSync_WntMsg *)this-(char *)(void *)this, MsgSync_WntMsg::getIfaceStatic(), 0, NULL);
}


/* constructor/destructor */
MsgSync_wnt::MsgSync_wnt(Thread *thread_):
	DR_OBJECT_INIT(MsgSync_wnt, MsgSync, (thread_)),
	msg_listener(Null())
{
	DR_LOG1(("%s: creating MsgSync_wnt: %p (thread %p)\n", DR_FUNCTION, this, thread_));

	InitializeCriticalSection(&sync_mutex);
	sync_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

MsgSync_wnt::~MsgSync_wnt()
{
	DR_LOG1(("%s: destroying MsgSync_wnt: %p\n", DR_FUNCTION, this));
	CloseHandle(sync_event);
	DeleteCriticalSection(&sync_mutex);
}

void MsgSync_wnt::registerWntMsgReceiver(const Eslot0<void> &notif)
{
	msg_listener = notif;
}

void MsgSync_wnt::unregisterWntMsgReceiver()
{
	msg_listener.setNull();
}

bool MsgSync_wnt::threadXchg(MsgSync *new_sync)
{
	return true;
}

/* thread sync interface */
int MsgSync_wnt::threadSleep(Sint64 timeout_ns)
{
	int got_message = 1;
	EnterCriticalSection(&sync_mutex);
	method = 0;
	if (thread->msg_setWaiting()) {
		if (timeout_ns >= (Sint64)0x7fffffff*1000000)
			timeout_ns = 0x7fffffff*1000000;
		DR_LOG1(("%s: %p: selecting method %d\n", DR_FUNCTION, this, method));
		LeaveCriticalSection(&sync_mutex);
		if (msg_listener.isNull()) {
			switch (WaitForSingleObject(sync_event, timeout_ns >= 0 ? (timeout_ns+999999)/1000000 : INFINITE)) {
			case WAIT_FAILED:
				xthrownew(SysError(GetLastError()));
				break;

			case WAIT_TIMEOUT:
				got_message = 0;
				break;

			case WAIT_OBJECT_0:
				break;

			}
		}
		else {
			int ret = MsgWaitForMultipleObjects(1, &sync_event, FALSE, timeout_ns >= 0 ? (timeout_ns+999999)/1000000 : INFINITE, QS_ALLEVENTS);
			switch (ret) {
			case WAIT_FAILED:
				xthrownew(SysError(GetLastError()));
				break;

			case WAIT_TIMEOUT:
				got_message = 0;
				break;

			case WAIT_OBJECT_0:
				break;

			case WAIT_OBJECT_0+1:
				ERef<Message> msg(new Message);
				msg->setSlot(msg_listener);
				thread->addMessage_lr(msg.getAndNull());
				break;
			}
		}
		DR_LOG1(("%s: being woken\n", DR_FUNCTION));
	}
	else {
		LeaveCriticalSection(&sync_mutex);
	}
	return 0;
}

void MsgSync_wnt::threadWake()
{
	EnterCriticalSection(&sync_mutex);
	thread->msg_unsetWaiting();
	switch (method) {
	case 0:
		SetEvent(sync_event);
		break;
	}
	LeaveCriticalSection(&sync_mutex);
}


MsgSync *MsgSync::createDefault(Thread *thread)
{
	return new MsgSync_wnt(thread);
}


DR_NS_END
