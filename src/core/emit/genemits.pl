#!/usr/bin/perl

##
## drt library
##
## drt multiplatform development toolkit
##
## ----------------------------------------------------------------------------------
##
## Copyright (C) 2004-2008 Zbyněk Vyškovský
##
## ----------------------------------------------------------------------------------
##
## LICENSE:
##
## This file is part of drt
##
## drt is free software; you can redistribute it and/or modify it under the
## terms of the GNU Lesser General Public License as published by the Free
## Software Foundation; either version 3 of the License, or (at your option)
## any later version.
##
## drt is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
## more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with drt; if not, write to the Free Software Foundation, Inc., 51
## Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##
## @author	2004-2008 Zbyněk Vyškovský
## @link	mailto:kvr@matfyz.cz
## @link	http://kvr.matfyz.cz/drt/
## @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
##

use warnings;
use strict;

use FileHandle;

sub alistj($$$) # template numofargs join
{
	my $t = shift;
	my $n = shift;
	my $j = shift;
	my $o = "";

	return "" if ($n == 0);
	for (my $i = 1; $i <= $n; $i++) {
		($_ = $t) =~ s/###/$i/g;
		$o .= "$_$j";
	}
	return substr($o, 0, -length($j));
}

sub alist($$) # template number-of-args
{
	my $t = shift;
	my $n = shift;
	my $o = "";

	for (my $i = 1; $i <= $n; $i++) {
		($_ = $t) =~ s/###/$i/g;
		$o .= $_;
	}
	return $o;
}

sub alistjE($$$$) # template numofargs join
{
	my $t = shift;
	my $n = shift;
	my $j = shift;
	my $e = shift;
	my $o = "";

	return "" if ($n == 0);
	for (my $i = 1; $i <= $n; $i++) {
		next if ($i == $e);
		($_ = $t) =~ s/###/$i/g;
		$o .= "$_$j";
	}
	return substr($o, 0, -length($j));
}

sub alistE($$$) # template number-of-args
{
	my $t = shift;
	my $n = shift;
	my $e = shift;
	my $o = "";

	for (my $i = 1; $i <= $n; $i++) {
		next if ($i == $e);
		($_ = $t) =~ s/###/$i/g;
		$o .= $_;
	}
	return $o;
}

sub alistjR($$$$$) # template numofargs join
{
	my $t = shift;
	my $n = shift;
	my $j = shift;
	my $e = shift;
	my $r = shift;
	my $o = "";

	return "" if ($n == 0);
	for (my $i = 1; $i <= $n; $i++) {
		if ($i == $e) {
			$o .= "$r$j";
		}
		else {
			($_ = $t) =~ s/###/$i/g;
			$o .= "$_$j";
		}
	}
	return substr($o, 0, -length($j));
}

sub alistR($$$$) # template number-of-args
{
	my $t = shift;
	my $n = shift;
	my $e = shift;
	my $r = shift;
	my $o = "";

	for (my $i = 1; $i <= $n; $i++) {
		if ($i == $e) {
			$o .= "$r";
		}
		else {
			($_ = $t) =~ s/###/$i/g;
			$o .= $_;
		}
	}
	return $o;
}

die "usage: $0 header cxx-file\n" unless (@ARGV == 2);
my $fheader = FileHandle->new($ARGV[0], ">") or die "failed to open $ARGV[0]: $!";
my $fcxx = FileHandle->new($ARGV[1], ">") or die "failed to open $ARGV[1]: $!";

my $hpre = "";
my $hname = "";
my $hdecl = "";
my $himpl = "";
my $hfin = "";

my $cxxgen = "";

my $cmax = 8;

$hpre .= <<HP_;
/* This file is generated from $0... */
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

HP_

$cxxgen .= <<CG_;
/* This file is generated from $0... */

DR_NS_BEGIN


CG_

