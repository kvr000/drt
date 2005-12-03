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

#ifndef dr__String_c__hxx__
# define dr__String_c__hxx__

#include <dr/def_core.hxx>
#include <dr/types.hxx>
#include <dr/Assert.hxx>
#include <dr/Atomic.hxx>
#include <dr/Alloc.hxx>
#include <dr/char_traits.hxx>

DR_NS_BEGIN


class DR_PUB String;

template <typename Subtype>
struct DR_PUB StrData_c;
template <typename Subtype>
class DR_PUB String_c;

template <typename Subtype>
struct DR_PUB StrData_c
{
	typedef typename Subtype::type		TC;

	struct cont: public Subtype {
		union {
			TC		s[(sizeof(Aligned)+sizeof(TC)-1)/sizeof(TC)];		// string
			Aligned		alig_[(sizeof(TC)+sizeof(Aligned)-1)/sizeof(Aligned)];// align it
		};
		DR_RINLINE		cont()					: Subtype() { }
		DR_RINLINE		cont(Invalid *v)			: Subtype(v) { s[0] = '\0'; }	// used to init null/zero members
	};

	Refcnt				refcnt;		// ref-count
	size_t				size;		// length
	ssize_t				al;		// allocated
	long				hash;		// hash

	Aligned				data[(sizeof(cont)+sizeof(Aligned)-1)/sizeof(Aligned)]; // memory containing cont (must not have constructor)

	DR_RINLINE void			subdestroy()				{ sub()->Subtype::destroy(); }
	DR_RINLINE void			changed()				{ hash = -1; sub()->Subtype::changed(); }
	long				updateHash();

	DR_RINLINE StrData_c *		ref()					{ Atomic::inc(&refcnt); return this; }

	DR_RINLINE cont *		con()					{ return reinterpret_cast<cont *>((void *)&data[0]); }
	DR_RINLINE Subtype *		sub()					{ return reinterpret_cast<cont *>((void *)&data[0]); }
	DR_RINLINE TC *			str()					{ return reinterpret_cast<cont *>((void *)&data[0])->s; }
	DR_RINLINE static size_t	roundsize(size_t size)			{ return ((size*sizeof(TC)+sizeof(long)-1)&-(ssize_t)sizeof(long))/sizeof(TC); }
	DR_RINLINE static size_t	compsize(size_t al)			{ return ((size_t)&((StrData_c *)NULL)->str()[al]+sizeof(long)-1)&~(sizeof(long)-1); }
	DR_RINLINE size_t		compsize()				{ return compsize(al); }
	DR_RINLINE void			padzeroes();

	static class DR_PUB inistatic { public: inistatic(); ~inistatic(); } ini;

	static StrData_c<Subtype>	null;
	static StrData_c<Subtype>	zero;

private:
	template <typename SubtypeT_>
	friend class String_c;
	friend class String;
};

template <typename Subtype>
class DR_PUB String_c
{
public:
	typedef typename Subtype::type	TC;
	typedef StrData_c<Subtype>	Data;

public:
	DR_RINLINE			String_c()				{ initD(&Data::zero); }
	DR_RINLINE			String_c(Invalid *)			: d(NULL) { }
	DR_RINLINE			String_c(const Null &)			{ initD(&Data::null); }
	DR_RINLINE			String_c(const String_c &s)		{ d = s.refD(); }
	DR_RINLINE			String_c(const TC *s)			{ initZ(s); }
	DR_RINLINE			String_c(const TC *s, size_t size)	{ initLen(s, size); }
	DR_RINLINE explicit		String_c(Data *sd)			{ d = sd; }
	DR_RINLINE			~String_c()							{ if (!Atomic::decr(&d->refcnt)) freeD(); DR_DCODE(d = (Data *)16); }

public:
	String_c &			setNull();
	String_c &			setEmpty();
	DR_RINLINE String_c &		invalidSet();
	DR_RINLINE String_c &		invalidUnset();
	DR_RINLINE String_c &		operator=(const TC *s)			{ copyZ(s); return *this; }
	String_c &			operator=(const String_c &s);

	DR_RINLINE			operator const TC *() const		{ return d->str(); }
	DR_RINLINE const TC *		toStr() const				{ return d->str(); }
	DR_RINLINE const TC *		toNStr() const				{ return (d->al == -1)?NULL:d->str(); }

	DR_RINLINE int			getCont() const				{ ssize_t cont = d->al; return (cont > 0)?1:(int)cont; }
	DR_RINLINE bool			isNull() const				{ return getCont() < 0; }
	DR_RINLINE bool			isEmpty() const				{ return getCont() == 0; }
	DR_RINLINE bool			isNullEmpty() const			{ return getCont() <= 0; }
	DR_RINLINE bool			invalidCheck() const			{ return d == NULL; }
	DR_RINLINE size_t		getSize() const				{ return d->size; }
	int				bcmp(const String_c &s) const;
	bool				beq(const String_c &s) const;
	bool				beq(const TC *s) const;
	bool				sdeq(const String_c &s) const		{ return d == s.d; }
	DR_RINLINE bool			operator==(const String_c &s) const	{ return beq(s); }
	DR_RINLINE bool			operator==(const TC *s) const		{ return beq(s); }
	DR_RINLINE bool			operator!=(const String_c &s) const	{ return !beq(s); }
	DR_RINLINE bool			operator!=(const TC *s) const		{ return !beq(s); }

