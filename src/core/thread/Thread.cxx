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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <dr/Const.hxx>
#include <dr/Assert.hxx>
#include <dr/Alloc.hxx>
#include <dr/Thread.hxx>
#include <dr/MsgSync.hxx>

#include <dr/dev/Thread_impl.hxx>

DR_NS_BEGIN

DR_OBJECT_DEF(DR_NS_STR, Message, Object);
DR_OBJECT_IMPL_SIMPLE(Message);


DR_EXPORT_MET bool Message::doEval()
{
	DR_AssertInvalid();
	return false;
}

DR_EXPORT_MET void Message::eval()
{
	switch (type&~(ANSWERED|SYNCED)) {
	case Message::NOCALL:
		break;

	case Message::TERMINATE:
		Thread::current()->requestTerminate();
		break;

	case Message::CALLEVAL:
		if (!doEval())
			type = type&~SYNCED;
		break;

	case Message::VOIDFUNC:
	case Message::BOOLFUNC:
	case Message::INTFUNC:
	case Message::LONGFUNC:
	case Message::PTRFUNC:
		ret = ((void *(*)(void *))call)(arg);
		break;

	case Message::VOIDEMIT:
	case Message::BOOLEMIT:
	case Message::INTEMIT:
	case Message::LONGEMIT:
	case Message::PTREMIT:
		{
			Eslot1<void *, void *> s((Eimpl_g *)((Eimpl_g *)call)->ref());
			ret = s(arg);
		}
		break;

	default:
		DR_AssertInvalid();
	}
}

DR_EXPORT_MET Message::~Message()
{
	switch (type&~(ANSWERED|SYNCED)) {
	case Message::NOCALL:
		break;
	case Message::TERMINATE:
		break;
	case Message::CALLEVAL:
		break;
	case Message::VOIDFUNC:
	case Message::BOOLFUNC:
	case Message::INTFUNC:
	case Message::LONGFUNC:
	case Message::PTRFUNC:
		break;
	case Message::VOIDEMIT:
	case Message::BOOLEMIT:
	case Message::INTEMIT:
	case Message::LONGEMIT:
	case Message::PTREMIT:
		((Eimpl_g *)call)->unref();
		break;
	default:
		DR_AssertInvalid();
	}
}

Message *Message::createTerminate()
{
	Message *msg = new Message;
	msg->setTerminate();
	return msg;
}


DR_OBJECT_DEF(DR_NS_STR, Thread, Object);

DR_EXPORT_MET const Static *Thread::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(Const::string(DR_NS_STRP("Thread")), &comp_static, Super::getObjectStatic(), 0, NULL);
}

DR_EXPORT_MET Thread::Thread():
	DR_OBJECT_INIT(Thread, Object, ()),
	impl(NULL),
	manager(NULL),
	exit_value(NULL),
	thrown_value(NULL)
{
	terminating = false;
	waiting_for_msg = false;

	serial_level = 0;
	serial_pend = NULL;

	cur_msg = NULL;
	add_msg = NULL;
}

DR_EXPORT_MET Thread::Thread(const None &):
	DR_OBJECT_INIT(Thread, Object, ()),
	/*impl(NULL),*/
	manager(NULL),
	exit_value(NULL),
	thrown_value(NULL)
{
	terminating = false;
	waiting_for_msg = false;

	serial_level = 0;
	serial_pend = NULL;

	cur_msg = NULL;
	add_msg = NULL;
}

DR_EXPORT_MET Thread::~Thread()
{
	if (thrown_value)
		DR_FORGET(thrown_value);
	if (manager)
		manager->unref();
	if (serial_pend)
		Alloc::free(serial_pend);
	if (impl) {
		//impl->destroy();
		impl->mainDestroy();
	}
}

DR_EXPORT_MET void Thread::requestTerminate()
{
	terminating = true;
}

DR_EXPORT_MET bool Thread::isTerminating()
{
	return terminating;
}

DR_EXPORT_MET void *Thread::wait()
{
	void *saved_exit;
	impl->wait();
	saved_exit = exit_value;
	return saved_exit;
}

DR_EXPORT_MET void *Thread::waitUnref()
{
	void *saved_exit = wait();
	unref();
	return saved_exit;
}

DR_EXPORT_MTS bool Thread::setMsgSyncer(MsgSync *new_sync)
{
	if (!manager->threadXchg(new_sync))
		return false;
	manager->unref();
	manager = new_sync;
	return true;
}

#ifndef LONG_BIT
# define LONG_BIT (8*sizeof(long))
#endif

