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

#ifndef dr__Throw__hxx__
# define dr__Throw__hxx__

#include <dr/Object.hxx>
#include <dr/Ref.hxx>
#include <dr/String.hxx>

DR_NS_BEGIN


#define DR_THROW_DECL_SIMPLE(objname, ancestor) \
	DR_OBJECT_DECL_SIMPLE(objname, ancestor); \
public: \
	void				throwThis() \


#define DR_THROW_DECL(objname, ancestor) \
	DR_OBJECT_DECL(objname, ancestor); \
public: \
	DR_SUPER_REDIR0C(Throw *, getCause) \
public: \
	void				throwThis() \


#define DR_THROW_DEF(NS_STR, objname, ancestor) \
	DR_OBJECT_DEF(NS_STR, objname, ancestor); \
	DR_EXPORT_MET void objname::throwThis() \
	{ \
		throw this; \
	} \


#define DR_THROW_IMPL_SIMPLE(objname) \
	DR_OBJECT_IMPL_SIMPLE(objname)


#define DR_THROW_INIT(objname, ancestor, args) \
	DR_OBJECT_INIT(objname, ancestor, args) \


/**
 * Throw interface
 */
class DR_PUB Throw: public Iface
{
	DR_IFACE_DECL(Throw);

public:
	virtual Throw *			getCause() const = 0;

protected:
	DR_RINLINE virtual		~Throw()				{}

public:
	virtual void			throwThis() = 0;
};


class DR_PUB SysThrow: public Throw
{
	DR_IFACE_DECL(SysThrow);

public:
	virtual int			getSyserr() = 0;
};


/**
 * Throwable object
 *
 * Is ancestor of all exceptions and errors
 */
class DR_PUB Throwable: public Object, public Throw
{
	DR_OBJECT_DECL(Throwable, Object);
	DR_REDIR_CMP();
	DR_REDIR_SERIALIZE();

public:
	virtual Throw *			getCause() const			{ return cause.getDoref(); }
	virtual String			stringify() const;

public:
	virtual void			throwThis();

public:
	/* constructor */		Throwable();
	/* constructor */		Throwable(Throw *cause);

public:
	static bool			throw_active();
	static bool			throw_overactive();
	static void			throw_move(Throwable *obj);
	static void			throw_ref(Throwable *obj);
	static void			throw_activate();
	static void			throw_inactivate();
	static void			throw_safe_destroy(Throwable *obj);
	static void			throw_inactivate_destroy(Throwable *obj);

protected:
	virtual				~Throwable();

protected:
	Ref<Throw>			cause;
};

#define DR_THROWNEW(obj_def)						\
	DR_NSP(Throwable)::throw_move(new obj_def)

#define DR_THROWMOVE(obj_old)						\
	DR_NSP(Throwable)::throw_move(obj_old)

#define DR_THROWREF(obj_old)						\
	DR_NSP(Throwable)::throw_ref(obj_old)

#define DR_RETHROW(ex)							\
	do {								\
		ex.getAndNull();					\
		DR_NSP(Throwable)::throw_activate();			\
		throw;							\
	} while(0)

#define DR_RETHROWANY							\
	do {								\
		DR_NSP(Throwable)::throw_activate();			\
		throw;							\
	} while (0)

#define DR_TRY								\
	try { {

#define DR_ENDTRY							\
	} }

#define DR_CATCH(Cls, var)						\
	} } catch (Cls *var##_) { {					\
		DR_NSP(Throwable)::throw_inactivate();			\
		Ref<Cls> var(var##_, false);

#define DR_CATCHANY							\
	} } catch (...) { {						\
		DR_NSP(Throwable)::throw_inactivate();

#define DR_FORGET(ex)							\
	DR_NSP(Throwable)::throw_safe_destroy(ex)


#define DR_THOLD(Cls, var)						\
	} catch (Cls *var##_) {						\
		DR_NSP(Throwable)::throw_inactivate();			\
		Ref<Cls> var(var##_, false);				\
		try {

#define DR_THOLDGO(var)							\
		}							\
		catch (DR_NSP(Throwable) *var##_hold_2) {		\
			DR_FORGET(var##_hold_2);			\
		}							\
		throw;							\
		{

#define DR_THOLDEND(var)						\
		DR_THOLDGO(var)						\
		DR_ENDTRY

#define DR_XSAFE(code)							\
	try {								\
		code;							\
	}								\
	catch (Throwable *xsafe_ex) {					\
		DR_NSP(Throwable::throw_inactivate_destroy(xsafe_ex));	\
	}

#define DR_XSAFETYPE(code, extype)							\
	try {								\
		code;							\
	}								\
	catch (extype *dr_xst_ex) {					\
		DR_NSP(Throwable::throw_inactivate_destroy(dr_xst_ex));	\
	}

DR_NS_END

#endif
