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

#include <dr/testenv/testenv.hxx>
#include <dr/testenv/TestObject.hxx>
#include <dr/testenv/TestObjectKV.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


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
TESTNS(object);
void test()
{
	CHECK(TestObject::countLiving() == 0);
	{
		ERef<TestObject> zer(new TestObject(0));
		ERef<TestObject> one(new TestObject(1));
		ERef<TestObject> sec(new TestObject(1));
		CHECK(TestObject::countLiving() == 3);
		{
			ERef<Iface> a((Iface *)zer->getIface(Object::comp_name));
			ERef<Iface> b(zer->ref());
			ERef<Iface> c((Iface *)zer->getCheckIface(DummyObject::comp_name)); CHECK(*c == NULL);
			ERef<Iface> d(zer->getCheckFinal(DummyObject::comp_name)); CHECK(*d == NULL);
			ERef<Iface> e(zer->getCheckFinal(Object::comp_name)); CHECK(*e == NULL);
			ERef<Iface> f(zer->getCheckFinal(TestObject::comp_name)); CHECK(*f == zer);
			CHECK(zer->accCheckFinal(Object::comp_name) == NULL);
			CHECK(zer->accCheckFinal(DummyObject::comp_name) == NULL);
			CHECK(zer->accCheckFinal(TestObject::comp_name) != NULL);
		}
		CHECK(TestObject::countLiving() == 3);
		{
			CHECK(zer->eq(one) == false);
			CHECK(one->eq(sec) == true);
			CHECK(zer->cmp(sec) < 0);
			CHECK(one->cmp(sec) == 0);
			CHECK(one->cmp(zer) > 0);
		}
		CHECK(TestObject::countLiving() == 3);
	}
	CHECK(TestObject::countLiving() == 0);

	{
		ERef<TestObjectKV>	o0a(new TestObjectKV(0, "a"));
		ERef<TestObjectKV>	o0b(new TestObjectKV(0, "b"));
		ERef<TestObjectKV>	o1a(new TestObjectKV(0, "a"));
		ERef<TestObjectKV>	o1b(new TestObjectKV(0, "b"));
		ERef<TestObjectKV::Key>	k0(new TestObjectKV::Key(0));
		ERef<TestObjectKV::Key>	k1(new TestObjectKV::Key(0));
	}
}
TESTNSE(object);
#endif


#ifdef TEST_ATOMIC
TESTNS(atomic);
SintPtr ga = -1;
void *glob;
void test()
{
	void *oval, *nval;

	ga = -1;
	Atomic::inc(&ga);
	CHECK(ga == 0);
	Atomic::dec(&ga);
	CHECK(ga == -1);
	CHECK(!Atomic::incr(&ga));
	CHECK(ga == 0);
	CHECK(!Atomic::decr(&ga));
	CHECK(ga == -1);
	ga = 0;
	CHECK(Atomic::incr(&ga));
	CHECK(ga == 1);
	CHECK(Atomic::decr(&ga));
	CHECK(ga == 0);

	glob = (void *)16; oval = (void *)20; nval = (void *)60;
	CHECK(!Atomic::cmpxchg(&glob, oval, nval));
	CHECK(glob == (void *)16);

	glob = (void *)16; oval = (void *)16; nval = (void *)60;
	CHECK(Atomic::cmpxchg(&glob, oval, nval));
	CHECK(glob == nval);

	glob = (void *)16; oval = (void *)24; nval = (void *)60;
	CHECK(Lockp::lock(&glob) == (void *)16);
	CHECK(glob > (void *)16 && glob < (void *)20);

	Lockp::unlock(&glob, nval);
	CHECK(glob == nval);
}
TESTNSE(atomic);
#endif


#ifdef TEST_REF
TESTNS(ref);

void func(Ref<TestObject> t)
{
}

void ifunc(IRef<TestObject> t)
{
}

void efunc(ERef<TestObject> t)
{
}

int global_x = 0;
void testUsed()
{
	{
		ERef<TestObject> hold(TestObject::createInstance());
		CHECK(TestObject::countLiving() == 1);
		CHECK(hold.checkSingleRef());
		{
			Ref<TestObject> hold2(hold);
			CHECK(!hold.checkSingleRef());
		}
			CHECK(hold.checkSingleRef());
		{
			Ref<TestObject> hold2(hold);
			CHECK(!hold.checkSingleRef());
			hold.setNoref(TestObject::createInstance());
			CHECK(hold.checkSingleRef());
		}
		CHECK(TestObject::countLiving() == 0);
		hold.setNull();
		CHECK(!hold.checkSingleRef());
		CHECK(TestObject::countLiving() == 1);
	}
	CHECK(TestObject::countLiving() == 0);
}