for (my $cn = 0; $cn <= $cmax; $cn++) {
	my $xid = sprintf("%x", $cn);
	my $xid_g = "${xid}_g";
	my $xidM = sprintf("%x", $cn-1);
	my $xidP = sprintf("%x", $cn+1);
	my $tnAlist = alist(", typename A###", $cn);
	my $tnNAlist = alist(", typename NA###", $cn);
	my $calalist = alist(", typename typeref<A###>::aref a###", $cn);
	my $calajoin = alistj("typename typeref<A###>::aref a###", $cn, ", ");
	my $calnalist = alist(", typename typeref<NA###>::aref na###", $cn);
	my $argjoin = alistj("A### a###", $cn, ", ");
	my $arglist = alist(", A### a###", $cn);
	my $tlAlist = alist(", A###", $cn);
	my $tlNAlist = alist(", NA###", $cn);
	my $ralist = alist(", &a###", $cn);
	my $dalist = alist(", *a###", $cn);
	my $alist = alist(", a###", $cn);
	my $nalist = alist(", na###", $cn);
	my $convnalist = alist(", na###", $cn);
	my $ajoin = alistj("a###", $cn, ", ");
	my $vparglist = alist(", const void *a###", $cn);

	$hname .= <<HN_;

// used only for type-safety pointers (implementation never used) */
template <typename R$tnAlist>
class Enode$xid: public Enode_g
{
	/* constructor */		Enode$xid(): Enode_g(Eslot_g(NULL)) {} // only to avoid warnings
};

template <typename R$tnAlist>
class Eslot$xid;

class Emitter$xid_g;

template <typename R$tnAlist, bool logtype>
class EmitterBase$xid;

HN_

	$hdecl .= <<HD_;

class Eslot$xid_g: public Eslot_g
{
protected:
	DR_MINLINE			Eslot$xid_g(const Null &s): Eslot_g(s) {}
	DR_MINLINE			Eslot$xid_g(const Eslot$xid_g &r): Eslot_g(r) {}
	DR_MINLINE			Eslot$xid_g(Eimpl_g *r): Eslot_g(r) {}

public:
	DR_RINLINE bool			pcall(void *r$vparglist) { return ((bool (*)(void *me, void *r$vparglist))impl->caller)(impl, r$alist); }
	DR_MINLINE bool			empty() { return impl == NULL; }
	
	/* for internal use */
	DR_MINLINE Eimpl_g *		toPtrRef() const { if (impl) impl->ref(); return impl; }
};

template <typename R$tnAlist>
class Eslot$xid: public Eslot$xid_g
{
	DR_RINLINE bool			vcall(R *r$calalist) { return pcall(r$ralist); }
HD_
	my $l_callers = "";
	my $l_funcs = "";
	$l_callers .= <<L_;
	struct cF { static bool callme(EimplF_g *me, typename typeref<R>::ptr r$calalist) { Epassign(r, ((typename typeref<R>::nvsimp (*)($argjoin))me->func)($ajoin)); return me->vret; } };
	template <typename O>
	struct cM { static bool callme(EimplM_g *me, typename typeref<R>::ptr r$calalist) { Epassign(r, (((O *)me->obj)->*((typename typeref<R>::nvsimp (O::*)($argjoin))me->meth))($ajoin)); return me->vret; } };
	template <bool logtype>
	struct cE { static bool callme(EimplE_g *me, typename typeref<R>::ptr r$calalist) { return ((EmitterBase$xid<R$tlAlist, logtype> *)me->em)->emitC($ajoin); } };

L_
	$l_funcs .= <<L_;

public:
	DR_RINLINE			Eslot$xid(const Eslot$xid &r): Eslot$xid_g(r) {}
	DR_RINLINE			Eslot$xid(Eimpl_g *impl_): Eslot$xid_g(impl_) {}
	DR_RINLINE			Eslot$xid(const Null &): Eslot$xid_g((Eimpl_g *)NULL) {}
	DR_RINLINE			Eslot$xid(R (*func)($argjoin), bool vret = true): Eslot$xid_g(new EimplF_g((bool (*)())&cF::callme, (void (*)())func, vret)) {}
	template <typename O>
	DR_RINLINE			Eslot$xid(O *obj, R (O::*meth)($argjoin), bool vret = true): Eslot$xid_g(new EimplM_g((bool (*)())&cM<O>::callme, obj, (void (Shared::*)())meth, vret)) {}
	template <bool logtype>
	DR_RINLINE			Eslot$xid(const EmitterBase$xid<R$tlAlist, logtype> *em): Eslot$xid_g(new EimplE_g((bool (*)())&cE<logtype>::callme, em)) {}

	DR_MINLINE typename typeref<R>::nvret operator()($calajoin) const { typename typeref<R>::nvtyp r; typename typeref<R>::ptr pr(&r); ((bool (*)(void *me, void *r_$calalist))impl->caller)(impl, pr$alist); return *(typename typeref<R>::nvtyp *)pr; }
L_
	$l_callers .= <<L_;
	template <typename AS>
	struct cAS
	{
L_
	for (my $i = 1; $i <= $cn; $i++) {
		my $tlAlistI = alistE(", A###", $cn, $i);
		my $calalistI = alistE(", typename typeref<A###>::ref a###", $cn, $i);
		my $ralistI = alistR(", &a###", $cn, $i, ", &a$i");
		$l_callers .= <<L_;
		static bool callme$i(EimplSC_g<AS> *me, typename typeref<R>::ptr r$calalistI) { A$i a$i(me->c); return ((Eslot$xid_g *)&me->s)->pcall(r$ralistI); }
L_
		$l_funcs.= <<L_;
	template <typename AS>
	DR_RINLINE Eslot$xidM<R$tlAlistI> a${i}Set(AS as) const { return new EimplSC_g<AS>((bool (*)())&cAS<AS>::callme$i, *this, as); }
L_
	}
	$l_callers .= <<L_;
	};
L_

	if ($cn < $cmax) {
		$l_funcs .= <<L_;
	template <typename AI>
	DR_RINLINE Eslot$xidP<R$tlAlist, AI> aAdd() const { return *reinterpret_cast<const Eslot$xidP<R$tlAlist, AI> *>(this); } /* simply retype, suppose classic C calling conventions */
L_
	}
	$l_callers .= <<L_;
	struct cRI { static bool callme(EimplS_g *me, void *r_$calalist) { typename typeref<R>::nvtyp r; return ((Eslot$xid_g *)&me->s)->pcall(&r$ralist); } };
L_
	$l_funcs .= <<L_;
	template <typename NR>
	DR_RINLINE Eslot$xid<NR$tlAlist> rIgnore() const { return new EimplS_g((bool (*)())&cRI::callme, *this); }
L_
	$l_callers .= <<L_;
	template <typename NR, typename SNR>
	struct cRS { static bool callme(EimplSC_g<SNR> *me, NR *nr$calalist) { typename typeref<R>::nvtyp r; bool o = ((Eslot$xid_g *)&me->s)->pcall(&r$ralist); *nr = me->c; return o; } };
L_
	$l_funcs .= <<L_;
	template <typename NR, typename SNR>
	DR_RINLINE Eslot$xid<NR$tlAlist> rSet(SNR r) const { return new EimplSC_g<SNR>((bool (*)())&cRS<NR, SNR>::callme, *this, r); }
L_
	$l_callers .= <<L_;
	template <typename NR>
	struct cRR { static bool callme(EimplS_g *me, NR *nr$calalist) { typename typeref<R>::typ r; bool o = ((Eslot$xid_g *)&me->s)->pcall(&r$ralist); *nr = (NR)r; return o; } };
L_
	$l_funcs .= <<L_;
	template <typename NR>
	DR_RINLINE Eslot$xid<NR$tlAlist> rRetype() const { return new EimplS_g((bool (*)())&cRR<NR>::callme, *this); }
L_
	$l_callers .= <<L_;
	template <typename NR$tnNAlist>
	struct cFR { static bool callme(EimplS_g *me, NR *nr$calnalist) { typename typeref<R>::nvtyp r; bool o = ((Eslot$xid<R$tlAlist> *)&me->s)->vcall(&r$convnalist); Epassign(nr, r); return o; } };
L_
	$l_funcs .= <<L_;
	template <typename NR$tnNAlist>
	DR_RINLINE Eslot$xid<NR$tlNAlist> fRetype() const { return new EimplS_g((bool (*)())&cFR<NR$tlNAlist>::callme, *this); }
L_
	$l_callers .= <<L_;
	struct cCS
	{
		static bool callmeLT(EimplSC_g<R> *me, R *r$calalist) { ((Eslot$xid_g *)&me->s)->pcall(r$ralist); return *r < me->c; }
		static bool callmeLE(EimplSC_g<R> *me, R *r$calalist) { ((Eslot$xid_g *)&me->s)->pcall(r$ralist); return *r <= me->c; }
		static bool callmeNE(EimplSC_g<R> *me, R *r$calalist) { ((Eslot$xid_g *)&me->s)->pcall(r$ralist); return *r != me->c; }
		static bool callmeEQ(EimplSC_g<R> *me, R *r$calalist) { ((Eslot$xid_g *)&me->s)->pcall(r$ralist); return *r == me->c; }
		static bool callmeGE(EimplSC_g<R> *me, R *r$calalist) { ((Eslot$xid_g *)&me->s)->pcall(r$ralist); return *r >= me->c; }
		static bool callmeGT(EimplSC_g<R> *me, R *r$calalist) { ((Eslot$xid_g *)&me->s)->pcall(r$ralist); return *r > me->c; }
	};
	template <typename V>
	struct cCG { typedef Eslot2<bool, bool, V> Slot; static bool callme(EimplSC_g<Slot> *me, R *r$calalist) { return me->c(((Eslot$xid_g *)&me->s)->pcall(r$ralist), *r); } };
L_
	$l_funcs .= <<L_;
	DR_RINLINE Eslot$xid<R$tlAlist> checkLT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLT, *this, r); }
	DR_RINLINE Eslot$xid<R$tlAlist> checkLE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeLE, *this, r); }
	DR_RINLINE Eslot$xid<R$tlAlist> checkNE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeNE, *this, r); }
	DR_RINLINE Eslot$xid<R$tlAlist> checkEQ(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeEQ, *this, r); }
	DR_RINLINE Eslot$xid<R$tlAlist> checkGE(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGE, *this, r); }
	DR_RINLINE Eslot$xid<R$tlAlist> checkGT(typename typeref<R>::cref r) { return new EimplSC_g<R>((bool (*)())&cCS::callmeGT, *this, r); }
	template <typename V>
	DR_RINLINE Eslot$xid<R$tlAlist> check(const typename cCG<V>::Slot &s) { return new EimplSC_g<typename cCG<V>::Slot>((bool (*)())&cCG<V>::callme, *this, s); }
