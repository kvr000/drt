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

#ifndef dr__String__hxx__
# define dr__String__hxx__

#include <stdarg.h>

#include <dr/BString.hxx>
#include <dr/WString.hxx>

DR_NS_BEGIN


struct DR_PUB StrData
{
public:
	enum {
		Bbin		= 1,			/**< bytes binary */
		Binv		= 2,			/**< bytes invalid */
		Wprf		= 4,			/**< wide prefer */
		Winv		= 8,			/**< wide invalid */
	};
public:
	Refcnt				refcnt;
	size_t				length;
	long				hash;

	BString::Data *			bytes;
	WString::Data *			wide;

public:
	long				updateHash();

public:
	static StrData *		allocAscii(size_t);
	static StrData *		allocBinary(size_t);
	static StrData *		allocWide(size_t);

	StrData *			addAsciiSpace(size_t add);
	StrData *			addWideSpace(size_t add);
	StrData *			makeAsciiSpace(size_t newlen);
	StrData *			makeWideSpace(size_t newlen);

	void				destroy();

public:
	DR_RINLINE int			getFlags()				{ return (((UintPtr)bytes&3)+(((UintPtr)wide&3)<<2)); }
	DR_RINLINE BString::Data *	b_get()					{ return (BString::Data *)((UintPtr)bytes&-4); }
	DR_RINLINE BString::Data *	b_real(BString::Data *b)		{ return (BString::Data *)((UintPtr)b&-4); }
	DR_RINLINE char *		b_str()					{ return ((BString::Data *)((UintPtr)bytes&-4))->str(); }
	DR_RINLINE WString::Data *	w_get()					{ return (WString::Data *)((UintPtr)wide&-4); }
	DR_RINLINE WString::Data *	w_real(WString::Data *w)		{ return (WString::Data *)((UintPtr)w&-4); }
	DR_RINLINE wchar_t *		w_str()					{ return ((WString::Data *)((UintPtr)(void *)wide&-4))->str(); }
	DR_RINLINE bool			b_ok()					{ return !((UintPtr)bytes&2); }
	DR_RINLINE bool			w_ok()					{ return !((UintPtr)wide&2); }
	DR_RINLINE bool			b_ok(BString::Data *b)			{ return !((UintPtr)b&2); }
	DR_RINLINE bool			w_ok(WString::Data *w)			{ return !((UintPtr)w&2); }

	DR_RINLINE void			b_init(BString::Data *b, UintPtr fl)	{ bytes = (BString::Data *)((UintPtr)b|fl); }
	DR_RINLINE void			w_init(WString::Data *w, UintPtr fl)	{ wide = (WString::Data *)((UintPtr)w|fl/4); }

	DR_RINLINE void			b_setlen(size_t newlen)			{ b_get()->size = newlen; b_str()[newlen] = '\0'; }
	DR_RINLINE void			w_setlen(size_t newlen)			{ w_get()->size = newlen; w_str()[newlen] = L'\0'; }

public:
	BString::Data *			refByte();
	void				updateWide();
	void				updateByte();

public:
	StrData *			append(StrData *sd);
	StrData *			appendAscii(const char *asciistr, size_t slength);
	StrData *			appendWide(const wchar_t *widestr, size_t slength);
	StrData *			appendAsciiReverse(const char *ascii, size_t slength);
	StrData *			appendAsciiChars(char ascii, size_t count);
	StrData *			appendWideChars(wchar_t ascii, size_t count);
	StrData *			vformatAscii(const char *asciifmt, va_list args);
	StrData *			vformatWide(const char *widefmt, va_list args);

public:
	bool				beq(StrData *sd);
	int				bcmp(StrData *sd);

public:
	DR_RINLINE StrData *		ref()					{ Atomic::inc(&refcnt); return this; }
	DR_RINLINE void			unref()					{ if (!Atomic::decr(&refcnt)) destroy(); }

public:
	static StrData			null;
	static StrData			zero;

public:
	friend class String;
};

/**
 * String
 *
 * Supports both byte and unicode representations.
 */
