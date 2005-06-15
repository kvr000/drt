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
#include <dr/emits.hxx>
#include <dr/Object.hxx>
#include <dr/BString.hxx>
#include <dr/WString.hxx>
#include <dr/Hash.hxx>
#include <dr/Array.hxx>
#include <dr/List.hxx>
#include <dr/ThreadSimple.hxx>
#include <dr/Mutex.hxx>
#include <dr/CastException.hxx>
#include <dr/Ref.hxx>
#include <dr/Throw.hxx>
#include <dr/DummyObject.hxx>

#include <dr/tenv/tenv.hxx>
#include <dr/tenv/TestIdObject.hxx>
#include <dr/tenv/TestIdObjectKV.hxx>

DR_NS_USE
DR_TENV_NS_USE


SintPtr global_counter0;

#define	TEST_OBJECT
#define TEST_ATOMIC
#define TEST_REF
#define TEST_THROW_THROW
#define TEST_EMIT
#define TEST_BSTRING
#define TEST_WSTRING
#define TEST_STRING_CB
#define TEST_STRING_OP
#define TEST_STRING_INDEX
//#define TEST_STRING_THS
//#define TEST_HASH
#define TEST_ARRAY
#define TEST_LIST
#define TEST_THREAD
#define TEST_IFACE

class DR_EXPORT_CLS T0If0: public Iface
{
	DR_IFACE_DECL(T0If0);
};

class DR_EXPORT_CLS T0If1: public Iface
{
	DR_IFACE_DECL(T0If1);
};

class DR_EXPORT_CLS T0If2: public Iface
{
	DR_IFACE_DECL(T0If2);
};

class DR_EXPORT_CLS T0IfD: public T0If1, public T0If2
{
	DR_IFACE_DECL(T0IfD);
};

DR_IFACE_DEF("", T0If0);
DR_IFACE_IMPL_SIMPLE(T0If0);
DR_IFACE_DEF("", T0If1);
DR_IFACE_IMPL_SIMPLE(T0If1);
DR_IFACE_DEF("", T0If2);
DR_IFACE_IMPL_SIMPLE(T0If2);
DR_IFACE_DEF("", T0IfD);
DR_IFACE_IMPL_IFACE2(T0IfD, T0If1, T0If2);


class DR_EXPORT_CLS T0Obj0: public Object, public T0If0, public T0IfD
{
	DR_OBJECT_DECL(T0Obj0, Object);
	DR_REDIR_BEHAV();

public:
	T0Obj0() { alive++; Fatal::plog("T0Obj0 %p: creating\n", this); }
protected:
	~T0Obj0() { alive--; Fatal::plog("T0Obj0 %p: destroying\n", this); }

public:
	static int			alive;
};

int T0Obj0::alive = 0;

DR_OBJECT_DEF("", T0Obj0, Object);
DR_OBJECT_IMPL_IFACE2(T0Obj0, T0If0, T0IfD);


