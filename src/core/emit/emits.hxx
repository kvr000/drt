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

/* This file is generated from ./emit/genemits.pl... */
#ifndef __emits__hxx__
# define __emits__hxx__

#include <dr/Alloc.hxx>
#include <dr/Shared.hxx>

DR_NS_BEGIN


template <typename TL, typename TR>
DR_MINLINE void EvassignR(TL *l, TR *r)
{
	*l = *r;
}
template <typename TL>
DR_MINLINE void EvassignR(TL *, void *)
{
}

template <typename TL, typename TR>
DR_MINLINE void Evassign(TL *l, TR *r)
{
	EvassignR(l, r);
}
template <typename TR>
DR_MINLINE void Evassign(void *l, TR *r)
{
}

template <typename TL, typename TR>
DR_MINLINE void Epassign(TL *l, const TR &r)
{
	*l = r;
}
template <typename TR>
DR_MINLINE void Epassign(void *, const TR &)
{
}

DR_COREFUN void *Ealloc(size_t size);
DR_COREFUN void Efree(void *m);

class DR_PUB Eimpl_g: public Shared
{
public:
	DR_MINLINE virtual		~Eimpl_g() {}

	bool (*caller)();

	DR_MINLINE			Eimpl_g(bool (*caller_)()): caller(caller_) {}
};

class DR_PUB Eslot_g
{
public:
	DR_MINLINE			Eslot_g(const Null &): impl(NULL) {}
	DR_MINLINE			Eslot_g(Eimpl_g *r): impl(r) {}
	/* constructor */		Eslot_g(const Eslot_g &r);
	/* constructor */		Eslot_g &operator=(const Eslot_g &r);
	/* destructor */		~Eslot_g();

	DR_MINLINE bool			isNull() const { return impl == NULL; }
	DR_MINLINE void			setNull() { if (impl) impl->unref(); impl = NULL; }

	Eimpl_g *			impl;

	friend class Enode_g;
	friend class Emitter_g;
};

class DR_PUB Enode_g
{
public:
	Enode_g	*			prev, *next;
	Eslot_g				slot;
	DR_MINLINE			Enode_g(const Eslot_g &r): slot(r) {}

	DR_MINLINE static void *	operator new(size_t s) { return Ealloc(s); }
	DR_MINLINE static void		operator delete(void *m) { Efree(m); }

	friend class Emitter_g;
};

class DR_PUB Emitter_g
{
private:
	Enode_g	*			firstv;

	DR_MINLINE bool			getclearVret() { bool o = (SintPtr)firstv&1; firstv = (Enode_g *)((SintPtr)firstv&~(SintPtr)1); return o; }
	DR_MINLINE void			setVret(bool v) { firstv = (Enode_g *)((SintPtr)firstv|(SintPtr)v); }


protected:
	DR_MINLINE Enode_g *		first() { return (Enode_g *)((SintPtr)firstv&~(SintPtr)1); }
	DR_MINLINE bool			vret() { return (SintPtr)firstv&1; }

protected:
	DR_RINLINE			Emitter_g(bool defvret): firstv((Enode_g *)((SintPtr)0|defvret)) {}
	/* destructor */		~Emitter_g();

	Enode_g *			append_g(const Eslot_g &s);
	Enode_g *			insert_g(const Eslot_g &s);
	void				remove_g(Enode_g *node);

private:
	/* disallowed */		Emitter_g(const Emitter_g &);
};

class DR_PUB EimplF_g: public Eimpl_g
{
public:
	void				(*func)();
	bool				vret;

	DR_MINLINE			EimplF_g(bool (*caller_)(), void (*func_)(), bool vret_): Eimpl_g(caller_), func(func_), vret(vret_) {}
};

class DR_PUB EimplM_g: public Eimpl_g
{
public:
	void *				obj;
	void				(Shared::*meth)();
	bool				vret;

	DR_MINLINE			EimplM_g(bool (*caller_)(), void *obj_, void (Shared::*meth_)(), bool vret_): Eimpl_g(caller_), obj(obj_), meth(meth_), vret(vret_) {}
};

class DR_PUB EimplE_g: public Eimpl_g
{
public:
	Emitter_g *			em;

	DR_MINLINE			EimplE_g(bool (*caller_)(), Emitter_g *em_): Eimpl_g(caller_), em(em_) {}
};

template <typename C>
class EimplC_g: public Eimpl_g
{
public:
	C c;

	DR_MINLINE			EimplC_g(bool (*caller_)(), typename typeref<C>::cref c_): Eimpl_g(caller_), c(c_) {}
};

class DR_PUB EimplS_g: public Eimpl_g
{
public:
	Eslot_g s;
	
	DR_MINLINE			EimplS_g(bool (*caller_)(), const Eslot_g &s_): Eimpl_g(caller_), s(s_) {}
};

template <typename C>
class EimplSC_g: public EimplS_g
{
public:
	C c;
	
	DR_MINLINE			EimplSC_g(bool (*caller_)(), const Eslot_g &s_, typename typeref<C>::cref c_): EimplS_g(caller_, s_), c(c_) {}
};

template <typename E>
class EimplL_g: public Eimpl_g
{
public:
	E e;

	DR_MINLINE			EimplL_g(bool (*caller_)(), bool dvret): Eimpl_g(caller_), e(dvret) {}
};