class DR_PUB String: public Base
{
public:
	typedef StrData			Data;

public:
	DR_RINLINE			String()				{ d = StrData::zero.ref(); }
	DR_RINLINE			String(const Null &)			{ d = StrData::null.ref(); }
	DR_RINLINE			String(const String &s)			{ d = s.refD(); }
	/* constructor */		String(const char *s, ssize_t len = -1);
	/* constructor */		String(const wchar_t *s, ssize_t len = -1);
	DR_RINLINE			String(StrData *sd)			{ d = sd; }
	DR_RINLINE			~String()				{ if (!Atomic::decr(&d->refcnt)) d->destroy(); DR_DCODE(d = (StrData *)64); }

public:
	static String			createUtf8(const BString &str);
	static String			createUtf8(const char *utf8, size_t size);
	static String			createLowerUtf8(const char *utf8, size_t size);
	static String			createUpperUtf8(const char *utf8, size_t size);
	static String			createLower(const String &str);
	static String			createUpper(const String &str);
	static String			createNumber(Sint32 value);
	static String			createNumber(Uint32 value);
	static String			createNumber(Sint64 value);
	static String			createNumber(Uint64 value);
	static String			createNumber(double value);


public:
	BString				ascii() const;
	WString				wide() const;
	BString				utf8() const;

public:
	String &			operator=(const String &s);
	String &			operator=(const char *ascii);
	String &			operator=(const wchar_t *wide);

public:
	String &			setNull();
	String &			setEmpty();
	String &			setAscii(const char *ascii, ssize_t length = -1);
	String &			setUtf8(const char *utf8, ssize_t length = -1);
	String &			setUtf8(const BString &str);
	String &			setWide(const wchar_t *wide, ssize_t length = -1);

public:
	String &			insert(ssize_t pos, const String &s);
	String &			insert(ssize_t pos, const BString &s);
	String &			insert(ssize_t pos, const WString &s);
	String &			append(const String &s);
	String &			append(const BString &s);
	String &			append(const WString &s);
	String &			append(const char *ascii, ssize_t length = -1);
	String &			append(const wchar_t *wide, ssize_t length = -1);
	String &			appendUtf8(const char *utf8, ssize_t length = -1);
	String &			appendAscii(const char *str, size_t size);
	String &			appendAsciiReverse(const char *str, size_t size);
	String &			appendNumber(Sint32 number, int base = 10);
	String &			appendNumber(Uint32 number, int base = 10);
	String &			appendNumber(Sint64 number, int base = 10);
	String &			appendNumber(Uint64 number, int base = 10);
	String &			appendPointer(const void *ptr);
	String &			appendDump(const void *mem, size_t size);

	String				left(size_t length) const;
	String				right(size_t from) const;
	String				mid(size_t from) const;
	String				mid(size_t from, size_t length) const;

	ssize_t				find(const String &s) const;
	ssize_t				find(const String &s, size_t start) const;
	ssize_t				rfind(const String &s) const;
	ssize_t				rfind(const String &s, size_t before) const;

	String &			replace(const String &what, const String &repl);
	String &			replace(const String &what, const String &repl, size_t count);

	String &			format(const char *asciifmt, ...);
	String &			vformat(const char *asciifmt, va_list args);
	static String			formats(const char *asciifmt, ...);
	static String			vformats(const char *asciifmt, va_list args);

	String &			wformat(const wchar_t *widefmt, ...);
	String &			vwformat(const wchar_t *widefmt, va_list args);
	static String			wformats(const wchar_t *widefmt, ...);
	static String			vwformats(const wchar_t *widefmt, va_list args);

public:
	char *				lockAscii(size_t length);
	wchar_t *			lockWide(size_t length);
	void				unlock(size_t length);
	void				unlockAuto();

public:
	DR_RINLINE size_t		getLength() const			{ return d->length; }
	DR_RINLINE long			getHash() const				{ return d->hash >= 0 ? d->hash : d->updateHash(); }

public:
	bool				isNull() const;
	bool				isEmpty() const;
	bool				beq(const String &s) const;
	int				bcmp(const String &s) const;
	bool				eqUtf8(const char *str, size_t size);
	DR_RINLINE bool			operator==(const String &s) const	{ return beq(s); }
	DR_RINLINE bool			operator!=(const String &s) const	{ return !beq(s); }

public:
	String				operator+(const String &s) const;
	String				operator+(const char *ascii) const;
	String				operator+(const wchar_t *wide) const;