void test()
{
	CHECK(TestObject::countLiving() == 0);

	{
		Ref<TestObject> hold(TestObject::createInstance(), false);
		CHECK(TestObject::countLiving() == 1);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	CHECK(TestObject::countLiving() == 0);

	{
		ERef<TestObject> hold(TestObject::createInstance());
		CHECK(TestObject::countLiving() == 1);
		ERef<TestObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	CHECK(TestObject::countLiving() == 0);

	{
		ERef<TestObject> hold(TestObject::createInstance());
		CHECK(TestObject::countLiving() == 1);
		IRef<TestObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	CHECK(TestObject::countLiving() == 0);

	{
		IRef<TestObject> hold(TestObject::createInstance(), Noref());
		CHECK(TestObject::countLiving() == 1);
		ERef<TestObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	CHECK(TestObject::countLiving() == 0);

	{
		IRef<TestObject> hold(TestObject::createInstance(), Noref());
		CHECK(TestObject::countLiving() == 1);
		IRef<TestObject> hold2(hold);
		func(hold);
		ifunc(hold);
		efunc(hold);
	}
	CHECK(TestObject::countLiving() == 0);

	{
		tref(TestObject::createInstance());
	}
	CHECK(TestObject::countLiving() == 0);
}

TESTNSE(ref);
#endif


#ifdef TEST_THROW_THROW
TESTNS(throw_throw);

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

TESTNSE(throw_throw);
#endif

#ifdef TEST_EMIT
TESTNS(emit);

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

TESTNSE(emit)
#endif

#ifdef TEST_BSTRING
TESTNS(bstring);
void test()
{
	const char *s1 = "blasehy", *s2 = "diese";
	CHECK_SIGN(strcmp(s1, s2), BString(s1).bcmp(BString(s2)));
}
TESTNSE(bstring)
#endif

#ifdef TEST_WSTRING
TESTNS(wstring);
void test()
{
	const wchar_t *s1 = L"zlasehy", *s2 = L"diese";
	CHECK_SIGN(wcscmp(s1, s2), WString(s1).bcmp(WString(s2)));
}
TESTNSE(wstring)
#endif

#ifdef TEST_STRING_CB
TESTNS(string_cb);
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
TESTNSE(string_cb);
#endif

#ifdef TEST_STRING_OP
TESTNS(string_op);
void test()
{
	String s;
	CHECK(String("bla").find("la") == 1);
	CHECK(String("bla").mid(1, 1) == String("l"));
	s = "krakra";
	s.replace("ra", "raa");
	CHECK(s == "kraakraa");
}
TESTNSE(string_op);
#endif

#ifdef TEST_STRING_INDEX
TESTNS(string_index);
void test()
{
	String ss_bla("bla");
	String ss_sehy("sehy");
	String ss_kra("kra");
	StringIndex si(&ss_bla, &ss_sehy, &ss_kra, NULL);
	CHECK(si.find(ss_bla) == 0);
	CHECK(si.find("bla") == 0);
	CHECK(si.find("sehy") == 1);
	CHECK(si.find("kra") == 2);
	CHECK(si.find("") == -1);
	CHECK(si.find("none") == -1);
	SList<String> list;
	si.appendToList(&list);
	StringIndex vsi(&ss_bla, 8, &ss_sehy, 11, &ss_kra, 3, NULL);
	CHECK(vsi.find(ss_bla) == 8);
	CHECK(vsi.find(ss_sehy) == 11);
	CHECK(vsi.find(ss_kra) == 3);
}
TESTNSE(string_op);
#endif

#ifdef TEST_STRING_THS
TESTNS(string_ths);

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
TESTNSE(string_ths);
#endif

#ifdef TEST_HASH
inline long hash(int i) { return i; }
inline long hash(const BString &s) { return s.getHash(); }
TESTNS(hash);
void test()
{
	Hash<int, int> hi;
	//Hash<BString, int> hs;
	//hs["bla"] = 6;
	//hs["diese"] = 7;
	hi[5] = 9;
	hi[7] = 11;

	CHECK(hi[5] == 9);
	CHECK(hi[7] == 11);
	//CHECK(hs["bla"] == 6);
}
TESTNSE(hash);
#endif

#ifdef TEST_ARRAY
TESTNS(array);

void test()
{
	{
		SArray<int> a;

		a.resizeInit(8, 891);
		a[0] = 6; a[1] = 11; a[2] = 1; a[3] = 89; a[4] = 1110; a[5] = 3; a[6] = 2; a[7] = 2;
		a.rsort();
		CHECK(a[7] == 1);
		CHECK(a[6] == 2);
		CHECK(a[5] == 2);
		CHECK(a[4] == 3);
		CHECK(a[3] == 6);
		CHECK(a[2] == 11);
		CHECK(a[1] == 89);
		CHECK(a[0] == 1110);
	}
	{
		SArray<BString> s;
		s.resize(4);
		s[3] = "blasehy";
		s[2] = "glasehy";
		s[1] = "diese";
		s[0] = "blar";
		s.sort();
		CHECK(s[0] == "blar");
		CHECK(s[1] == "blasehy");
		CHECK(s[2] == "diese");
		CHECK(s[3] == "glasehy");
	}

	CHECK(TestObject::countLiving() == 0);
	{
		RArray<TestObject> ra;
		ra.appendNoref(new TestObject(0));
		ra.appendNoref(new TestObject(1));
		ra.appendNoref(new TestObject(2));
		ra.appendDoref(tref(new TestObject(3)));
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(array);
#endif

#ifdef TEST_LIST
TESTNS(list);
void test()
{
	{
		TList<int> li;
		li.append(5);
		li.append(8);
		li.insert(3);
		CHECK(li.removeFirst() == 3);
		CHECK(li.removeFirst() == 5);
	}
	CHECK(TestObject::countLiving() == 0);
	{
		RList<TestObject> li;
		li.append(tref(new TestObject(5)));
		li.append(tref(new TestObject(8)));
		li.append(tref(new TestObject(4)));
		li.insert(tref(new TestObject(3)));
		CHECK(tref(li.removeFirst())->getId() == 3);
		CHECK(tref(li.removeFirst())->getId() == 5);
		RList<TestObject>::Node *n = li.iterFirst();
		CHECK(n->v->getId() == 8);
		n->v.getAndNull()->unref();
		li.remove(n);

	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(list);
#endif

#ifdef TEST_THREAD
TESTNS(thread);
void *t0func(void *arg)
{
	test_sleep(1);
	Fatal::plog("t0: %d\n", (int)(SintPtr)arg);
	return (void *)6;
}
void *t1func(void *arg)
{
	Thread *t = (Thread *)arg;
	test_sleep(1);
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
TESTNSE(thread);
#endif

#ifdef TEST_IFACE
TESTNS(iface);
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
		CHECK_RESET(ok);
		Fatal::plog("getting non-existing interface with unref\n");
		DR_TRY {
			t0->getIfaceUnref("non-existing");
		}
		DR_CATCH (CastException, ex) {
			Fatal::plog("caught cast-exception: %s\n", ex->stringify().utf8().toStr());
			ok = true;
		}
		DR_ENDTRY;
		CHECK_RESET(ok);
	}

	t0->unref();
	CHECK(T0Obj0::alive == 0);
}
TESTNSE(iface);
#endif

DR_TESTENV_MAIN()
{
	test_init();
#ifdef TEST_OBJECT
	TESTRUN(object);
#endif
#ifdef TEST_ATOMIC
	TESTRUN(atomic);
#endif
#ifdef TEST_REF
	TESTRUN(ref);
#endif
#ifdef TEST_THROW_THROW
	TESTRUN(throw_throw);
#endif
#ifdef TEST_EMIT
	TESTRUN(emit);
#endif
#ifdef TEST_BSTRING
	TESTRUN(bstring);
#endif
#ifdef TEST_WSTRING
	TESTRUN(wstring);
#endif
#ifdef TEST_STRING_CB
	TESTRUN(string_cb);
#endif
#ifdef TEST_STRING_OP
	TESTRUN(string_op);
#endif
#ifdef TEST_STRING_INDEX
	TESTRUN(string_index);
#endif
#ifdef TEST_STRING_THS
	TESTRUN(string_ths);
#endif
#ifdef TEST_HASH
	TESTRUN(hash);
#endif
#ifdef TEST_ARRAY
	TESTRUN(array);
#endif
#ifdef TEST_LIST
	TESTRUN(list);
#endif
#ifdef TEST_THREAD
	TESTRUN(thread);
#endif
#ifdef TEST_IFACE
	TESTRUN(iface);
#endif
	return 0;
}