L_
	for (my $i = $cn+1; $i <= $cmax; $i++) {
		my $idI = $i-$cn;
		my $xnidI = sprintf("%x", $i);
		my $tnlistI = alistj("typename AA###", $idI, ", ");
		my $tllistI = alist(", AA###", $idI);
		$l_funcs .= <<L_;
	template <$tnlistI>
	DR_RINLINE Eslot$xnidI<R$tlAlist$tllistI> aAdd$idI() { return *reinterpret_cast<const Eslot$xnidI<R$tlAlist$tllistI> *>(this); }
L_
	}
	$hdecl .= <<HD_;
$l_callers$l_funcs};

template <typename R$tnAlist>
DR_RINLINE Eslot$xid<R$tlAlist> Eslot(R (*func)($argjoin), bool vret = true)
{
	return Eslot$xid<R$tlAlist>(func, vret);
}
template <typename O, typename R$tnAlist>
DR_RINLINE Eslot$xid<R$tlAlist> Eslot(O *o, R (O::*meth)($argjoin), bool vret = true)
{
	return Eslot$xid<R$tlAlist>(o, meth, vret);
}
template <typename R$tnAlist, bool logtype>
DR_RINLINE Eslot$xid<R$tlAlist>Eslot(EmitterBase$xid<R$tlAlist, logtype> *e)
{
	return Eslot$xid<R$tlAlist>(e);
}