	/**
	 * lock the string setting the new size
	 */
	TC *				lock(size_t size = getSize());

	/**
	 * lock the string keeping the current size
	 */
	TC *				lockKeep(size_t size);

	/**
	 * unlock the string keeping the old size
	 */
	DR_RINLINE void			unlock()				{ DR_Assert(d->refcnt == 0); }

	/**
	 * unlock the string setting the new size
	 */
	void				unlock(size_t size);

	/**
	 * unlock the string extending the size
	 */
	void				unlockAppend(size_t size);

	/**
	 * unlock the string setting the new size by searching terminating zero
	 */
	void				unlockZ();

	long				getHash() const;

	String_c			operator+(const String_c &s) const;
	String_c &			append(const String_c &s);
	String_c &			append(const TC *str, size_t size);
	String_c &			append(const TC *str);
	String_c &			appendAscii(const char *str, size_t size);
	String_c &			appendChar(TC c);
	String_c &			appendReverse(const TC *str, size_t size);
	String_c &			appendNumber(Sint32 number, int base = 10);
	String_c &			appendNumber(Uint32 number, int base = 10);
	String_c &			appendNumber(Sint64 number, int base = 10);
	String_c &			appendNumber(Uint64 number, int base = 10);
	String_c &			appendNumber(double number);

	DR_MINLINE String_c &		operator+=(const String_c &s)		{ return append(s); }
	DR_MINLINE String_c &		operator+=(const TC *s)			{ return append(s); }

	String_c			left(size_t length) const;
	String_c			mid(size_t pos) const;
	String_c			mid(size_t pos, size_t length) const;
	String_c			right(size_t size) const;

	void				removeLeft(size_t size);
	void				removeMid(size_t pos);
	void				removeMid(size_t pos, size_t size);
	void				removeRight(size_t size);
	String_c			cleanGet();
	String_c			removeLeftGet(size_t size);
	String_c			removeMidGet(size_t pos);
	String_c			removeMidGet(size_t pos, size_t size);
	String_c			removeRightGet(size_t size);

	/**
	 * removes all surrounding space at the beginning and end of the string
	 */
	void				trimSpaces();

	/**
	 * Skips the spaces starting at position pos
	 *
	 * @return the length of the string
	 * 	in case there is only whitespace
	 * @return the position of the first non-space character
	 *	in case there is any
	 */
	ssize_t				skipSpaces(size_t pos);

	ssize_t				findChar(TC chr) const;
	ssize_t				findChar(TC chr, size_t start_pos) const;
	ssize_t				rfindChar(TC chr, size_t before_pos) const;

	ssize_t				find(const String_c &search) const;
	ssize_t				find(const String_c &search, size_t start_pos) const;
	ssize_t				rfind(const String_c &search, size_t before_pos) const;

	static String_c			createNumber(Sint64 number, int base = 10);
	static String_c			createNumber(Uint64 number, int base = 10);
	static String_c			createNumber(double number);

public:
	void				copyS(const String_c &s);
	void				copyZ(const TC *s);
	void				copyLen(const TC *s, size_t len);

protected:
	DR_RINLINE void			initD(Data *s)				{ Atomic::inc(&s->refcnt); d = s; }
	void				initS(const String_c &s);
	void				initZ(const TC *s);
	void				initLen(const TC *s, size_t len);

	static Data *			createDgLen(const TC *s, size_t len);

protected:
	static Data *			allocDg(size_t size);
	static void			freeDg(Data *s);
	void				freeD() const;
	DR_RINLINE static Data *	refDg(Data *s)				{ Atomic::inc(&s->refcnt); return s; }
	DR_RINLINE static void		unrefDg(Data *s)			{ if (!Atomic::decr(&s->refcnt)) freeDg(s); }

	void				reallocSelf(size_t newcsize)		{ if (d->refcnt != 0 || newcsize >= (size_t)d->al) reallocSelfDo(newcsize); }
	void				reallocSelfDo(size_t newcsize);

	DR_RINLINE void			i_setDoref(Data *s)			{ if (!Atomic::incdecr(&s->refcnt, &d->refcnt)) freeD(); d = s; }
	DR_RINLINE void			i_setNoref(Data *s)			{ if (!Atomic::decr(&d->refcnt)) freeD(); d = s; }
	DR_RINLINE Data *		i_refD() const				{ return d->ref(); }
	DR_RINLINE Data *		refD() const				{ Atomic::inc(&d->refcnt); return d; }

protected:
	Data *				d;

private:
	template <typename Subtype_t>
	friend struct StrData_c;
	friend struct StrData;
	friend class String;
};


DR_NS_END

template <typename Subtype>
DR_RINLINE long hash(const DR_NSP(String_c)<Subtype> &s)
{
	return s.getHash();
}

#include <dr/cmp.hxx>

DR_NS_BEGIN


template <typename SubtypeT>
class Compar<String_c<SubtypeT> >
{
	typedef String_c<SubtypeT> Str;
public:
	static DR_RINLINE bool		teq(const Str &t1, const Str &t2)	{ return t1 == t2; }
	static DR_RINLINE int		tcmp(const Str &t1, const Str &t2)	{ return t1.bcmp(t2); }
	static DR_RINLINE void		txchg(Str *t1, Str *t2)			{ Str t(*t1); *t1 = *t2; *t2 = t; }
};


DR_NS_END

#endif