#ifdef TEST_OBJECT
TENV_NS(object);
void test()
{
	TENV_CHECK(TestIdObject::countLiving() == 0);
	{
		ERef<TestIdObject> zer(new TestIdObject(0));
		ERef<TestIdObject> one(new TestIdObject(1));
		ERef<TestIdObject> sec(new TestIdObject(1));
		TENV_CHECK(TestIdObject::countLiving() == 3);
		{
			ERef<Iface> a((Iface *)zer->getIface(Object::comp_name));
			ERef<Iface> b(zer->ref());
			ERef<Iface> c((Iface *)zer->getCheckIface(DummyObject::comp_name)); TENV_CHECK(*c == NULL);
			ERef<Iface> d(zer->getCheckFinal(DummyObject::comp_name)); TENV_CHECK(*d == NULL);
			ERef<Iface> e(zer->getCheckFinal(Object::comp_name)); TENV_CHECK(*e == NULL);
			ERef<Iface> f(zer->getCheckFinal(TestIdObject::comp_name)); TENV_CHECK(*f == zer);
			TENV_CHECK(zer->accCheckFinal(Object::comp_name) == NULL);
			TENV_CHECK(zer->accCheckFinal(DummyObject::comp_name) == NULL);
			TENV_CHECK(zer->accCheckFinal(TestIdObject::comp_name) != NULL);
		}
		TENV_CHECK(TestIdObject::countLiving() == 3);
		{
			TENV_CHECK(zer->eq(one) == false);
			TENV_CHECK(one->eq(sec) == true);
			TENV_CHECK(zer->cmp(sec) < 0);
			TENV_CHECK(one->cmp(sec) == 0);
			TENV_CHECK(one->cmp(zer) > 0);
		}
		TENV_CHECK(TestIdObject::countLiving() == 3);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		ERef<TestIdObjectKV>	o0a(new TestIdObjectKV(0, "a"));
		ERef<TestIdObjectKV>	o0b(new TestIdObjectKV(0, "b"));
		ERef<TestIdObjectKV>	o1a(new TestIdObjectKV(0, "a"));
		ERef<TestIdObjectKV>	o1b(new TestIdObjectKV(0, "b"));
		ERef<TestIdObjectKV::Key>	k0(new TestIdObjectKV::Key(0));
		ERef<TestIdObjectKV::Key>	k1(new TestIdObjectKV::Key(0));
	}
}
TENV_NSE(object);
#endif


#ifdef TEST_ATOMIC
TENV_NS(atomic);
SintPtr ga = -1;
void *glob;
void test()
{
	void *oval, *nval;

	ga = -1;
	Atomic::inc(&ga);
	TENV_CHECK(ga == 0);
	Atomic::dec(&ga);
	TENV_CHECK(ga == -1);
	TENV_CHECK(!Atomic::incr(&ga));
	TENV_CHECK(ga == 0);
	TENV_CHECK(!Atomic::decr(&ga));
	TENV_CHECK(ga == -1);
	ga = 0;
	TENV_CHECK(Atomic::incr(&ga));
	TENV_CHECK(ga == 1);
	TENV_CHECK(Atomic::decr(&ga));
	TENV_CHECK(ga == 0);

	glob = (void *)16; oval = (void *)20; nval = (void *)60;
	TENV_CHECK(!Atomic::cmpxchg(&glob, oval, nval));
	TENV_CHECK(glob == (void *)16);

	glob = (void *)16; oval = (void *)16; nval = (void *)60;
	TENV_CHECK(Atomic::cmpxchg(&glob, oval, nval));
	TENV_CHECK(glob == nval);

	glob = (void *)16; oval = (void *)24; nval = (void *)60;
	TENV_CHECK(Lockp::lock(&glob) == (void *)16);
	TENV_CHECK(glob > (void *)16 && glob < (void *)20);

	Lockp::unlock(&glob, nval);
	TENV_CHECK(glob == nval);
}
TENV_NSE(atomic);
#endif


#ifdef TEST_REF
TENV_NS(ref);

void func(Ref<TestIdObject> t)
{
}

void ifunc(IRef<TestIdObject> t)
{
}

void efunc(ERef<TestIdObject> t)
{
}