HD_

	$himpl .= <<HI_;
class DR_PUB Emitter$xid_g: protected Emitter_g
{
protected:
	DR_MINLINE Emitter$xid_g(bool vret_): Emitter_g(vret_) {}
	bool emitAND_g(void *r$vparglist);
	bool emitOR_g(void *r$vparglist);
};

HI_
	$cxxgen .= <<CG_;
bool Emitter${xid_g}::emitAND_g(void *r$vparglist)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot$xid_g *slot = (Eslot$xid_g *)&node->slot;
		if (slot->empty())
			continue;
		if (!slot->pcall(r$alist))
			return false;
	}
	return ret;
}

bool Emitter${xid_g}::emitOR_g(void *r$vparglist)
{
	bool ret = vret();
	Enode_g *node;

	for (node = first(); node; node = node->next) {
		Eslot$xid_g *slot = (Eslot$xid_g *)&node->slot;
		if (slot->empty())
			continue;
		if (slot->pcall(r$alist))
			return true;
	}
	return ret;
}

CG_

	$himpl .= <<HI_;
template <typename R$tnAlist>
class EmitterBaseC$xid: public Emitter$xid_g
{
public:
	typedef Enode$xid<R$tlAlist> Node;

	DR_MINLINE EmitterBaseC$xid(bool vret_): Emitter$xid_g(vret_) {}

	DR_RINLINE Node *connect_begin(const Eslot$xid<R$tlAlist> &r) { return (Node *)insert_g(r); }
	DR_RINLINE Node *connect(const Eslot$xid<R$tlAlist> &r) { return (Node *)append_g(r); }

