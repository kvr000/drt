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

#ifndef dr__Thread__hxx__
# define dr__Thread__hxx__

#include <dr/base.hxx>
#include <dr/cmp.hxx>
#include <dr/enumops.hxx>
#include <dr/emits.hxx>
#include <dr/List.hxx>

DR_NS_BEGIN


class Thread;
class Thread_impl;
class MsgSync;

/**
 * message carrier
 */
class DR_PUB Message: public Object
{
	DR_OBJECT_DECL_SIMPLE(Message, Object);

protected:
	Message *			next;

public:
	enum Type
	{
		UNSET = 0,				/* invalid */
		TERMINATE,				/* finish thread */

		NOCALL,					/* no call */
		CALLEVAL,				/* call message method */

		VOIDFUNC,				/* call ptr-function void f(void *); */
		BOOLFUNC,				/* call ptr-function bool f(void *); */
		INTFUNC,				/* call ptr-function int f(void *); */
		LONGFUNC,				/* call ptr-function long f(void *); */
		PTRFUNC,				/* call ptr-function void *f(void *); */

		VOIDEMIT,				/* call ptr-slot void f(void *); */
		BOOLEMIT,				/* call ptr-slot bool f(void *); */
		INTEMIT,				/* call ptr-slot int f(void *); */
		LONGEMIT,				/* call ptr-slot long f(void *); */
		PTREMIT,				/* call ptr-slot void *f(void *); */

		SYNCED = 0x20,				/* synchronised between threads (need answer) */
		ANSWERED = 0x40,			/* answer to message */
	};

	enum Type			type:8;		/* type of message */
	unsigned long			serial:(sizeof(void*)*8-8);	/* serial (for response of sync-ed messages) */
	Thread *			creator;	/* creator thread */

	void *				call;		/* function/slot address */
	union {
		void *			arg;		/* argument */
		void *			ret;		/* return */
	};

public:
	DR_MINLINE			Message()				{ type = UNSET; }
	virtual				~Message();
	virtual bool			doEval();

	void				eval();

public:
	DR_RINLINE void			setNone()				{ type = NOCALL; }
	DR_RINLINE void			setTerminate()				{ type = TERMINATE; }
	DR_RINLINE void			setEval()				{ type = CALLEVAL; }
	DR_RINLINE void			setFunc(void (*f)())			{ type = VOIDFUNC; call = (void *)f; }
	DR_RINLINE void			setFunc(void (*f)(void *), void *arg_)	{ type = VOIDFUNC; call = (void *)f; arg = arg_; }
	DR_RINLINE void			setFunc(bool (*f)())			{ type = BOOLFUNC; call = (void *)f; }
	DR_RINLINE void			setFunc(bool (*f)(void *), void *arg_)	{ type = BOOLFUNC; call = (void *)f; arg = arg_; }
	DR_RINLINE void			setFunc(int (*f)())			{ type = INTFUNC; call = (void *)f; }
	DR_RINLINE void			setFunc(int (*f)(void *), void *arg_)	{ type = INTFUNC; call = (void *)f; arg = arg_; }
	DR_RINLINE void			setFunc(long (*f)())			{ type = LONGFUNC; call = (void *)f; }
	DR_RINLINE void			setFunc(long (*f)(void *), void *arg_)	{ type = LONGFUNC; call = (void *)f; arg = arg_; }
	DR_RINLINE void			setFunc(void *(*f)())			{ type = PTRFUNC; call = (void *)f; }
	DR_RINLINE void			setFunc(void *(*f)(void *), void *arg_)	{ type = PTRFUNC; call = (void *)f; arg = arg_; }
	DR_RINLINE void			setSlot(const Eslot0<void> &s)	{ type = VOIDEMIT; call = s.toPtrRef(); }
	DR_RINLINE void			setSlot(const Eslot1<void, void *> &s, void *arg_) { type = VOIDEMIT; call = s.toPtrRef(); arg = arg_; }
	DR_RINLINE void			setSlot(const Eslot0<bool> &s)	{ type = BOOLEMIT; call = s.toPtrRef(); }
	DR_RINLINE void			setSlot(const Eslot1<bool, void *> &s, void *arg_) { type = BOOLEMIT; call = s.toPtrRef(); arg = arg_; }
	DR_RINLINE void			setSlot(const Eslot0<int> &s)		{ type = INTEMIT; call = s.toPtrRef(); }
	DR_RINLINE void			setSlot(const Eslot1<int, void *> &s, void *arg_) { type = INTEMIT; call = s.toPtrRef(); arg = arg_; }
	DR_RINLINE void			setSlot(const Eslot0<long> &s)	{ type = LONGEMIT; call = s.toPtrRef(); }
	DR_RINLINE void			setSlot(const Eslot1<long, void *> &s, void *arg_) { type = LONGEMIT; call = s.toPtrRef(); arg = arg_; }
	DR_RINLINE void			setSlot(const Eslot0<void *> &s)	{ type = PTREMIT; call = s.toPtrRef(); }
	DR_RINLINE void			setSlot(const Eslot1<void *, void *> &s, void *arg_) { type = PTREMIT; call = s.toPtrRef(); arg = arg_; }