// used only for type-safety pointers (implementation never used) */
template <typename R>
class Enode0: public Enode_g
{
	/* constructor */		Enode0(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R>
class Eslot0;

class Emitter0_g;

template <typename R, bool logtype>
class EmitterBase0;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1>
class Enode1: public Enode_g
{
	/* constructor */		Enode1(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1>
class Eslot1;

class Emitter1_g;

template <typename R, typename A1, bool logtype>
class EmitterBase1;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2>
class Enode2: public Enode_g
{
	/* constructor */		Enode2(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2>
class Eslot2;

class Emitter2_g;

template <typename R, typename A1, typename A2, bool logtype>
class EmitterBase2;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2, typename A3>
class Enode3: public Enode_g
{
	/* constructor */		Enode3(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2, typename A3>
class Eslot3;

class Emitter3_g;

template <typename R, typename A1, typename A2, typename A3, bool logtype>
class EmitterBase3;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2, typename A3, typename A4>
class Enode4: public Enode_g
{
	/* constructor */		Enode4(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class Eslot4;

class Emitter4_g;

template <typename R, typename A1, typename A2, typename A3, typename A4, bool logtype>
class EmitterBase4;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class Enode5: public Enode_g
{
	/* constructor */		Enode5(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class Eslot5;

class Emitter5_g;

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, bool logtype>
class EmitterBase5;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Enode6: public Enode_g
{
	/* constructor */		Enode6(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Eslot6;

class Emitter6_g;

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, bool logtype>
class EmitterBase6;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class Enode7: public Enode_g
{
	/* constructor */		Enode7(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class Eslot7;

class Emitter7_g;

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, bool logtype>
class EmitterBase7;


// used only for type-safety pointers (implementation never used) */
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class Enode8: public Enode_g
{
	/* constructor */		Enode8(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class Eslot8;

class Emitter8_g;

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, bool logtype>
class EmitterBase8;


class Eslot0_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot0_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot0_g(const Eslot0_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot0_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r) { return ((bool (*)(void *me, void *r))impl->caller)(impl, r); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R>
class Eslot0: public Eslot0_g
{
	DR_RINLINE bool			vcall(R *r) { return pcall(r); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r) { Epassign(r, ((typename typeref<R>::nvsimp (*)())me->func)()); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)())me->meth))()); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r) { return ((EmitterBase0<R, logtype> *)me->em)->emitC(); } };

	template <typename AS>
	struct cAS
	{
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_) { typename typeref<R>::nvtyp r; return ((Eslot0_g *)&me->s)->pcall(&r); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr) { typename typeref<R>::nvtyp r; bool o = ((Eslot0_g *)&me->s)->pcall(&r); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr) { typename typeref<R>::typ r; bool o = ((Eslot0_g *)&me->s)->pcall(&r); *nr = (NR)r; return o; } };
	template <typename NR>
	struct cFR { static bool callme(EimplS_g *me, NR *nr) { typename typeref<R>::nvtyp r; bool o = ((Eslot0<R> *)&me->s)->vcall(&r); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r) { ((Eslot0_g *)&me->s)->pcall(r); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r) { ((Eslot0_g *)&me->s)->pcall(r); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r) { ((Eslot0_g *)&me->s)->pcall(r); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r) { ((Eslot0_g *)&me->s)->pcall(r); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r) { ((Eslot0_g *)&me->s)->pcall(r); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r) { ((Eslot0_g *)&me->s)->pcall(r); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r) { return me->c(((Eslot0_g *)&me->s)->pcall(r), *r); } };

public:
	DR_RINLINE			Eslot0(const Eslot0 &r): Eslot0_g(r) {}
	DR_RINLINE			Eslot0(Eimpl_g *impl_): Eslot0_g(impl_) {}
	DR_RINLINE			Eslot0(const Null &): Eslot0_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot0(R (*func)(), bool vret = true): Eslot0_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot0(O *obj, R (O::*meth)(), bool vret = true): Eslot0_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot0(const EmitterBase0<R, logtype> *em): Eslot0_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()() const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r))impl->caller)(impl, pr); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AI>
	DR_RINLINE Eslot1<R, AI> aAdd() const { return *reinterpret_cast<const Eslot1<R, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot0<NR> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot0<NR> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot0<NR> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR>
	DR_RINLINE Eslot0<NR> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR>::callme, *this); }
	DR_RINLINE Eslot0<R> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot0<R> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot0<R> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot0<R> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot0<R> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot0<R> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot0<R> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot1<R, AA1> aAdd1() { return *reinterpret_cast<const Eslot1<R, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot2<R, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot2<R, AA1, AA2> *>(this); }
	template <typename AA1, typename AA2, typename AA3>
	DR_RINLINE Eslot3<R, AA1, AA2, AA3> aAdd3() { return *reinterpret_cast<const Eslot3<R, AA1, AA2, AA3> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4>
	DR_RINLINE Eslot4<R, AA1, AA2, AA3, AA4> aAdd4() { return *reinterpret_cast<const Eslot4<R, AA1, AA2, AA3, AA4> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5>
	DR_RINLINE Eslot5<R, AA1, AA2, AA3, AA4, AA5> aAdd5() { return *reinterpret_cast<const Eslot5<R, AA1, AA2, AA3, AA4, AA5> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5, typename AA6>
	DR_RINLINE Eslot6<R, AA1, AA2, AA3, AA4, AA5, AA6> aAdd6() { return *reinterpret_cast<const Eslot6<R, AA1, AA2, AA3, AA4, AA5, AA6> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5, typename AA6, typename AA7>
	DR_RINLINE Eslot7<R, AA1, AA2, AA3, AA4, AA5, AA6, AA7> aAdd7() { return *reinterpret_cast<const Eslot7<R, AA1, AA2, AA3, AA4, AA5, AA6, AA7> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5, typename AA6, typename AA7, typename AA8>
	DR_RINLINE Eslot8<R, AA1, AA2, AA3, AA4, AA5, AA6, AA7, AA8> aAdd8() { return *reinterpret_cast<const Eslot8<R, AA1, AA2, AA3, AA4, AA5, AA6, AA7, AA8> *>(this); }
};

template <typename R>
DR_RINLINE Eslot0<R> Eslot(R (*func)(), bool vret = true)
{
	return Eslot0<R>(func, vret);
}
template <typename O, typename R>
DR_RINLINE Eslot0<R> Eslot(O *o, R (O::*meth)(), bool vret = true)
{
	return Eslot0<R>(o, meth, vret);
}
template <typename R, bool logtype>
DR_RINLINE Eslot0<R>Eslot(EmitterBase0<R, logtype> *e)
{
	return Eslot0<R>(e);
}


class Eslot1_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot1_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot1_g(const Eslot1_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot1_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1) { return ((bool (*)(void *me, void *r, const void *a1))impl->caller)(impl, r, a1); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1>
class Eslot1: public Eslot1_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1) { return pcall(r, &a1); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1))me->func)(a1)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1))me->meth))(a1)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1) { return ((EmitterBase1<R, A1, logtype> *)me->em)->emitC(a1); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r) { A1 a1(me->c); return ((Eslot1_g *)&me->s)->pcall(r, &a1); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1) { typename typeref<R>::nvtyp r; return ((Eslot1_g *)&me->s)->pcall(&r, &a1); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1) { typename typeref<R>::nvtyp r; bool o = ((Eslot1_g *)&me->s)->pcall(&r, &a1); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1) { typename typeref<R>::typ r; bool o = ((Eslot1_g *)&me->s)->pcall(&r, &a1); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1) { typename typeref<R>::nvtyp r; bool o = ((Eslot1<R, A1> *)&me->s)->vcall(&r, na1); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1) { ((Eslot1_g *)&me->s)->pcall(r, &a1); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1) { ((Eslot1_g *)&me->s)->pcall(r, &a1); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1) { ((Eslot1_g *)&me->s)->pcall(r, &a1); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1) { ((Eslot1_g *)&me->s)->pcall(r, &a1); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1) { ((Eslot1_g *)&me->s)->pcall(r, &a1); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1) { ((Eslot1_g *)&me->s)->pcall(r, &a1); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1) { return me->c(((Eslot1_g *)&me->s)->pcall(r, &a1), *r); } };

public:
	DR_RINLINE			Eslot1(const Eslot1 &r): Eslot1_g(r) {}
	DR_RINLINE			Eslot1(Eimpl_g *impl_): Eslot1_g(impl_) {}
	DR_RINLINE			Eslot1(const Null &): Eslot1_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot1(R (*func)(A1 a1), bool vret = true): Eslot1_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot1(O *obj, R (O::*meth)(A1 a1), bool vret = true): Eslot1_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot1(const EmitterBase1<R, A1, logtype> *em): Eslot1_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1))impl->caller)(impl, pr, a1); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot0<R> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot2<R, A1, AI> aAdd() const { return *reinterpret_cast<const Eslot2<R, A1, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot1<NR, A1> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot1<NR, A1> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot1<NR, A1> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1>
	DR_RINLINE Eslot1<NR, NA1> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1>::callme, *this); }
	DR_RINLINE Eslot1<R, A1> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot1<R, A1> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot1<R, A1> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot1<R, A1> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot1<R, A1> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot1<R, A1> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot1<R, A1> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot2<R, A1, AA1> aAdd1() { return *reinterpret_cast<const Eslot2<R, A1, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot3<R, A1, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot3<R, A1, AA1, AA2> *>(this); }
	template <typename AA1, typename AA2, typename AA3>
	DR_RINLINE Eslot4<R, A1, AA1, AA2, AA3> aAdd3() { return *reinterpret_cast<const Eslot4<R, A1, AA1, AA2, AA3> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4>
	DR_RINLINE Eslot5<R, A1, AA1, AA2, AA3, AA4> aAdd4() { return *reinterpret_cast<const Eslot5<R, A1, AA1, AA2, AA3, AA4> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5>
	DR_RINLINE Eslot6<R, A1, AA1, AA2, AA3, AA4, AA5> aAdd5() { return *reinterpret_cast<const Eslot6<R, A1, AA1, AA2, AA3, AA4, AA5> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5, typename AA6>
	DR_RINLINE Eslot7<R, A1, AA1, AA2, AA3, AA4, AA5, AA6> aAdd6() { return *reinterpret_cast<const Eslot7<R, A1, AA1, AA2, AA3, AA4, AA5, AA6> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5, typename AA6, typename AA7>
	DR_RINLINE Eslot8<R, A1, AA1, AA2, AA3, AA4, AA5, AA6, AA7> aAdd7() { return *reinterpret_cast<const Eslot8<R, A1, AA1, AA2, AA3, AA4, AA5, AA6, AA7> *>(this); }
};

template <typename R, typename A1>
DR_RINLINE Eslot1<R, A1> Eslot(R (*func)(A1 a1), bool vret = true)
{
	return Eslot1<R, A1>(func, vret);
}
template <typename O, typename R, typename A1>
DR_RINLINE Eslot1<R, A1> Eslot(O *o, R (O::*meth)(A1 a1), bool vret = true)
{
	return Eslot1<R, A1>(o, meth, vret);
}
template <typename R, typename A1, bool logtype>
DR_RINLINE Eslot1<R, A1>Eslot(EmitterBase1<R, A1, logtype> *e)
{
	return Eslot1<R, A1>(e);
}


class Eslot2_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot2_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot2_g(const Eslot2_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot2_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2))impl->caller)(impl, r, a1, a2); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2>
class Eslot2: public Eslot2_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { return pcall(r, &a1, &a2); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2))me->func)(a1, a2)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2))me->meth))(a1, a2)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { return ((EmitterBase2<R, A1, A2, logtype> *)me->em)->emitC(a1, a2); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2) { A1 a1(me->c); return ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1) { A2 a2(me->c); return ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { typename typeref<R>::nvtyp r; return ((Eslot2_g *)&me->s)->pcall(&r, &a1, &a2); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { typename typeref<R>::nvtyp r; bool o = ((Eslot2_g *)&me->s)->pcall(&r, &a1, &a2); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { typename typeref<R>::typ r; bool o = ((Eslot2_g *)&me->s)->pcall(&r, &a1, &a2); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2) { typename typeref<R>::nvtyp r; bool o = ((Eslot2<R, A1, A2> *)&me->s)->vcall(&r, na1, na2); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { ((Eslot2_g *)&me->s)->pcall(r, &a1, &a2); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { return me->c(((Eslot2_g *)&me->s)->pcall(r, &a1, &a2), *r); } };

public:
	DR_RINLINE			Eslot2(const Eslot2 &r): Eslot2_g(r) {}
	DR_RINLINE			Eslot2(Eimpl_g *impl_): Eslot2_g(impl_) {}
	DR_RINLINE			Eslot2(const Null &): Eslot2_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot2(R (*func)(A1 a1, A2 a2), bool vret = true): Eslot2_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot2(O *obj, R (O::*meth)(A1 a1, A2 a2), bool vret = true): Eslot2_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot2(const EmitterBase2<R, A1, A2, logtype> *em): Eslot2_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2))impl->caller)(impl, pr, a1, a2); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot1<R, A2> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot1<R, A1> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot3<R, A1, A2, AI> aAdd() const { return *reinterpret_cast<const Eslot3<R, A1, A2, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot2<NR, A1, A2> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot2<NR, A1, A2> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot2<NR, A1, A2> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2>
	DR_RINLINE Eslot2<NR, NA1, NA2> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2>::callme, *this); }
	DR_RINLINE Eslot2<R, A1, A2> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot2<R, A1, A2> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot2<R, A1, A2> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot2<R, A1, A2> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot2<R, A1, A2> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot2<R, A1, A2> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot2<R, A1, A2> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot3<R, A1, A2, AA1> aAdd1() { return *reinterpret_cast<const Eslot3<R, A1, A2, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot4<R, A1, A2, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot4<R, A1, A2, AA1, AA2> *>(this); }
	template <typename AA1, typename AA2, typename AA3>
	DR_RINLINE Eslot5<R, A1, A2, AA1, AA2, AA3> aAdd3() { return *reinterpret_cast<const Eslot5<R, A1, A2, AA1, AA2, AA3> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4>
	DR_RINLINE Eslot6<R, A1, A2, AA1, AA2, AA3, AA4> aAdd4() { return *reinterpret_cast<const Eslot6<R, A1, A2, AA1, AA2, AA3, AA4> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5>
	DR_RINLINE Eslot7<R, A1, A2, AA1, AA2, AA3, AA4, AA5> aAdd5() { return *reinterpret_cast<const Eslot7<R, A1, A2, AA1, AA2, AA3, AA4, AA5> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5, typename AA6>
	DR_RINLINE Eslot8<R, A1, A2, AA1, AA2, AA3, AA4, AA5, AA6> aAdd6() { return *reinterpret_cast<const Eslot8<R, A1, A2, AA1, AA2, AA3, AA4, AA5, AA6> *>(this); }
};

template <typename R, typename A1, typename A2>
DR_RINLINE Eslot2<R, A1, A2> Eslot(R (*func)(A1 a1, A2 a2), bool vret = true)
{
	return Eslot2<R, A1, A2>(func, vret);
}
template <typename O, typename R, typename A1, typename A2>
DR_RINLINE Eslot2<R, A1, A2> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2), bool vret = true)
{
	return Eslot2<R, A1, A2>(o, meth, vret);
}
template <typename R, typename A1, typename A2, bool logtype>
DR_RINLINE Eslot2<R, A1, A2>Eslot(EmitterBase2<R, A1, A2, logtype> *e)
{
	return Eslot2<R, A1, A2>(e);
}