	template <typename SIA, typename IA>
	DR_RINLINE Node *i0connect(const Eslot1<R, IA> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); } // it's possible to add args simply by ignoring them
HI_
	if ($xid >= 1) {
		$himpl .= <<HI_;
	template <typename SIA, typename IA>
	DR_RINLINE Node *i1connect(const Eslot2<R, IA, A1> &r, SIA i) { return (Node *)insert_g(r.a1Set(i)); }
HI_
	}
	$himpl .= <<HI_;
};

template <typename R$tnAlist, bool logtype>
class EmitterBase$xid: public EmitterBaseC$xid<R$tlAlist>
{
	typedef typename typeref<R>::nvret Rnvtyp;
public:
	DR_RINLINE Rnvtyp emit($argjoin) /* emit, get result only */
	{
		typename typeref<R>::nvtyp r;
		typename typeref<R>::nvtyp *pr = &r;
		logtype?emitOR_g(pr$ralist):emitAND_g(&r$ralist);
		return *pr;
	}
	DR_RINLINE Rnvtyp emitD(Rnvtyp def$arglist) /* emit, get result only (with default ret) */
	{
		logtype?emitOR_g(&def$ralist):emitAND_g(&def$ralist);
		return def;
	}
	DR_RINLINE bool emitI($argjoin) /* emit, check validity only */
	{
		typename typeref<R>::nvtyp r;
		return logtype?emitOR_g(&r$ralist):emitAND_g(&r$ralist);
	}
	DR_RINLINE bool emitC(R *r$arglist) /* emit, check both validity and result */
	{
		return logtype?emitOR_g(r$ralist):emitAND_g(&r$ralist);
	}
	DR_MINLINE EmitterBase$xid(bool vret_): EmitterBaseC$xid<R$tlAlist>(vret_) {}

	DR_RINLINE Eslot$xid<R$tlAlist> slot() { return Eslot$xid<R$tlAlist>(this, &emitC); }
};

template <typename R$tnAlist>
class EmitterAND$xid: public EmitterBase$xid<R$tlAlist, 0>
{
public:
	DR_RINLINE EmitterAND$xid(bool vret_ = true): EmitterBase$xid<R$tlAlist, 0>(vret_) {}
};

template <typename R$tnAlist>
class EmitterOR$xid: public EmitterBase$xid<R$tlAlist, 1>
{
public:
	DR_RINLINE EmitterOR$xid(bool vret_ = false): EmitterBase$xid<R$tlAlist, 1>(vret_) {}
};

template <typename R$tnAlist, bool logtype>
class EslotLog$xid: public Eslot$xid<R$tlAlist>
{
	typedef EimplL_g< EmitterBase$xid<R$tlAlist, logtype> > Log;
	static bool callme(Log *me, typename typeref<R>::ptr r$calalist) { return me->emitC($ajoin); }

public:
	DR_MINLINE EslotLog$xid(bool vret_ = !logtype): Eslot$xid<R$tlAlist>(new Log((bool (*)())&callme, vret_)) {}
	DR_MINLINE EslotLog$xid(const EslotLog$xid &r): Eslot$xid<R$tlAlist>(r) {}

	DR_MINLINE EmitterBase$xid<R$tlAlist, logtype> *emitter() { return (Log *)this->impl; }
};

template <typename R$tnAlist>
class EslotAND$xid: public EslotLog$xid<R$tlAlist, 0>
{
	typedef EslotLog$xid<R$tlAlist, 0> Base;
	typedef EmitterAND$xid<R$tlAlist> Log;
public:
	DR_RINLINE EslotAND$xid(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotAND$xid(const EslotAND$xid &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

template <typename R$tnAlist>
class EslotOR$xid: public EslotLog$xid<R$tlAlist, 1>
{
	typedef EslotLog$xid<R$tlAlist, 1> Base;
	typedef EmitterOR$xid<R$tlAlist> Log;
public:
	DR_RINLINE EslotOR$xid(bool vret_ = true): Base(vret_) {}
	DR_RINLINE EslotOR$xid(const EslotOR$xid &r): Base(r) {}
	DR_RINLINE Log *emitter() { return (Log *)Base::emitter(); }
};

HI_

}

$hfin .= <<HF_;

DR_NS_END

#endif
HF_

$cxxgen .= <<CG_;

DR_NS_END
CG_

$fheader->print($hpre, $hname, $hdecl, $himpl, $hfin);
$fcxx->print($cxxgen);