DR_EXPORT_MTS bool Thread::processMessages(int wait_answer)
{
	Thread *thr = current();

	for (;;) {
		Message *msg;

		if (wait_answer >= 0) {
			if ((thr->serial_pend[wait_answer/LONG_BIT]&(1<<(wait_answer%LONG_BIT))) != 0)
				return true;
		}
		
		if ((msg = thr->cur_msg) != NULL) {
			thr->cur_msg = msg->next;
			if ((msg->type&Message::ANSWERED) != 0) {
				if (msg->serial != (1UL<<(LONG_BIT-8))-1)
					thr->serial_pend[msg->serial/LONG_BIT] |= (1<<(msg->serial%LONG_BIT));
				msg->unref();
			}
			else {
				msg->eval();
				if ((msg->type&Message::SYNCED) != 0) {
					msg->type = msg->type|Message::ANSWERED;
					msg->creator->addMessage(msg);
				}
				else
					msg->unref();
			}
		}
		else if ((msg = (Message *)Atomic::xchg((void **)(void *)&thr->add_msg, NULL)) != NULL) {
			Message *o;
			for (o = NULL; ; ) {
				Message *n = msg->next;
				msg->next = o;
				if (!n)
					break;
				msg = n;
			}
			thr->cur_msg = msg;
		}
		else { /* wait for the next message */
			thr->manager->threadSleep(-1);
		}
	}
}

DR_EXPORT_MTS Message *Thread::waitAnyMessageTimeout(Sint64 timeout_ns)
{
	Thread *thr = current();
	Message *msg;

	for (;;) {
		if ((msg = thr->cur_msg) != NULL) {
			thr->cur_msg = msg->next;
			if ((msg->type&Message::ANSWERED) != 0) {
				thr->serial_pend[msg->serial/LONG_BIT] |= (1<<(msg->serial%LONG_BIT));
				msg->unref();
			}
			else {
				return msg;
			}
		}
		else if ((msg = (Message *)Atomic::xchg((void **)(void *)&thr->add_msg, NULL)) != NULL) {
			Message *o;
			for (o = NULL; ; ) {
				Message *n = msg->next;
				msg->next = o;
				if (!n)
					break;
				msg = n;
			}
			thr->cur_msg = msg;
		}
		else { /* wait for the next message */
			if (thr->manager->threadSleep(timeout_ns) == 0)
				return NULL;
		}
	}
}

DR_EXPORT_MTS void Thread::processOneMessage()
{
	Thread *thr = current();

	Message *msg;

	for (;;) {
		if ((msg = thr->cur_msg) != NULL) {
			thr->cur_msg = msg->next;
			if ((msg->type&Message::ANSWERED) != 0) {
				thr->serial_pend[msg->serial/LONG_BIT] |= (1<<(msg->serial%LONG_BIT));
				msg->unref();
			}
			else {
				msg->eval();
				if ((msg->type&Message::SYNCED) != 0) {
					msg->type = msg->type|Message::ANSWERED;
					msg->creator->addMessage(msg);
				}
				else
					msg->unref();
			}
			break;
		}
		else if ((msg = (Message *)Atomic::xchg((void **)(void *)&thr->add_msg, NULL)) != NULL) {
			Message *o;
			for (o = NULL; ; ) {
				Message *n = msg->next;
				msg->next = o;
				if (!n)
					break;
				msg = n;
			}
			thr->cur_msg = msg;
		}
		else { /* wait for the next message */
			thr->manager->threadSleep(-1);
		}
	}
}

void Thread::addMessage(Message *msg)
{
	return addMessage_lr((Message *)msg->ref());
}

void Thread::addMessage_lr(Message *msg)
{
retry:
	Message *n = add_msg;
	msg->next = n;
	if (!Atomic::cmpxchg((void **)(void *)&add_msg, n, msg))
		goto retry;

	if (waiting_for_msg) {
		waiting_for_msg = false;
		manager->threadWake();
	}
}

void Thread::addAnswer(int serial)
{
	Message *msg = new Message;
	msg->resetAnswer(serial);
	addMessage_lr(msg);
}

DR_EXPORT_MTS int Thread::createMessageSerial()
{
	unsigned serial;
	Thread *thr = current();

	serial = thr->serial_level++;

	if (serial%LONG_BIT == 0)
		thr->serial_pend = (unsigned long *)Alloc::regrow(thr->serial_pend, (serial+LONG_BIT)/8);
	thr->serial_pend[serial/LONG_BIT] &= ~(1UL<<(serial%LONG_BIT));
	return serial;
}


DR_NS_END