class Eslot3_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot3_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot3_g(const Eslot3_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot3_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2, const void *a3) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2, const void *a3))impl->caller)(impl, r, a1, a2, a3); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2, typename A3>
class Eslot3: public Eslot3_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { return pcall(r, &a1, &a2, &a3); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2, A3 a3))me->func)(a1, a2, a3)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2, A3 a3))me->meth))(a1, a2, a3)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { return ((EmitterBase3<R, A1, A2, A3, logtype> *)me->em)->emitC(a1, a2, a3); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3) { A1 a1(me->c); return ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A3>::ref a3) { A2 a2(me->c); return ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); }
		static bool callme3(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2) { A3 a3(me->c); return ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { typename typeref<R>::nvtyp r; return ((Eslot3_g *)&me->s)->pcall(&r, &a1, &a2, &a3); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { typename typeref<R>::nvtyp r; bool o = ((Eslot3_g *)&me->s)->pcall(&r, &a1, &a2, &a3); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { typename typeref<R>::typ r; bool o = ((Eslot3_g *)&me->s)->pcall(&r, &a1, &a2, &a3); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2, typename NA3>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2, typename typeref<NA3>::aref na3) { typename typeref<R>::nvtyp r; bool o = ((Eslot3<R, A1, A2, A3> *)&me->s)->vcall(&r, na1, na2, na3); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { ((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { return me->c(((Eslot3_g *)&me->s)->pcall(r, &a1, &a2, &a3), *r); } };

public:
	DR_RINLINE			Eslot3(const Eslot3 &r): Eslot3_g(r) {}
	DR_RINLINE			Eslot3(Eimpl_g *impl_): Eslot3_g(impl_) {}
	DR_RINLINE			Eslot3(const Null &): Eslot3_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot3(R (*func)(A1 a1, A2 a2, A3 a3), bool vret = true): Eslot3_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot3(O *obj, R (O::*meth)(A1 a1, A2 a2, A3 a3), bool vret = true): Eslot3_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot3(const EmitterBase3<R, A1, A2, A3, logtype> *em): Eslot3_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3))impl->caller)(impl, pr, a1, a2, a3); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot2<R, A2, A3> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot2<R, A1, A3> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot2<R, A1, A2> a3Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme3, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot4<R, A1, A2, A3, AI> aAdd() const { return *reinterpret_cast<const Eslot4<R, A1, A2, A3, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot3<NR, A1, A2, A3> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot3<NR, A1, A2, A3> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot3<NR, A1, A2, A3> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2, typename NA3>
	DR_RINLINE Eslot3<NR, NA1, NA2, NA3> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2, NA3>::callme, *this); }
	DR_RINLINE Eslot3<R, A1, A2, A3> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot3<R, A1, A2, A3> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot3<R, A1, A2, A3> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot3<R, A1, A2, A3> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot3<R, A1, A2, A3> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot3<R, A1, A2, A3> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot3<R, A1, A2, A3> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot4<R, A1, A2, A3, AA1> aAdd1() { return *reinterpret_cast<const Eslot4<R, A1, A2, A3, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot5<R, A1, A2, A3, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot5<R, A1, A2, A3, AA1, AA2> *>(this); }
	template <typename AA1, typename AA2, typename AA3>
	DR_RINLINE Eslot6<R, A1, A2, A3, AA1, AA2, AA3> aAdd3() { return *reinterpret_cast<const Eslot6<R, A1, A2, A3, AA1, AA2, AA3> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4>
	DR_RINLINE Eslot7<R, A1, A2, A3, AA1, AA2, AA3, AA4> aAdd4() { return *reinterpret_cast<const Eslot7<R, A1, A2, A3, AA1, AA2, AA3, AA4> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4, typename AA5>
	DR_RINLINE Eslot8<R, A1, A2, A3, AA1, AA2, AA3, AA4, AA5> aAdd5() { return *reinterpret_cast<const Eslot8<R, A1, A2, A3, AA1, AA2, AA3, AA4, AA5> *>(this); }
};

template <typename R, typename A1, typename A2, typename A3>
DR_RINLINE Eslot3<R, A1, A2, A3> Eslot(R (*func)(A1 a1, A2 a2, A3 a3), bool vret = true)
{
	return Eslot3<R, A1, A2, A3>(func, vret);
}
template <typename O, typename R, typename A1, typename A2, typename A3>
DR_RINLINE Eslot3<R, A1, A2, A3> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2, A3 a3), bool vret = true)
{
	return Eslot3<R, A1, A2, A3>(o, meth, vret);
}
template <typename R, typename A1, typename A2, typename A3, bool logtype>
DR_RINLINE Eslot3<R, A1, A2, A3>Eslot(EmitterBase3<R, A1, A2, A3, logtype> *e)
{
	return Eslot3<R, A1, A2, A3>(e);
}


class Eslot4_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot4_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot4_g(const Eslot4_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot4_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2, const void *a3, const void *a4) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2, const void *a3, const void *a4))impl->caller)(impl, r, a1, a2, a3, a4); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class Eslot4: public Eslot4_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { return pcall(r, &a1, &a2, &a3, &a4); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2, A3 a3, A4 a4))me->func)(a1, a2, a3, a4)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2, A3 a3, A4 a4))me->meth))(a1, a2, a3, a4)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { return ((EmitterBase4<R, A1, A2, A3, A4, logtype> *)me->em)->emitC(a1, a2, a3, a4); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4) { A1 a1(me->c); return ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4) { A2 a2(me->c); return ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); }
		static bool callme3(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A4>::ref a4) { A3 a3(me->c); return ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); }
		static bool callme4(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3) { A4 a4(me->c); return ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { typename typeref<R>::nvtyp r; return ((Eslot4_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { typename typeref<R>::nvtyp r; bool o = ((Eslot4_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { typename typeref<R>::typ r; bool o = ((Eslot4_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2, typename typeref<NA3>::aref na3, typename typeref<NA4>::aref na4) { typename typeref<R>::nvtyp r; bool o = ((Eslot4<R, A1, A2, A3, A4> *)&me->s)->vcall(&r, na1, na2, na3, na4); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { ((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { return me->c(((Eslot4_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4), *r); } };

public:
	DR_RINLINE			Eslot4(const Eslot4 &r): Eslot4_g(r) {}
	DR_RINLINE			Eslot4(Eimpl_g *impl_): Eslot4_g(impl_) {}
	DR_RINLINE			Eslot4(const Null &): Eslot4_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot4(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4), bool vret = true): Eslot4_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot4(O *obj, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4), bool vret = true): Eslot4_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot4(const EmitterBase4<R, A1, A2, A3, A4, logtype> *em): Eslot4_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4))impl->caller)(impl, pr, a1, a2, a3, a4); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot3<R, A2, A3, A4> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot3<R, A1, A3, A4> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot3<R, A1, A2, A4> a3Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme3, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot3<R, A1, A2, A3> a4Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme4, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, AI> aAdd() const { return *reinterpret_cast<const Eslot5<R, A1, A2, A3, A4, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot4<NR, A1, A2, A3, A4> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot4<NR, A1, A2, A3, A4> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot4<NR, A1, A2, A3, A4> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4>
	DR_RINLINE Eslot4<NR, NA1, NA2, NA3, NA4> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2, NA3, NA4>::callme, *this); }
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, AA1> aAdd1() { return *reinterpret_cast<const Eslot5<R, A1, A2, A3, A4, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot6<R, A1, A2, A3, A4, AA1, AA2> *>(this); }
	template <typename AA1, typename AA2, typename AA3>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, AA1, AA2, AA3> aAdd3() { return *reinterpret_cast<const Eslot7<R, A1, A2, A3, A4, AA1, AA2, AA3> *>(this); }
	template <typename AA1, typename AA2, typename AA3, typename AA4>
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, AA1, AA2, AA3, AA4> aAdd4() { return *reinterpret_cast<const Eslot8<R, A1, A2, A3, A4, AA1, AA2, AA3, AA4> *>(this); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
DR_RINLINE Eslot4<R, A1, A2, A3, A4> Eslot(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4), bool vret = true)
{
	return Eslot4<R, A1, A2, A3, A4>(func, vret);
}
template <typename O, typename R, typename A1, typename A2, typename A3, typename A4>
DR_RINLINE Eslot4<R, A1, A2, A3, A4> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4), bool vret = true)
{
	return Eslot4<R, A1, A2, A3, A4>(o, meth, vret);
}
template <typename R, typename A1, typename A2, typename A3, typename A4, bool logtype>
DR_RINLINE Eslot4<R, A1, A2, A3, A4>Eslot(EmitterBase4<R, A1, A2, A3, A4, logtype> *e)
{
	return Eslot4<R, A1, A2, A3, A4>(e);
}


class Eslot5_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot5_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot5_g(const Eslot5_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot5_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5))impl->caller)(impl, r, a1, a2, a3, a4, a5); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class Eslot5: public Eslot5_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { return pcall(r, &a1, &a2, &a3, &a4, &a5); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5))me->func)(a1, a2, a3, a4, a5)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5))me->meth))(a1, a2, a3, a4, a5)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { return ((EmitterBase5<R, A1, A2, A3, A4, A5, logtype> *)me->em)->emitC(a1, a2, a3, a4, a5); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5) { A1 a1(me->c); return ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5) { A2 a2(me->c); return ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); }
		static bool callme3(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5) { A3 a3(me->c); return ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); }
		static bool callme4(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A5>::ref a5) { A4 a4(me->c); return ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); }
		static bool callme5(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4) { A5 a5(me->c); return ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { typename typeref<R>::nvtyp r; return ((Eslot5_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { typename typeref<R>::nvtyp r; bool o = ((Eslot5_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { typename typeref<R>::typ r; bool o = ((Eslot5_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2, typename typeref<NA3>::aref na3, typename typeref<NA4>::aref na4, typename typeref<NA5>::aref na5) { typename typeref<R>::nvtyp r; bool o = ((Eslot5<R, A1, A2, A3, A4, A5> *)&me->s)->vcall(&r, na1, na2, na3, na4, na5); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { ((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { return me->c(((Eslot5_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5), *r); } };

public:
	DR_RINLINE			Eslot5(const Eslot5 &r): Eslot5_g(r) {}
	DR_RINLINE			Eslot5(Eimpl_g *impl_): Eslot5_g(impl_) {}
	DR_RINLINE			Eslot5(const Null &): Eslot5_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot5(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5), bool vret = true): Eslot5_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot5(O *obj, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5), bool vret = true): Eslot5_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot5(const EmitterBase5<R, A1, A2, A3, A4, A5, logtype> *em): Eslot5_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5))impl->caller)(impl, pr, a1, a2, a3, a4, a5); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot4<R, A2, A3, A4, A5> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot4<R, A1, A3, A4, A5> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot4<R, A1, A2, A4, A5> a3Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme3, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot4<R, A1, A2, A3, A5> a4Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme4, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot4<R, A1, A2, A3, A4> a5Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme5, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, AI> aAdd() const { return *reinterpret_cast<const Eslot6<R, A1, A2, A3, A4, A5, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot5<NR, A1, A2, A3, A4, A5> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot5<NR, A1, A2, A3, A4, A5> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot5<NR, A1, A2, A3, A4, A5> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5>
	DR_RINLINE Eslot5<NR, NA1, NA2, NA3, NA4, NA5> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2, NA3, NA4, NA5>::callme, *this); }
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, AA1> aAdd1() { return *reinterpret_cast<const Eslot6<R, A1, A2, A3, A4, A5, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot7<R, A1, A2, A3, A4, A5, AA1, AA2> *>(this); }
	template <typename AA1, typename AA2, typename AA3>
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, AA1, AA2, AA3> aAdd3() { return *reinterpret_cast<const Eslot8<R, A1, A2, A3, A4, A5, AA1, AA2, AA3> *>(this); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> Eslot(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5), bool vret = true)
{
	return Eslot5<R, A1, A2, A3, A4, A5>(func, vret);
}
template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5), bool vret = true)
{
	return Eslot5<R, A1, A2, A3, A4, A5>(o, meth, vret);
}
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, bool logtype>
DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5>Eslot(EmitterBase5<R, A1, A2, A3, A4, A5, logtype> *e)
{
	return Eslot5<R, A1, A2, A3, A4, A5>(e);
}


class Eslot6_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot6_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot6_g(const Eslot6_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot6_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6))impl->caller)(impl, r, a1, a2, a3, a4, a5, a6); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Eslot6: public Eslot6_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { return pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6))me->func)(a1, a2, a3, a4, a5, a6)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6))me->meth))(a1, a2, a3, a4, a5, a6)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { return ((EmitterBase6<R, A1, A2, A3, A4, A5, A6, logtype> *)me->em)->emitC(a1, a2, a3, a4, a5, a6); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6) { A1 a1(me->c); return ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6) { A2 a2(me->c); return ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
		static bool callme3(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6) { A3 a3(me->c); return ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
		static bool callme4(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6) { A4 a4(me->c); return ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
		static bool callme5(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A6>::ref a6) { A5 a5(me->c); return ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
		static bool callme6(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5) { A6 a6(me->c); return ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { typename typeref<R>::nvtyp r; return ((Eslot6_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { typename typeref<R>::nvtyp r; bool o = ((Eslot6_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { typename typeref<R>::typ r; bool o = ((Eslot6_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5, typename NA6>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2, typename typeref<NA3>::aref na3, typename typeref<NA4>::aref na4, typename typeref<NA5>::aref na5, typename typeref<NA6>::aref na6) { typename typeref<R>::nvtyp r; bool o = ((Eslot6<R, A1, A2, A3, A4, A5, A6> *)&me->s)->vcall(&r, na1, na2, na3, na4, na5, na6); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { ((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { return me->c(((Eslot6_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6), *r); } };

public:
	DR_RINLINE			Eslot6(const Eslot6 &r): Eslot6_g(r) {}
	DR_RINLINE			Eslot6(Eimpl_g *impl_): Eslot6_g(impl_) {}
	DR_RINLINE			Eslot6(const Null &): Eslot6_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot6(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6), bool vret = true): Eslot6_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot6(O *obj, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6), bool vret = true): Eslot6_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot6(const EmitterBase6<R, A1, A2, A3, A4, A5, A6, logtype> *em): Eslot6_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6))impl->caller)(impl, pr, a1, a2, a3, a4, a5, a6); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot5<R, A2, A3, A4, A5, A6> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot5<R, A1, A3, A4, A5, A6> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot5<R, A1, A2, A4, A5, A6> a3Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme3, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot5<R, A1, A2, A3, A5, A6> a4Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme4, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A6> a5Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme5, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> a6Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme6, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, AI> aAdd() const { return *reinterpret_cast<const Eslot7<R, A1, A2, A3, A4, A5, A6, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot6<NR, A1, A2, A3, A4, A5, A6> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot6<NR, A1, A2, A3, A4, A5, A6> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot6<NR, A1, A2, A3, A4, A5, A6> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5, typename NA6>
	DR_RINLINE Eslot6<NR, NA1, NA2, NA3, NA4, NA5, NA6> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2, NA3, NA4, NA5, NA6>::callme, *this); }
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, AA1> aAdd1() { return *reinterpret_cast<const Eslot7<R, A1, A2, A3, A4, A5, A6, AA1> *>(this); }
	template <typename AA1, typename AA2>
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, AA1, AA2> aAdd2() { return *reinterpret_cast<const Eslot8<R, A1, A2, A3, A4, A5, A6, AA1, AA2> *>(this); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> Eslot(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6), bool vret = true)
{
	return Eslot6<R, A1, A2, A3, A4, A5, A6>(func, vret);
}
template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6), bool vret = true)
{
	return Eslot6<R, A1, A2, A3, A4, A5, A6>(o, meth, vret);
}
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, bool logtype>
DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6>Eslot(EmitterBase6<R, A1, A2, A3, A4, A5, A6, logtype> *e)
{
	return Eslot6<R, A1, A2, A3, A4, A5, A6>(e);
}


class Eslot7_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot7_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot7_g(const Eslot7_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot7_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7))impl->caller)(impl, r, a1, a2, a3, a4, a5, a6, a7); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class Eslot7: public Eslot7_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { return pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7))me->func)(a1, a2, a3, a4, a5, a6, a7)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7))me->meth))(a1, a2, a3, a4, a5, a6, a7)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { return ((EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, logtype> *)me->em)->emitC(a1, a2, a3, a4, a5, a6, a7); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7) { A1 a1(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7) { A2 a2(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
		static bool callme3(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7) { A3 a3(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
		static bool callme4(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7) { A4 a4(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
		static bool callme5(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7) { A5 a5(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
		static bool callme6(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A7>::ref a7) { A6 a6(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
		static bool callme7(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6) { A7 a7(me->c); return ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { typename typeref<R>::nvtyp r; return ((Eslot7_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { typename typeref<R>::nvtyp r; bool o = ((Eslot7_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { typename typeref<R>::typ r; bool o = ((Eslot7_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5, typename NA6, typename NA7>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2, typename typeref<NA3>::aref na3, typename typeref<NA4>::aref na4, typename typeref<NA5>::aref na5, typename typeref<NA6>::aref na6, typename typeref<NA7>::aref na7) { typename typeref<R>::nvtyp r; bool o = ((Eslot7<R, A1, A2, A3, A4, A5, A6, A7> *)&me->s)->vcall(&r, na1, na2, na3, na4, na5, na6, na7); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { ((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { return me->c(((Eslot7_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7), *r); } };

public:
	DR_RINLINE			Eslot7(const Eslot7 &r): Eslot7_g(r) {}
	DR_RINLINE			Eslot7(Eimpl_g *impl_): Eslot7_g(impl_) {}
	DR_RINLINE			Eslot7(const Null &): Eslot7_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot7(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7), bool vret = true): Eslot7_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot7(O *obj, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7), bool vret = true): Eslot7_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot7(const EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, logtype> *em): Eslot7_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7))impl->caller)(impl, pr, a1, a2, a3, a4, a5, a6, a7); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot6<R, A2, A3, A4, A5, A6, A7> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot6<R, A1, A3, A4, A5, A6, A7> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot6<R, A1, A2, A4, A5, A6, A7> a3Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme3, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot6<R, A1, A2, A3, A5, A6, A7> a4Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme4, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A6, A7> a5Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme5, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A7> a6Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme6, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> a7Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme7, *this, as); }
	template <typename AI>
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, AI> aAdd() const { return *reinterpret_cast<const Eslot8<R, A1, A2, A3, A4, A5, A6, A7, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
	template <typename NR>
	DR_RINLINE Eslot7<NR, A1, A2, A3, A4, A5, A6, A7> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot7<NR, A1, A2, A3, A4, A5, A6, A7> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot7<NR, A1, A2, A3, A4, A5, A6, A7> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5, typename NA6, typename NA7>
	DR_RINLINE Eslot7<NR, NA1, NA2, NA3, NA4, NA5, NA6, NA7> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2, NA3, NA4, NA5, NA6, NA7>::callme, *this); }
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
	template <typename AA1>
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, AA1> aAdd1() { return *reinterpret_cast<const Eslot8<R, A1, A2, A3, A4, A5, A6, A7, AA1> *>(this); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> Eslot(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7), bool vret = true)
{
	return Eslot7<R, A1, A2, A3, A4, A5, A6, A7>(func, vret);
}
template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7), bool vret = true)
{
	return Eslot7<R, A1, A2, A3, A4, A5, A6, A7>(o, meth, vret);
}
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, bool logtype>
DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7>Eslot(EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, logtype> *e)
{
	return Eslot7<R, A1, A2, A3, A4, A5, A6, A7>(e);
}


class Eslot8_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot8_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot8_g(const Eslot8_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot8_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7, const void *a8) { return ((bool (*)(void *me, void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7, const void *a8))impl->caller)(impl, r, a1, a2, a3, a4, a5, a6, a7, a8); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class Eslot8: public Eslot8_g
{
	DR_RINLINE bool			vcall(R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { return pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { Epassign(r, ((typename typeref<R>::nvsimp (*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8))me->func)(a1, a2, a3, a4, a5, a6, a7, a8)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8))me->meth))(a1, a2, a3, a4, a5, a6, a7, a8)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { return ((EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, logtype> *)me->em)->emitC(a1, a2, a3, a4, a5, a6, a7, a8); } };

	template <typename AS>
	struct cAS
	{
		static bool callme1(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7, typename typeref<A8>::ref a8) { A1 a1(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme2(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7, typename typeref<A8>::ref a8) { A2 a2(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme3(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7, typename typeref<A8>::ref a8) { A3 a3(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme4(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7, typename typeref<A8>::ref a8) { A4 a4(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme5(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7, typename typeref<A8>::ref a8) { A5 a5(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme6(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A7>::ref a7, typename typeref<A8>::ref a8) { A6 a6(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme7(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A8>::ref a8) { A7 a7(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
		static bool callme8(EimplSC_g<AS> *me, typename typeref<R>::ptr r, typename typeref<A1>::ref a1, typename typeref<A2>::ref a2, typename typeref<A3>::ref a3, typename typeref<A4>::ref a4, typename typeref<A5>::ref a5, typename typeref<A6>::ref a6, typename typeref<A7>::ref a7) { A8 a8(me->c); return ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); }
	};
	struct cRI { static bool callme(EimplS_g *me, void *r_, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { typename typeref<R>::nvtyp r; return ((Eslot8_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); } };
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { typename typeref<R>::nvtyp r; bool o = ((Eslot8_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); *nr = me->c; return o; } };
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { typename typeref<R>::typ r; bool o = ((Eslot8_g *)&me->s)->pcall(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); *nr = (NR)r; return o; } };
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5, typename NA6, typename NA7, typename NA8>
	struct cFR { static bool callme(EimplS_g *me, NR *nr, typename typeref<NA1>::aref na1, typename typeref<NA2>::aref na2, typename typeref<NA3>::aref na3, typename typeref<NA4>::aref na4, typename typeref<NA5>::aref na5, typename typeref<NA6>::aref na6, typename typeref<NA7>::aref na7, typename typeref<NA8>::aref na8) { typename typeref<R>::nvtyp r; bool o = ((Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> *)&me->s)->vcall(&r, na1, na2, na3, na4, na5, na6, na7, na8); Epassign(nr, r); return o; } };
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { ((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { return me->c(((Eslot8_g *)&me->s)->pcall(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8), *r); } };

public:
	DR_RINLINE			Eslot8(const Eslot8 &r): Eslot8_g(r) {}
	DR_RINLINE			Eslot8(Eimpl_g *impl_): Eslot8_g(impl_) {}
	DR_RINLINE			Eslot8(const Null &): Eslot8_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot8(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8), bool vret = true): Eslot8_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot8(O *obj, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8), bool vret = true): Eslot8_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot8(const EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, logtype> *em): Eslot8_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()(typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8))impl->caller)(impl, pr, a1, a2, a3, a4, a5, a6, a7, a8); return *(typename typeref<R>::nvtyp *)pr; }
	template <typename AS>
	DR_RINLINE Eslot7<R, A2, A3, A4, A5, A6, A7, A8> a1Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme1, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A3, A4, A5, A6, A7, A8> a2Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme2, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A2, A4, A5, A6, A7, A8> a3Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme3, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A2, A3, A5, A6, A7, A8> a4Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme4, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A6, A7, A8> a5Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme5, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A7, A8> a6Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme6, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A8> a7Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme7, *this, as); }
	template <typename AS>
	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> a8Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme8, *this, as); }
	template <typename NR>
	DR_RINLINE Eslot8<NR, A1, A2, A3, A4, A5, A6, A7, A8> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
	template <typename NR, typename SNR>
	DR_RINLINE Eslot8<NR, A1, A2, A3, A4, A5, A6, A7, A8> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
	template <typename NR>
	DR_RINLINE Eslot8<NR, A1, A2, A3, A4, A5, A6, A7, A8> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
	template <typename NR, typename NA1, typename NA2, typename NA3, typename NA4, typename NA5, typename NA6, typename NA7, typename NA8>
	DR_RINLINE Eslot8<NR, NA1, NA2, NA3, NA4, NA5, NA6, NA7, NA8> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR, NA1, NA2, NA3, NA4, NA5, NA6, NA7, NA8>::callme, *this); }
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> Eslot(R (*func)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8), bool vret = true)
{
	return Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>(func, vret);
}
template <typename O, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> Eslot(O *o, R (O::*meth)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8), bool vret = true)
{
	return Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>(o, meth, vret);
}
template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, bool logtype>
DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>Eslot(EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, logtype> *e)
{
	return Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>(e);
}

class DR_PUB Emitter0_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter0_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r);
	bool emitOR_g(void *r);
};

template <typename R>
class EmitterBaseC0: public Emitter0_g
{
public:
	typedef Enode0<R> Node;

	DR_MINLINE EmitterBaseC0(bool vret_): Emitter0_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot0<R> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot0<R> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
};

template <typename R, bool logtype>
class EmitterBase0: public EmitterBaseC0<R>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit() /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr):emitAND_g(&r);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def):emitAND_g(&def);
		return def;
	}
	DR_RINLINE bool emitI() /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r):emitAND_g(&r);
	}
	DR_RINLINE bool emitC(R *r) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r):emitAND_g(&r);
	}
	DR_MINLINE EmitterBase0(bool vret_): EmitterBaseC0<R>(vret_) {}

	DR_RINLINE Eslot0<R> slot() { return Eslot0<R>(this, &emitC); }
};

template <typename R>
class EmitterAND0: public EmitterBase0<R, 0>
{
public:
	DR_RINLINE EmitterAND0(bool vret_ = true): EmitterBase0<R, 0>(vret_) {}
};

template <typename R>
class EmitterOR0: public EmitterBase0<R, 1>
{
public:
	DR_RINLINE EmitterOR0(bool vret_ = false): EmitterBase0<R, 1>(vret_) {}
};

template <typename R, bool logtype>
class EslotLog0: public Eslot0<R>
{
	typedef EimplL_g< EmitterBase0<R, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r) { return me->emitC(); }

public:
	DR_MINLINE EslotLog0(bool vret_ = !logtype): Eslot0<R>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog0(const EslotLog0 &r): Eslot0<R>(r) {}

	DR_MINLINE EmitterBase0<R, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R>
class EslotAND0: public EslotLog0<R, 0>
{
	typedef EslotLog0<R, 0> Base;
	typedef EmitterAND0<R> Log;
public:
	DR_RINLINE EslotAND0(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND0(const EslotAND0 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R>
class EslotOR0: public EslotLog0<R, 1>
{
	typedef EslotLog0<R, 1> Base;
	typedef EmitterOR0<R> Log;
public:
	DR_RINLINE EslotOR0(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR0(const EslotOR0 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter1_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter1_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1);
	bool emitOR_g(void *r, const void *a1);
};

template <typename R, typename A1>
class EmitterBaseC1: public Emitter1_g
{
public:
	typedef Enode1<R, A1> Node;

	DR_MINLINE EmitterBaseC1(bool vret_): Emitter1_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot1<R, A1> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot1<R, A1> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, bool logtype>
class EmitterBase1: public EmitterBaseC1<R, A1>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1):emitAND_g(&r, &a1);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1):emitAND_g(&def, &a1);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1):emitAND_g(&r, &a1);
	}
	DR_RINLINE bool emitC(R *r, A1 a1) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1):emitAND_g(&r, &a1);
	}
	DR_MINLINE EmitterBase1(bool vret_): EmitterBaseC1<R, A1>(vret_) {}

	DR_RINLINE Eslot1<R, A1> slot() { return Eslot1<R, A1>(this, &emitC); }
};

template <typename R, typename A1>
class EmitterAND1: public EmitterBase1<R, A1, 0>
{
public:
	DR_RINLINE EmitterAND1(bool vret_ = true): EmitterBase1<R, A1, 0>(vret_) {}
};

template <typename R, typename A1>
class EmitterOR1: public EmitterBase1<R, A1, 1>
{
public:
	DR_RINLINE EmitterOR1(bool vret_ = false): EmitterBase1<R, A1, 1>(vret_) {}
};

template <typename R, typename A1, bool logtype>
class EslotLog1: public Eslot1<R, A1>
{
	typedef EimplL_g< EmitterBase1<R, A1, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1) { return me->emitC(a1); }

public:
	DR_MINLINE EslotLog1(bool vret_ = !logtype): Eslot1<R, A1>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog1(const EslotLog1 &r): Eslot1<R, A1>(r) {}

	DR_MINLINE EmitterBase1<R, A1, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1>
class EslotAND1: public EslotLog1<R, A1, 0>
{
	typedef EslotLog1<R, A1, 0> Base;
	typedef EmitterAND1<R, A1> Log;
public:
	DR_RINLINE EslotAND1(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND1(const EslotAND1 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1>
class EslotOR1: public EslotLog1<R, A1, 1>
{
	typedef EslotLog1<R, A1, 1> Base;
	typedef EmitterOR1<R, A1> Log;
public:
	DR_RINLINE EslotOR1(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR1(const EslotOR1 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter2_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter2_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2);
	bool emitOR_g(void *r, const void *a1, const void *a2);
};

template <typename R, typename A1, typename A2>
class EmitterBaseC2: public Emitter2_g
{
public:
	typedef Enode2<R, A1, A2> Node;

	DR_MINLINE EmitterBaseC2(bool vret_): Emitter2_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot2<R, A1, A2> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot2<R, A1, A2> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, bool logtype>
class EmitterBase2: public EmitterBaseC2<R, A1, A2>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2):emitAND_g(&r, &a1, &a2);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2):emitAND_g(&def, &a1, &a2);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2):emitAND_g(&r, &a1, &a2);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2):emitAND_g(&r, &a1, &a2);
	}
	DR_MINLINE EmitterBase2(bool vret_): EmitterBaseC2<R, A1, A2>(vret_) {}

	DR_RINLINE Eslot2<R, A1, A2> slot() { return Eslot2<R, A1, A2>(this, &emitC); }
};

template <typename R, typename A1, typename A2>
class EmitterAND2: public EmitterBase2<R, A1, A2, 0>
{
public:
	DR_RINLINE EmitterAND2(bool vret_ = true): EmitterBase2<R, A1, A2, 0>(vret_) {}
};

template <typename R, typename A1, typename A2>
class EmitterOR2: public EmitterBase2<R, A1, A2, 1>
{
public:
	DR_RINLINE EmitterOR2(bool vret_ = false): EmitterBase2<R, A1, A2, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, bool logtype>
class EslotLog2: public Eslot2<R, A1, A2>
{
	typedef EimplL_g< EmitterBase2<R, A1, A2, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2) { return me->emitC(a1, a2); }

public:
	DR_MINLINE EslotLog2(bool vret_ = !logtype): Eslot2<R, A1, A2>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog2(const EslotLog2 &r): Eslot2<R, A1, A2>(r) {}

	DR_MINLINE EmitterBase2<R, A1, A2, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2>
class EslotAND2: public EslotLog2<R, A1, A2, 0>
{
	typedef EslotLog2<R, A1, A2, 0> Base;
	typedef EmitterAND2<R, A1, A2> Log;
public:
	DR_RINLINE EslotAND2(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND2(const EslotAND2 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2>
class EslotOR2: public EslotLog2<R, A1, A2, 1>
{
	typedef EslotLog2<R, A1, A2, 1> Base;
	typedef EmitterOR2<R, A1, A2> Log;
public:
	DR_RINLINE EslotOR2(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR2(const EslotOR2 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter3_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter3_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2, const void *a3);
	bool emitOR_g(void *r, const void *a1, const void *a2, const void *a3);
};

template <typename R, typename A1, typename A2, typename A3>
class EmitterBaseC3: public Emitter3_g
{
public:
	typedef Enode3<R, A1, A2, A3> Node;

	DR_MINLINE EmitterBaseC3(bool vret_): Emitter3_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot3<R, A1, A2, A3> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot3<R, A1, A2, A3> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, typename A3, bool logtype>
class EmitterBase3: public EmitterBaseC3<R, A1, A2, A3>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2, A3 a3) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2, &a3):emitAND_g(&r, &a1, &a2, &a3);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2, A3 a3) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2, &a3):emitAND_g(&def, &a1, &a2, &a3);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2, A3 a3) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2, &a3):emitAND_g(&r, &a1, &a2, &a3);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2, A3 a3) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2, &a3):emitAND_g(&r, &a1, &a2, &a3);
	}
	DR_MINLINE EmitterBase3(bool vret_): EmitterBaseC3<R, A1, A2, A3>(vret_) {}