	DR_RINLINE String &		operator+=(const String &s)		{ return append(s); }
	DR_RINLINE String &		operator+=(const char *s)		{ return append(s); }
	DR_RINLINE String &		operator+=(const wchar_t *s)		{ return append(s); }

protected:
	DR_RINLINE String &		i_append(const String &s)				{ d = d->append(s.d); return *this; }
	DR_RINLINE String &		i_appendAscii(const char *str, size_t size)		{ d = d->appendAscii(str, size); return *this; }
	DR_RINLINE String &		i_appendAsciiReverse(const char *str, size_t size)	{ d = d->appendAsciiReverse(str, size); return *this; }

protected:
	DR_RINLINE void			i_setDoref(StrData *s)			{ if (!Atomic::incdecr(&s->refcnt, &d->refcnt)) d->destroy(); d = s; }
	DR_RINLINE void			i_setNoref(StrData *s)			{ if (!Atomic::decr(&d->refcnt)) d->destroy(); d = s; }
	DR_RINLINE StrData *		i_refD() const				{ return d->ref(); }
	DR_RINLINE StrData *		refD() const				{ return d->ref(); }

protected:
	StrData *			d;

private:
	friend class Object;
	friend class StringIndex;
};

template <typename V> class SList;

class DR_PUB StringIndex: public Base
{
public:
	/* constructor */		StringIndex(size_t count, const String * const*strs);
	/* constructor */		StringIndex(size_t count, const String *strs);
	/* constructor */		StringIndex(const String *str0, const String *str1, ...);
	/* constructor */		StringIndex(size_t count, const char *const *strs);
	/* constructor */		StringIndex(const char *str0, ...);
	/* constructor */		StringIndex(size_t count, const wchar_t *const *strs);
	/* constructor */		StringIndex(const wchar_t *str0, ...);
	/* constructor */		StringIndex(const String *str0, long value, ...);

public:
	/* destructor */		~StringIndex();

public:
	bool				isEmpty()				{ return hash_list == (element **)(void *)&hash_mask; }

public:
	long				find(const String &s) const;
	long				findXchg(const String &s);

	void *				findPtr(const String &s) const;
	void *				findPtrXchg(const String &s);

public:
	void				reinitArraySized(size_t count, const char *const *list, size_t el_size);
	void				reinitArraySizedConst(size_t count, const char *const *list, size_t el_size);

public:
	void				appendToList(SList<String> *list);

protected:
	void				init(size_t count, const String * const*strs);
	void				init(size_t count, const String * const*strs, const long *values);
	void				clean();

protected:
	struct element
	{
		long			index;
		String			str;
		element *		next;
		/* constructor */	element(long i, const String &s, element *n): index(i), str(s), next(n) {}
	} **				hash_list;
	UintPtr				hash_mask;
};


DR_NS_END

template <typename T>
DR_RINLINE long hash(const T &s)
{
	return (long)s;
}

template <>
DR_RINLINE long hash(const DR_NSP(String) &s)
{
	return s.getHash();
}


#include <dr/cmp.hxx>

DR_NS_BEGIN


template <>
class Compar<String>
{
public:
	static DR_RINLINE bool		teq(const String &t1, const String &t2)	{ return t1 == t2; }
	static DR_RINLINE int		tcmp(const String &t1, const String &t2)	{ DR_AssertInvalid(); return 0; }
	static DR_RINLINE void		txchg(String *t1, String *t2)		{ String t(*t1); *t1 = *t2; *t2 = t; }
};


DR_NS_END

#endif