	DR_RINLINE void			setSynced()				{ type = (Type)(type|SYNCED); }

	DR_RINLINE void			setAnswer(int serial_)			{ type = (Type)(type|ANSWERED); serial = serial_; }
	DR_RINLINE void			resetAnswer(int serial_)		{ type = Type(NOCALL|ANSWERED); serial = serial_; }

	static Message *		createTerminate();

protected:
	friend class Thread;
};

DR_ENUM_OPS(Message::Type);


/**
 * thread user object
 */
class DR_PUB Thread: public Object
{
	DR_OBJECT_DECL_SIMPLE(Thread, Object);

public:
	/**
	 * returns the current Thread
	 *
	 * @note this function doesn't increment the object count
	 */
	static Thread *			current();

public:
	static void *			getMsgIface(const String &iface);
	bool				setMsgSyncer(MsgSync *new_sync);

public:
	virtual void			start();
	virtual void			startUnref();
	virtual void			startThrowUnref();

protected:
	virtual void *			run() = 0;
	virtual void			requestTerminate();

public:
	/* thread local storage */
	static long			createSpecific();
	static void			freeSpecific(long key);
	static void *			getSpecific(long key);
	static void			setSpecific(long key, void *val);

	/* synchronization */
	virtual void *			wait();
	virtual void *			waitUnref();

	virtual bool			isTerminating();

	/* messages */
	void				addMessage(Message *msg);
	void				addMessage_lr(Message *msg);
	void				addAnswer(int serial);
	static void			postMessage(Thread *target, Message *msg);
	static void			sendMessage(Thread *target, Message *msg);
	static void			immMessage(Thread *target, Message *msg);
	static int			createMessageSerial();

	static Message *		waitAnyMessage();
	static Message *		waitAnyMessageTimeout(Sint64 timeout_ns);

	static bool			processMessages(int wait_answer = -1);
	static void			processOneMessage();

protected:
	DR_CONSTRUCT			Thread();
	DR_CONSTRUCT			Thread(const None &);
	virtual				~Thread();

public:
	Thread_impl *			impl;
	MsgSync *			manager;

protected:
	void *				exit_value;
	Throwable *			thrown_value;
	bool				terminating;

	bool volatile			waiting_for_msg;

	unsigned			serial_level;
	unsigned long *			serial_pend;

	Message *			cur_msg;		/* currently processed queue */
	Message * volatile		add_msg;		/* added messages */

public:
	DR_RINLINE bool			msg_setWaiting();
	DR_RINLINE void			msg_unsetWaiting();

private:
	friend class Thread_impl;
	friend class Message;
};


DR_NS_END

#endif