	DR_RINLINE Eslot3<R, A1, A2, A3> slot() { return Eslot3<R, A1, A2, A3>(this, &emitC); }
};

template <typename R, typename A1, typename A2, typename A3>
class EmitterAND3: public EmitterBase3<R, A1, A2, A3, 0>
{
public:
	DR_RINLINE EmitterAND3(bool vret_ = true): EmitterBase3<R, A1, A2, A3, 0>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3>
class EmitterOR3: public EmitterBase3<R, A1, A2, A3, 1>
{
public:
	DR_RINLINE EmitterOR3(bool vret_ = false): EmitterBase3<R, A1, A2, A3, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, bool logtype>
class EslotLog3: public Eslot3<R, A1, A2, A3>
{
	typedef EimplL_g< EmitterBase3<R, A1, A2, A3, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3) { return me->emitC(a1, a2, a3); }

public:
	DR_MINLINE EslotLog3(bool vret_ = !logtype): Eslot3<R, A1, A2, A3>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog3(const EslotLog3 &r): Eslot3<R, A1, A2, A3>(r) {}

	DR_MINLINE EmitterBase3<R, A1, A2, A3, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2, typename A3>
class EslotAND3: public EslotLog3<R, A1, A2, A3, 0>
{
	typedef EslotLog3<R, A1, A2, A3, 0> Base;
	typedef EmitterAND3<R, A1, A2, A3> Log;
public:
	DR_RINLINE EslotAND3(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND3(const EslotAND3 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2, typename A3>
class EslotOR3: public EslotLog3<R, A1, A2, A3, 1>
{
	typedef EslotLog3<R, A1, A2, A3, 1> Base;
	typedef EmitterOR3<R, A1, A2, A3> Log;
public:
	DR_RINLINE EslotOR3(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR3(const EslotOR3 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter4_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter4_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4);
	bool emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4);
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class EmitterBaseC4: public Emitter4_g
{
public:
	typedef Enode4<R, A1, A2, A3, A4> Node;

	DR_MINLINE EmitterBaseC4(bool vret_): Emitter4_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot4<R, A1, A2, A3, A4> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot4<R, A1, A2, A3, A4> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, bool logtype>
class EmitterBase4: public EmitterBaseC4<R, A1, A2, A3, A4>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2, A3 a3, A4 a4) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2, &a3, &a4):emitAND_g(&r, &a1, &a2, &a3, &a4);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2, A3 a3, A4 a4) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2, &a3, &a4):emitAND_g(&def, &a1, &a2, &a3, &a4);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2, A3 a3, A4 a4) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2, &a3, &a4):emitAND_g(&r, &a1, &a2, &a3, &a4);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2, A3 a3, A4 a4) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2, &a3, &a4):emitAND_g(&r, &a1, &a2, &a3, &a4);
	}
	DR_MINLINE EmitterBase4(bool vret_): EmitterBaseC4<R, A1, A2, A3, A4>(vret_) {}

	DR_RINLINE Eslot4<R, A1, A2, A3, A4> slot() { return Eslot4<R, A1, A2, A3, A4>(this, &emitC); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class EmitterAND4: public EmitterBase4<R, A1, A2, A3, A4, 0>
{
public:
	DR_RINLINE EmitterAND4(bool vret_ = true): EmitterBase4<R, A1, A2, A3, A4, 0>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class EmitterOR4: public EmitterBase4<R, A1, A2, A3, A4, 1>
{
public:
	DR_RINLINE EmitterOR4(bool vret_ = false): EmitterBase4<R, A1, A2, A3, A4, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, bool logtype>
class EslotLog4: public Eslot4<R, A1, A2, A3, A4>
{
	typedef EimplL_g< EmitterBase4<R, A1, A2, A3, A4, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4) { return me->emitC(a1, a2, a3, a4); }

public:
	DR_MINLINE EslotLog4(bool vret_ = !logtype): Eslot4<R, A1, A2, A3, A4>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog4(const EslotLog4 &r): Eslot4<R, A1, A2, A3, A4>(r) {}

	DR_MINLINE EmitterBase4<R, A1, A2, A3, A4, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class EslotAND4: public EslotLog4<R, A1, A2, A3, A4, 0>
{
	typedef EslotLog4<R, A1, A2, A3, A4, 0> Base;
	typedef EmitterAND4<R, A1, A2, A3, A4> Log;
public:
	DR_RINLINE EslotAND4(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND4(const EslotAND4 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class EslotOR4: public EslotLog4<R, A1, A2, A3, A4, 1>
{
	typedef EslotLog4<R, A1, A2, A3, A4, 1> Base;
	typedef EmitterOR4<R, A1, A2, A3, A4> Log;
public:
	DR_RINLINE EslotOR4(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR4(const EslotOR4 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter5_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter5_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5);
	bool emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5);
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class EmitterBaseC5: public Emitter5_g
{
public:
	typedef Enode5<R, A1, A2, A3, A4, A5> Node;

	DR_MINLINE EmitterBaseC5(bool vret_): Emitter5_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot5<R, A1, A2, A3, A4, A5> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot5<R, A1, A2, A3, A4, A5> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, bool logtype>
class EmitterBase5: public EmitterBaseC5<R, A1, A2, A3, A4, A5>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2, &a3, &a4, &a5):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2, &a3, &a4, &a5):emitAND_g(&def, &a1, &a2, &a3, &a4, &a5);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2, &a3, &a4, &a5):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2, &a3, &a4, &a5):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5);
	}
	DR_MINLINE EmitterBase5(bool vret_): EmitterBaseC5<R, A1, A2, A3, A4, A5>(vret_) {}

	DR_RINLINE Eslot5<R, A1, A2, A3, A4, A5> slot() { return Eslot5<R, A1, A2, A3, A4, A5>(this, &emitC); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class EmitterAND5: public EmitterBase5<R, A1, A2, A3, A4, A5, 0>
{
public:
	DR_RINLINE EmitterAND5(bool vret_ = true): EmitterBase5<R, A1, A2, A3, A4, A5, 0>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class EmitterOR5: public EmitterBase5<R, A1, A2, A3, A4, A5, 1>
{
public:
	DR_RINLINE EmitterOR5(bool vret_ = false): EmitterBase5<R, A1, A2, A3, A4, A5, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, bool logtype>
class EslotLog5: public Eslot5<R, A1, A2, A3, A4, A5>
{
	typedef EimplL_g< EmitterBase5<R, A1, A2, A3, A4, A5, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5) { return me->emitC(a1, a2, a3, a4, a5); }

public:
	DR_MINLINE EslotLog5(bool vret_ = !logtype): Eslot5<R, A1, A2, A3, A4, A5>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog5(const EslotLog5 &r): Eslot5<R, A1, A2, A3, A4, A5>(r) {}

	DR_MINLINE EmitterBase5<R, A1, A2, A3, A4, A5, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class EslotAND5: public EslotLog5<R, A1, A2, A3, A4, A5, 0>
{
	typedef EslotLog5<R, A1, A2, A3, A4, A5, 0> Base;
	typedef EmitterAND5<R, A1, A2, A3, A4, A5> Log;
public:
	DR_RINLINE EslotAND5(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND5(const EslotAND5 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class EslotOR5: public EslotLog5<R, A1, A2, A3, A4, A5, 1>
{
	typedef EslotLog5<R, A1, A2, A3, A4, A5, 1> Base;
	typedef EmitterOR5<R, A1, A2, A3, A4, A5> Log;
public:
	DR_RINLINE EslotOR5(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR5(const EslotOR5 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter6_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter6_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6);
	bool emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6);
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class EmitterBaseC6: public Emitter6_g
{
public:
	typedef Enode6<R, A1, A2, A3, A4, A5, A6> Node;

	DR_MINLINE EmitterBaseC6(bool vret_): Emitter6_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot6<R, A1, A2, A3, A4, A5, A6> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot6<R, A1, A2, A3, A4, A5, A6> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, bool logtype>
class EmitterBase6: public EmitterBaseC6<R, A1, A2, A3, A4, A5, A6>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2, &a3, &a4, &a5, &a6):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2, &a3, &a4, &a5, &a6):emitAND_g(&def, &a1, &a2, &a3, &a4, &a5, &a6);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2, &a3, &a4, &a5, &a6):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2, &a3, &a4, &a5, &a6):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6);
	}
	DR_MINLINE EmitterBase6(bool vret_): EmitterBaseC6<R, A1, A2, A3, A4, A5, A6>(vret_) {}

	DR_RINLINE Eslot6<R, A1, A2, A3, A4, A5, A6> slot() { return Eslot6<R, A1, A2, A3, A4, A5, A6>(this, &emitC); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class EmitterAND6: public EmitterBase6<R, A1, A2, A3, A4, A5, A6, 0>
{
public:
	DR_RINLINE EmitterAND6(bool vret_ = true): EmitterBase6<R, A1, A2, A3, A4, A5, A6, 0>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class EmitterOR6: public EmitterBase6<R, A1, A2, A3, A4, A5, A6, 1>
{
public:
	DR_RINLINE EmitterOR6(bool vret_ = false): EmitterBase6<R, A1, A2, A3, A4, A5, A6, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, bool logtype>
class EslotLog6: public Eslot6<R, A1, A2, A3, A4, A5, A6>
{
	typedef EimplL_g< EmitterBase6<R, A1, A2, A3, A4, A5, A6, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6) { return me->emitC(a1, a2, a3, a4, a5, a6); }

public:
	DR_MINLINE EslotLog6(bool vret_ = !logtype): Eslot6<R, A1, A2, A3, A4, A5, A6>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog6(const EslotLog6 &r): Eslot6<R, A1, A2, A3, A4, A5, A6>(r) {}

	DR_MINLINE EmitterBase6<R, A1, A2, A3, A4, A5, A6, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class EslotAND6: public EslotLog6<R, A1, A2, A3, A4, A5, A6, 0>
{
	typedef EslotLog6<R, A1, A2, A3, A4, A5, A6, 0> Base;
	typedef EmitterAND6<R, A1, A2, A3, A4, A5, A6> Log;
public:
	DR_RINLINE EslotAND6(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND6(const EslotAND6 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class EslotOR6: public EslotLog6<R, A1, A2, A3, A4, A5, A6, 1>
{
	typedef EslotLog6<R, A1, A2, A3, A4, A5, A6, 1> Base;
	typedef EmitterOR6<R, A1, A2, A3, A4, A5, A6> Log;
public:
	DR_RINLINE EslotOR6(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR6(const EslotOR6 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter7_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter7_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7);
	bool emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7);
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class EmitterBaseC7: public Emitter7_g
{
public:
	typedef Enode7<R, A1, A2, A3, A4, A5, A6, A7> Node;

	DR_MINLINE EmitterBaseC7(bool vret_): Emitter7_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot7<R, A1, A2, A3, A4, A5, A6, A7> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot7<R, A1, A2, A3, A4, A5, A6, A7> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, bool logtype>
class EmitterBase7: public EmitterBaseC7<R, A1, A2, A3, A4, A5, A6, A7>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2, &a3, &a4, &a5, &a6, &a7):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2, &a3, &a4, &a5, &a6, &a7):emitAND_g(&def, &a1, &a2, &a3, &a4, &a5, &a6, &a7);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7);
	}
	DR_MINLINE EmitterBase7(bool vret_): EmitterBaseC7<R, A1, A2, A3, A4, A5, A6, A7>(vret_) {}

	DR_RINLINE Eslot7<R, A1, A2, A3, A4, A5, A6, A7> slot() { return Eslot7<R, A1, A2, A3, A4, A5, A6, A7>(this, &emitC); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class EmitterAND7: public EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, 0>
{
public:
	DR_RINLINE EmitterAND7(bool vret_ = true): EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, 0>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class EmitterOR7: public EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, 1>
{
public:
	DR_RINLINE EmitterOR7(bool vret_ = false): EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, bool logtype>
class EslotLog7: public Eslot7<R, A1, A2, A3, A4, A5, A6, A7>
{
	typedef EimplL_g< EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7) { return me->emitC(a1, a2, a3, a4, a5, a6, a7); }

public:
	DR_MINLINE EslotLog7(bool vret_ = !logtype): Eslot7<R, A1, A2, A3, A4, A5, A6, A7>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog7(const EslotLog7 &r): Eslot7<R, A1, A2, A3, A4, A5, A6, A7>(r) {}

	DR_MINLINE EmitterBase7<R, A1, A2, A3, A4, A5, A6, A7, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class EslotAND7: public EslotLog7<R, A1, A2, A3, A4, A5, A6, A7, 0>
{
	typedef EslotLog7<R, A1, A2, A3, A4, A5, A6, A7, 0> Base;
	typedef EmitterAND7<R, A1, A2, A3, A4, A5, A6, A7> Log;
public:
	DR_RINLINE EslotAND7(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND7(const EslotAND7 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
class EslotOR7: public EslotLog7<R, A1, A2, A3, A4, A5, A6, A7, 1>
{
	typedef EslotLog7<R, A1, A2, A3, A4, A5, A6, A7, 1> Base;
	typedef EmitterOR7<R, A1, A2, A3, A4, A5, A6, A7> Log;
public:
	DR_RINLINE EslotOR7(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR7(const EslotOR7 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

class DR_PUB Emitter8_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter8_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7, const void *a8);
	bool emitOR_g(void *r, const void *a1, const void *a2, const void *a3, const void *a4, const void *a5, const void *a6, const void *a7, const void *a8);
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class EmitterBaseC8: public Emitter8_g
{
public:
	typedef Enode8<R, A1, A2, A3, A4, A5, A6, A7, A8> Node;

	DR_MINLINE EmitterBaseC8(bool vret_): Emitter8_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, bool logtype>
class EmitterBase8: public EmitterBaseC8<R, A1, A2, A3, A4, A5, A6, A7, A8>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8):emitAND_g(&def, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8);
		return def;
	}
	DR_RINLINE bool emitI(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8);
	}
	DR_RINLINE bool emitC(R *r, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8):emitAND_g(&r, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8);
	}
	DR_MINLINE EmitterBase8(bool vret_): EmitterBaseC8<R, A1, A2, A3, A4, A5, A6, A7, A8>(vret_) {}

	DR_RINLINE Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8> slot() { return Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>(this, &emitC); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class EmitterAND8: public EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, 0>
{
public:
	DR_RINLINE EmitterAND8(bool vret_ = true): EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, 0>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class EmitterOR8: public EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, 1>
{
public:
	DR_RINLINE EmitterOR8(bool vret_ = false): EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, 1>(vret_) {}
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, bool logtype>
class EslotLog8: public Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>
{
	typedef EimplL_g< EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r, typename typeref<A1>::aref a1, typename typeref<A2>::aref a2, typename typeref<A3>::aref a3, typename typeref<A4>::aref a4, typename typeref<A5>::aref a5, typename typeref<A6>::aref a6, typename typeref<A7>::aref a7, typename typeref<A8>::aref a8) { return me->emitC(a1, a2, a3, a4, a5, a6, a7, a8); }

public:
	DR_MINLINE EslotLog8(bool vret_ = !logtype): Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog8(const EslotLog8 &r): Eslot8<R, A1, A2, A3, A4, A5, A6, A7, A8>(r) {}

	DR_MINLINE EmitterBase8<R, A1, A2, A3, A4, A5, A6, A7, A8, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class EslotAND8: public EslotLog8<R, A1, A2, A3, A4, A5, A6, A7, A8, 0>
{
	typedef EslotLog8<R, A1, A2, A3, A4, A5, A6, A7, A8, 0> Base;
	typedef EmitterAND8<R, A1, A2, A3, A4, A5, A6, A7, A8> Log;
public:
	DR_RINLINE EslotAND8(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND8(const EslotAND8 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
class EslotOR8: public EslotLog8<R, A1, A2, A3, A4, A5, A6, A7, A8, 1>
{
	typedef EslotLog8<R, A1, A2, A3, A4, A5, A6, A7, A8, 1> Base;
	typedef EmitterOR8<R, A1, A2, A3, A4, A5, A6, A7, A8> Log;
public:
	DR_RINLINE EslotOR8(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR8(const EslotOR8 &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};


DR_NS_END

#endif