int global_x = 0;
void testUsed()
{
	{
		ERef<TestIdObject> hold(TestIdObject::createInstance());
		TENV_CHECK(TestIdObject::countLiving() == 1);
		TENV_CHECK(hold.checkSingleRef());
		{
			Ref<TestIdObject> hold2(hold);
			TENV_CHECK(!hold.checkSingleRef());
		}
			TENV_CHECK(hold.checkSingleRef());
		{
			Ref<TestIdObject> hold2(hold);
			TENV_CHECK(!hold.checkSingleRef());
			hold.setNoref(TestIdObject::createInstance());
			TENV_CHECK(hold.checkSingleRef());
		}
		TENV_CHECK(TestIdObject::countLiving() == 0);
		hold.setNull();
		TENV_CHECK(!hold.checkSingleRef());
		TENV_CHECK(TestIdObject::countLiving() == 1);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}

void test()
{
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		Ref<TestIdObject> hold(TestIdObject::createInstance(), false);
		TENV_CHECK(TestIdObject::countLiving() == 1);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		ERef<TestIdObject> hold(TestIdObject::createInstance());
		TENV_CHECK(TestIdObject::countLiving() == 1);
		ERef<TestIdObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		ERef<TestIdObject> hold(TestIdObject::createInstance());
		TENV_CHECK(TestIdObject::countLiving() == 1);
		IRef<TestIdObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		IRef<TestIdObject> hold(TestIdObject::createInstance(), Noref());
		TENV_CHECK(TestIdObject::countLiving() == 1);
		ERef<TestIdObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		IRef<TestIdObject> hold(TestIdObject::createInstance(), Noref());
		TENV_CHECK(TestIdObject::countLiving() == 1);
		IRef<TestIdObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);

	{
		tref(TestIdObject::createInstance());
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}

TENV_NSE(ref);
#endif


#ifdef TEST_THROW_THROW
TENV_NS(throw_throw);

class DR_EXPORT_CLS T0: public Object
{
	DR_OBJECT_DECL_SIMPLE(T0, Object);

public:
	virtual				~T0();
};

DR_OBJECT_DEF("nt_throw_throw::", T0, Object);
DR_OBJECT_IMPL_SIMPLE(T0);

T0::~T0()
{
	DR_THROWNEW(Exception());
}

void test()
{
	DR_TRY {
		ERef<T0> t(new T0);
		DR_THROWNEW(Exception());
	}
	DR_CATCH (Exception, ex) {
		Fatal::plog("caught expected exception\n");
	}
	DR_ENDTRY

}

TENV_NSE(throw_throw);
#endif

#ifdef TEST_EMIT
TENV_NS(emit);

int rfsig()
{
	Fatal::plog("func: got signal...\n");
	return 7;
}

void fsig()
{
	Fatal::plog("fsig: got signal\n");
}

void fisig(int a)
{
	Fatal::plog("fisig: got signal %d\n", a);
}

double retypesig()
{
	return 6.0;
}

void retypes1(double v)
{
	Fatal::plog("retypes1: got signal %g\n", v);
}

class T1: public Shared
{
public:
	DR_MINLINE virtual		~T1() { }

	DR_MINLINE virtual void		rec(int from, int a)			{ Fatal::plog("T1: got signal %d from %d...\n", a, from); }
	DR_MINLINE void			onef(int from)				{ Fatal::plog("T1: got signal from %d\n", from); }
};

class T2: public T1
{
public:
	DR_MINLINE virtual void rec(int from, int a) { Fatal::plog("T2: got signal %d from %d...\n", a, from); }
};

T1 *f0()
{
	class Tlocal: public T1
	{
		virtual void rec(int from, int a) { Fatal::plog("Tlocal: got signal %d from %d...\n", a, from); }
	};
	return new Tlocal;
}

void test()
{
	T1 *t = f0();
	EmitterAND1<void, int> e0;
	EmitterAND0<int> e1;
	e0.i1connect(Eslot(t, &T1::rec), 7);
	//e0.i1connect(t, &T1::rec, 1);
	//e0.append(Eslot(&fsig)->rIgnore<void>()->aAdd<int>());
	e0.connect(Eslot(&fisig));
	e0.connect(Eslot(&fsig).aAdd<int>());
	e0.connect(Eslot(t, &T1::rec).a1Set(11));
	e0.connect(Eslot(&retypes1).fRetype<void, int>());
	e0.emit(5);
	e0.emit(9);
	e1.connect(Eslot(&fsig).rSet<int>(7).checkGT(8));
	e1.connect(Eslot(&retypesig).rRetype<int>());
	Fatal::plog("e1.emit: %d\n", e1.emitD(0));
	t->unref();
	return;
}

TENV_NSE(emit)
#endif

#ifdef TEST_BSTRING
TENV_NS(bstring);
void test()
{
	const char *s1 = "blasehy", *s2 = "diese";
	TENV_CHECK_SIGN(strcmp(s1, s2), BString(s1).bcmp(BString(s2)));
}
TENV_NSE(bstring)
#endif

#ifdef TEST_WSTRING
TENV_NS(wstring);
void test()
{
	const wchar_t *s1 = L"zlasehy", *s2 = L"diese";
	TENV_CHECK_SIGN(wcscmp(s1, s2), WString(s1).bcmp(WString(s2)));
}
TENV_NSE(wstring)
#endif

#ifdef TEST_STRING_CB
TENV_NS(string_cb);
String sn;
typedef String Str;
struct sss {
	Str c0;
	Str c1;
};
void f(sss *c)
{
	c->c1 = "blasehy";
	c->c0 = c->c1;
	c->c1 = c->c0;
	c->c0 = c->c1;
	c->c0 = c->c1;
}
void test()
{
	sss c;
	f(&c);
}
TENV_NSE(string_cb);
#endif

#ifdef TEST_STRING_OP
TENV_NS(string_op);
void test()
{
	String s;
	TENV_CHECK(String("bla").find("la") == 1);
	TENV_CHECK(String("bla").mid(1, 1) == String("l"));
	s = "krakra";
	s.replace("ra", "raa");
	TENV_CHECK(s == "kraakraa");
}
TENV_NSE(string_op);
#endif

#ifdef TEST_STRING_INDEX
TENV_NS(string_index);
void test()
{
	String ss_bla("bla");
	String ss_sehy("sehy");
	String ss_kra("kra");
	StringIndex si(&ss_bla, &ss_sehy, &ss_kra, NULL);
	TENV_CHECK(si.find(ss_bla) == 0);
	TENV_CHECK(si.find("bla") == 0);
	TENV_CHECK(si.find("sehy") == 1);
	TENV_CHECK(si.find("kra") == 2);
	TENV_CHECK(si.find("") == -1);
	TENV_CHECK(si.find("none") == -1);
	SList<String> list;
	si.appendToList(&list);
	StringIndex vsi(&ss_bla, 8, &ss_sehy, 11, &ss_kra, 3, NULL);
	TENV_CHECK(vsi.find(ss_bla) == 8);
	TENV_CHECK(vsi.find(ss_sehy) == 11);
	TENV_CHECK(vsi.find(ss_kra) == 3);
}
TENV_NSE(string_op);
#endif

#ifdef TEST_STRING_THS
TENV_NS(string_ths);

struct strings
{
	BString c0, c1, c2;
};

#define ASCOUNT (2*1024/**1024*/)

void *t0func(void *arg)
{
	strings *s = (strings *)arg;
	for (int i = 0; i < ASCOUNT; i++) {
#if 1
		switch (rand()%2) {
		case 0:
			s->c0 = "bla";
			break;
		case 1:
			s->c0 = s->c1;
			break;
		}
#endif
		Atomic::inc(&global_counter0);
	}
	return NULL;
}

void *t1func(void *arg)
{
	strings *s = (strings *)arg;
	for (int i = 0; i < ASCOUNT; i++) {
#if 1
		switch (rand()%2) {
		case 0:
			s->c1 = "sehy";
			break;
		case 1:
			s->c1 = s->c2;
			break;
		}
#endif
		Atomic::inc(&global_counter0);
	}
	return NULL;
}

void *t2func(void *arg)
{
	strings *s = (strings *)arg;
	for (int i = 0; i < ASCOUNT; i++) {
#if 1
		switch (rand()%2) {
		case 0:
			s->c2 = "diese";
			break;
		case 1:
			s->c2 = s->c0;
			break;
		}
#endif
		Atomic::inc(&global_counter0);
	}
	return NULL;
}

void test()
{
	strings s;
	Thread *ths[3];

	s.c0.setTsafe(true); s.c0 = "bla";
	s.c1.setTsafe(true); s.c1 = "sehy";
	s.c2.setTsafe(true); s.c2 = "klar";

	global_counter0 = 0;

#if 1
	ths[0] = ThreadSimple::go(&t0func, &s);
	ths[1] = ThreadSimple::go(&t1func, &s);
	ths[2] = ThreadSimple::go(&t2func, &s);

	ths[0]->waitUnref();
	ths[1]->waitUnref();
	ths[2]->waitUnref();
#endif

	Fatal::plog("c0: %s\nc1: %s\nc2: %s\n", s.c0.toStr(), s.c1.toStr(), s.c2.toStr());
	Fatal::plog("%ld\n", (long)global_counter0);
}
TENV_NSE(string_ths);
#endif

#ifdef TEST_HASH
inline long hash(int i) { return i; }
inline long hash(const BString &s) { return s.getHash(); }
TENV_NS(hash);
void test()
{
	Hash<int, int> hi;
	//Hash<BString, int> hs;
	//hs["bla"] = 6;
	//hs["diese"] = 7;
	hi[5] = 9;
	hi[7] = 11;

	TENV_CHECK(hi[5] == 9);
	TENV_CHECK(hi[7] == 11);
	//TENV_CHECK(hs["bla"] == 6);
}
TENV_NSE(hash);
#endif

#ifdef TEST_ARRAY
TENV_NS(array);

void test()
{
	{
		SArray<int> a;

		a.resizeInit(8, 891);
		a[0] = 6; a[1] = 11; a[2] = 1; a[3] = 89; a[4] = 1110; a[5] = 3; a[6] = 2; a[7] = 2;
		a.rsort();
		TENV_CHECK(a[7] == 1);
		TENV_CHECK(a[6] == 2);
		TENV_CHECK(a[5] == 2);
		TENV_CHECK(a[4] == 3);
		TENV_CHECK(a[3] == 6);
		TENV_CHECK(a[2] == 11);
		TENV_CHECK(a[1] == 89);
		TENV_CHECK(a[0] == 1110);
	}
	{
		SArray<BString> s;
		s.resize(4);
		s[3] = "blasehy";
		s[2] = "glasehy";
		s[1] = "diese";
		s[0] = "blar";
		s.sort();
		TENV_CHECK(s[0] == "blar");
		TENV_CHECK(s[1] == "blasehy");
		TENV_CHECK(s[2] == "diese");
		TENV_CHECK(s[3] == "glasehy");
	}

	TENV_CHECK(TestIdObject::countLiving() == 0);
	{
		RArray<TestIdObject> ra;
		ra.appendNoref(new TestIdObject(0));
		ra.appendNoref(new TestIdObject(1));
		ra.appendNoref(new TestIdObject(2));
		ra.appendDoref(tref(new TestIdObject(3)));
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}
TENV_NSE(array);
#endif

#ifdef TEST_LIST
TENV_NS(list);
void test()
{
	{
		TList<int> li;
		li.append(5);
		li.append(8);
		li.insert(3);
		TENV_CHECK(li.removeFirst() == 3);
		TENV_CHECK(li.removeFirst() == 5);
	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
	{
		RList<TestIdObject> li;
		li.append(tref(new TestIdObject(5)));
		li.append(tref(new TestIdObject(8)));
		li.append(tref(new TestIdObject(4)));
		li.insert(tref(new TestIdObject(3)));
		TENV_CHECK(tref(li.removeFirst())->getId() == 3);
		TENV_CHECK(tref(li.removeFirst())->getId() == 5);
		RList<TestIdObject>::Node *n = li.iterFirst();
		TENV_CHECK(n->v->getId() == 8);
		n->v.getAndNull()->unref();
		li.remove(n);

	}
	TENV_CHECK(TestIdObject::countLiving() == 0);
}
TENV_NSE(list);
#endif

#ifdef TEST_THREAD
TENV_NS(thread);
void *t0func(void *arg)
{
	tenv_sleep(1);
	Fatal::plog("t0: %d\n", (int)(SintPtr)arg);
	return (void *)6;
}
void *t1func(void *arg)
{
	Thread *t = (Thread *)arg;
	tenv_sleep(1);
	arg = t->waitUnref();
	Fatal::plog("%p exited with %p\n", t, arg);
	return NULL;
}

void test()
{
	Thread *t0 = ThreadSimple::go(&t0func, (void *)1);
	Thread *t1 = ThreadSimple::go(&t1func, t0->ref());
	t1->unref();
	t0->waitUnref();
	Fatal::plog("waited...\n");
}
TENV_NSE(thread);
#endif

#ifdef TEST_IFACE
TENV_NS(iface);
void test()
{
	bool ok = false;
	String ifd[] = { Object::comp_name, T0Obj0::comp_name, T0If0::iface_name, T0If1::iface_name, T0If2::iface_name, T0IfD::iface_name };
	const char *ifs[] = { "dr::Object", "T0Obj0", "T0If0", "T0If1", "T0If2", "T0IfD" };
	T0Obj0 *t0 = new T0Obj0;

	Fatal::plog("T0If0 size: %ld\n", (long)sizeof(T0If0));
	Fatal::plog("T0If1 size: %ld\n", (long)sizeof(T0If1));
	Fatal::plog("T0If2 size: %ld\n", (long)sizeof(T0If2));
	Fatal::plog("T0IfD size: %ld\n", (long)sizeof(T0IfD));
	Fatal::plog("T0Obj0 size: %ld\n", (long)sizeof(T0Obj0));
	Fatal::plog("Object size: %ld\n", (long)sizeof(Object));

	Fatal::plog("object: %p\n", t0);

	{
		for (size_t i = 0; i < sizeof(ifs)/sizeof(ifs[0]); i++) {
			Iface *ri = (Iface *)t0->getIface(ifd[i]);
			Iface *si = (Iface *)t0->getIface(ifs[i]);
			Fatal::plog("%s: ri: %p, si: %p\n", ifs[i], ri, si);
			ri->unref();
			si->unref();
		}
	}

	{
		t0->ref();
		Fatal::plog("getting non-existing interface\n");
		DR_TRY {
			t0->getIface("non-existing");
		}
		DR_CATCH (CastException, ex) {
			Fatal::plog("caught cast-exception: %s\n", ex->stringify().utf8().toStr());
			ok = true;
		}
		DR_ENDTRY;
		TENV_CHECK_RESET(ok);
		Fatal::plog("getting non-existing interface with unref\n");
		DR_TRY {
			t0->getIfaceUnref("non-existing");
		}
		DR_CATCH (CastException, ex) {
			Fatal::plog("caught cast-exception: %s\n", ex->stringify().utf8().toStr());
			ok = true;
		}
		DR_ENDTRY;
		TENV_CHECK_RESET(ok);
	}

	t0->unref();
	TENV_CHECK(T0Obj0::alive == 0);
}
TENV_NSE(iface);
#endif

DR_TENV_MAIN()
{
	tenv_init();
#ifdef TEST_OBJECT
	TENV_RUN(object);
#endif
#ifdef TEST_ATOMIC
	TENV_RUN(atomic);
#endif
#ifdef TEST_REF
	TENV_RUN(ref);
#endif
#ifdef TEST_THROW_THROW
	TENV_RUN(throw_throw);
#endif
#ifdef TEST_EMIT
	TENV_RUN(emit);
#endif
#ifdef TEST_BSTRING
	TENV_RUN(bstring);
#endif
#ifdef TEST_WSTRING
	TENV_RUN(wstring);
#endif
#ifdef TEST_STRING_CB
	TENV_RUN(string_cb);
#endif
#ifdef TEST_STRING_OP
	TENV_RUN(string_op);
#endif
#ifdef TEST_STRING_INDEX
	TENV_RUN(string_index);
#endif
#ifdef TEST_STRING_THS
	TENV_RUN(string_ths);
#endif
#ifdef TEST_HASH
	TENV_RUN(hash);
#endif
#ifdef TEST_ARRAY
	TENV_RUN(array);
#endif
#ifdef TEST_LIST
	TENV_RUN(list);
#endif
#ifdef TEST_THREAD
	TENV_RUN(thread);
#endif
#ifdef TEST_IFACE
	TENV_RUN(iface);
#endif
	return 0;
}
