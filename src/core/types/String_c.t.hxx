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

#ifndef dr__String_c__t__hxx__
# define dr__String_c__t__hxx__

#include <stdio.h>
#include <limits.h>

#include <dr/String_c.hxx>
#include <dr/sex.hxx>
#include <dr/Throw.hxx>

DR_NS_BEGIN

#define DRSTRING_C_TI(SubtypeT) \
	template <> StrData_c<SubtypeT> StrData_c<SubtypeT>::null = { 1, 0, -1, -1, { } }; \
	template <> StrData_c<SubtypeT> StrData_c<SubtypeT>::zero = { 1, 0, 0, -1, { } }; \
	template <> StrData_c<SubtypeT>::inistatic StrData_c<SubtypeT>::ini; \
	template struct StrData_c<SubtypeT>; \
	template class String_c<SubtypeT>

template <typename Subtype>
DR_EXPORT_MET StrData_c<Subtype>::inistatic::inistatic()
{
	new(null.con()) cont((Invalid *)-1);
	new(zero.con()) cont((Invalid *)0); 
}

template <typename Subtype>
DR_EXPORT_MET StrData_c<Subtype>::inistatic::~inistatic()
{
	//null.con()->~cont();
	//zero.con()->~cont(); 
}

template <typename Subtype>
DR_EXPORT_MET long StrData_c<Subtype>::updateHash()
{
	DR_AssertCompile(sizeof(TC) == sizeof(Uint8) || sizeof(TC) == sizeof(Uint32) || sizeof(TC) == sizeof(Uint16), StrData_c__updateHash__unexpected_size_of_TC);
	unsigned long h = 0;
	size_t i;
	for (i = 0; i < size; i++)
		h = (h*19)^(sizeof(TC) == sizeof(Uint8)?(Uint8)str()[i]:sizeof(TC) == sizeof(Uint16)?(Uint16)str()[i]:(Uint32)str()[i]);
	hash = (h^(h>>10)^(h>>20))&LONG_MAX;
	return hash;
}

template <typename Subtype>
DR_RINLINE void StrData_c<Subtype>::padzeroes()
{
	size_t npad = ((size*sizeof(TC))&(sizeof(long)-1));
	if (npad == 0) {
		*((long *)(void *)&str()[size]) = 0;
	}
	else {
		unsigned long *base = (unsigned long *)((char *)(str()+size)-npad);
#if DR_SEX == DR_SEX_LE
		*base &= (ULONG_MAX>>((sizeof(long)-npad)*8));
#else
		*base &= (ULONG_MAX<<((sizeof(long)-npad)*8));
#endif
	}
}

/**
 * allocates shared data to store string of size size (including padding zero)
 *
 * the data are guaranteed to have zeroes starting at the size-1 till the end
 */
template <typename SubtypeT>
DR_RINLINE typename String_c<SubtypeT>::Data *String_c<SubtypeT>::allocDg(size_t size)
{
	Data *s;
	DR_Assert(size > 0);
	s = (Data *)Alloc::allocS(Data::compsize(size));
	new(s->data) typename Data::cont();
	s->al = size;
	s->refcnt = 0;
	((long *)(void *)s->str())[((size-1)*sizeof(TC))/sizeof(long)] = 0;
	s->changed();
	return s;
}

template <typename SubtypeT>
DR_RINLINE void String_c<SubtypeT>::freeDg(Data *s)
{
	DR_Assert(s->al > 0);
	s->subdestroy();
	Alloc::freeS(s, s->compsize());
}

template <typename SubtypeT>
DR_RINLINE void String_c<SubtypeT>::freeD() const
{
	DR_Assert(d->al > 0);
	d->subdestroy();
	Alloc::freeS(d, d->compsize());
}

template <typename SubtypeT>
DR_RINLINE void String_c<SubtypeT>::reallocSelfDo(size_t newcsize)
{
	Data *cr = allocDg(newcsize+1);
	if (newcsize < d->size*3/2) {
		newcsize = d->size*3/2;
	}
	SubtypeT::pcopy(cr->str(), d->str(), d->size);
	cr->size = d->size;
	i_setNoref(cr);
}

template <typename SubtypeT>
DR_EXPORT_MET String_c<SubtypeT> &String_c<SubtypeT>::setNull()
{
	i_setDoref(&Data::null);
	return *this; 
}

template <typename SubtypeT>
DR_EXPORT_MET String_c<SubtypeT> &String_c<SubtypeT>::setEmpty()
{
	i_setDoref(&Data::zero);
	return *this; 
}

template <typename SubtypeT>
DR_EXPORT_MET String_c<SubtypeT> &String_c<SubtypeT>::invalidSet()
{
	if (d) {
		unrefDg(d);
		d = NULL;
	}
	return *this;
}

template <typename SubtypeT>
DR_EXPORT_MET String_c<SubtypeT> &String_c<SubtypeT>::invalidUnset()
{
	if (!d) {
		d = refDg(&Data::zero);
	}
	return *this;
}

template <typename SubtypeT>
DR_EXPORT_MET typename String_c<SubtypeT>::Data *String_c<SubtypeT>::createDgLen(const TC *s, size_t size)
{
	Data *sh;
	if (size == 0) {
		sh = refDg(&Data::zero);
	}
	else {
		sh = allocDg(size+1);
		sh->size = size;
		sh->sub()->pcopy(sh->str(), s, size);
	}
	return sh;
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::initS(const String_c &s)
{
	d = s.i_refD();
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::initZ(const TC *s)
{
	d = createDgLen(s, d->sub()->zlen(s));
}

template <typename SubtypeT>
DR_EXPORT_MET void String_c<SubtypeT>::initLen(const TC *s, size_t size)
{
	d = createDgLen(s, size);
}

template <typename Subtype>
DR_EXPORT_MET String_c<Subtype> &String_c<Subtype>::operator=(const String_c &s)
{
	i_setDoref(s.d);
	return *this;
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::copyS(const String_c &s)
{
	i_setDoref(s.d);
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::copyZ(const TC *s)
{
	i_setNoref(createDgLen(s, Subtype::zlen(s)));
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::copyLen(const TC *s, size_t size)
{
	i_setNoref(createDgLen(s, size));
}

template <typename Subtype>
DR_EXPORT_MET bool String_c<Subtype>::beq(const String_c &s) const
{
	Data *fd = d, *sd = s.d;

	if ((fd->hash|sd->hash) < 0) {
		if ((fd->hash) < 0)
			fd->updateHash();
		if ((sd->hash) < 0)
			sd->updateHash();
	}
	if (fd->hash != sd->hash)
		return false;
	if (fd == sd)
		return true;
	if (fd->size != sd->size)
		return false;
	if (Subtype::pbcmp(fd->str(), sd->str(), fd->size) == 0)
		return true;
	return false;
}

template <typename Subtype>
DR_EXPORT_MET bool String_c<Subtype>::beq(const TC *s) const
{
	Data *fd = d;

	if (fd->size != Subtype::zlen(s))
		return false;
	if (Subtype::pbcmp(fd->str(), s, fd->size) == 0)
		return true;
	return false;
}

template <typename Subtype>
DR_EXPORT_MET int String_c<Subtype>::bcmp(const String_c &s) const
{
	int r;
	Data *fd = d, *sd = s.d;

	if (fd->al < 0) {
		return (sd->al < 0)?0:-1-(sd->al != 0);
	}
	else if (sd->al < 0) {
		return 1+(fd->al != 0);
	}
	if (fd == sd) {
		return true;
	}
	if ((r = Subtype::pbcmp(fd->str(), sd->str(), (fd->size < sd->size)?fd->size:sd->size)) == 0) {
		if (fd->size != sd->size)
			return (fd->size < sd->size)?-2:2;
	}
	return 4*(r > 0)-2;
}

template <typename Subtype>
DR_EXPORT_MET typename String_c<Subtype>::TC *String_c<Subtype>::lock(size_t size)
{
	if (d->refcnt > 0 || (ssize_t)size >= d->al) {
		Data *n = allocDg(size+1);
		n->size = size;
		n->sub()->pcopy(n->str(), d->str(), d->roundsize(d->size+1));
		i_setNoref(n);
	}
	else if (d->size != size) {
		d->padzeroes();
		d->size = size;
	}
	return d->str();
}

template <typename Subtype>
DR_EXPORT_MET typename String_c<Subtype>::TC *String_c<Subtype>::lockKeep(size_t size)
{
	if (d->refcnt > 0 || (ssize_t)size >= d->al) {
		Data *n = allocDg(size+1);
		n->size = d->size;
		n->sub()->pcopy(n->str(), d->str(), d->roundsize(d->size+1));
		i_setNoref(n);
	}
	return d->str();
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::unlock(size_t size)
{
	DR_Assert(d->refcnt == 0);
	DR_Assert((ssize_t)size < d->al);
	if (d->size == size)
		return;
	if ((d->size = size) == 0) {
		i_setDoref(&Data::zero);
	}
	else {
		d->padzeroes();
		d->changed();
	}
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::unlockAppend(size_t size)
{
	DR_Assert(d->refcnt == 0);
	DR_Assert((ssize_t)(d->size+size) < d->al);
	if (size == 0)
		return;
	d->size += size;
	d->padzeroes();
	d->changed();
}

template <typename Subtype>
DR_EXPORT_MET void String_c<Subtype>::unlockZ()
{
	size_t newsize;
	DR_Assert(d->refcnt == 0);
	if ((newsize = Subtype::zlen(d->str())) == d->size)
		return;
	if ((d->size = newsize) == 0) {
		i_setDoref(&Data::zero);
	}
	else {
		d->padzeroes();
		d->changed();
	}
}

template <typename Subtype>
DR_EXPORT_MET long String_c<Subtype>::getHash() const
{
	return d->hash >= 0 ? d->hash : d->updateHash();
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::operator+(const String_c &s)
{
	Data *out;
	Data *fd = d, *sd = s.d;

	if (fd->size == 0)
		out = refDg(sd);
	else if (sd->size == 0)
		out = refDg(fd);
	else {
		out = allocDg(fd->size+sd->size+1);
		Subtype::pcopy(out->str(), fd->str(), fd->size);
		Subtype::pcopy(out->str()+fd->size, sd->str(), sd->size);
		out->size = fd->size+sd->size;
		out->str()[fd->size+sd->size] = '\0';
	}

	return String_c(out);
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::append(const String_c &s)
{
	Data *sd = s.d;

	if (sd->size == 0) {
		// no modification
	}
	else if (d->size == 0) {
		i_setDoref(sd);
	}
	else {
		reallocSelf(d->size+sd->size);
		Subtype::pcopy(d->str()+d->size, sd->str(), sd->size);
		d->size += sd->size;
		d->str()[d->size] = '\0';
	}

	return *this;
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::append(const TC *s, size_t slen)
{
	if (slen == 0) {
		// no modification
	}
	else {
		reallocSelf(d->size+slen);
		Subtype::pcopy(d->str()+d->size, s, slen);
		d->size += slen;
		d->str()[d->size] = '\0';
	}
	return *this;
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::append(const TC *s)
{
	return append(s, Subtype::zlen(s));
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendChar(TC c)
{
	return append(&c, 1);
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendAscii(const char *s, size_t slen)
{
	if (slen == 0) {
		// no modification
	}
	else {
		reallocSelf(d->size+slen);
		for (size_t i = 0; i < slen; i++) {
			d->str()[d->size+i] = s[i];
		}
		d->size += slen;
		d->str()[d->size] = '\0';
	}
	return *this;
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendReverse(const TC *s, size_t slen)
{
	if (slen == 0) {
		// no modification
	}
	else {
		Data *out = allocDg(d->size+slen+1);
		Subtype::pcopy(out->str(), d->str(), d->size);
		out->size = d->size+slen;
		for (TC *dst = out->str()+d->size; slen-- != 0; dst++)
			*dst = s[slen];
		out->str()[out->size] = '\0';
		i_setNoref(out);
	}

	return *this;
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendNumber(Sint32 number, int base)
{
	TC buf[33];
	size_t len = 0;

	TC sign = '\0';
	if (number < 0) {
		sign = '-';
		number = -number;
	}
	do {
		buf[len++] = "0123456789abcdefghijklmnopqrstuvwxyz"[(Uint32)number%base];
		number = (Uint32)number/base;
	} while (number != 0);
	if (sign)
		buf[len++] = sign;
	return appendReverse(buf, len);
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendNumber(Uint32 number, int base)
{
	TC buf[33];
	size_t len = 0;

	do {
		buf[len++] = "0123456789abcdefghijklmnopqrstuvwxyz"[number%base];
		number /= base;
	} while (number != 0);
	return appendReverse(buf, len);
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendNumber(Sint64 number, int base)
{
	TC buf[65];
	size_t len = 0;

	TC sign = '\0';
	if (number < 0) {
		sign = '-';
		number = -number;
	}
	do {
		buf[len++] = "0123456789abcdefghijklmnopqrstuvwxyz"[(Uint64)number%base];
		number = (Uint64)number/base;
	} while (number != 0);
	if (sign)
		buf[len++] = sign;
	return appendReverse(buf, len);
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendNumber(Uint64 number, int base)
{
	TC buf[65];
	size_t len = 0;

	do {
		buf[len++] = "0123456789abcdefghijklmnopqrstuvwxyz"[number%base];
		number /= base;
	} while (number != 0);
	return appendReverse(buf, len);
}

template <typename Subtype>
String_c<Subtype> &String_c<Subtype>::appendNumber(double number)
{
	char buf[128];
	size_t len = sprintf(buf, "%g", number);
	return appendAscii(buf, len);
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::left(size_t length) const
{
	Data *out;

	if (length > d->size) {
		out = refDg(d);
	}
	else {
		out = createDgLen(d->str(), length);
	}

	return String_c(out);
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::mid(size_t pos) const
{
	Data *out;
	if (pos >= d->size) {
		out = refDg(&Data::zero);
	}
	else {
		out = createDgLen(d->str()+pos, d->size-pos);
	}

	return String_c(out);
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::mid(size_t pos, size_t length) const
{
	Data *out;
	if (pos >= d->size) {
		out = refDg(&Data::zero);
	}
	else {
		if (length > d->size-pos)
			length = d->size-pos;
		out = createDgLen(d->str()+pos, length);
	}

	return String_c(out);
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::right(size_t size) const
{
	Data *out;
	if (size >= d->size) {
		out = refDg(&Data::zero);
	}
	else {
		out = createDgLen(d->str(), d->size-size);
	}

	return String_c(out);
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::cleanGet()
{
	Data *out = d;
	d = refDg(&Data::zero);
	return String_c(out);
}

template <typename Subtype>
void String_c<Subtype>::removeLeft(size_t size)
{
	if (size == 0)
		return;

	if (size > d->size)
		size = d->size;
	i_setNoref(createDgLen(d->str()+size, d->size-size));
}

template <typename Subtype>
void String_c<Subtype>::removeMid(size_t pos)
{
	if (pos >= d->size)
		pos = d->size;
	i_setNoref(createDgLen(d->str(), pos));
}

template <typename Subtype>
void String_c<Subtype>::removeMid(size_t pos, size_t size)
{
	if (pos >= d->size)
		pos = d->size;
	if (size > d->size-pos)
		size = d->size-pos;
	Data *out = allocDg(d->size-size+1);
	Subtype::pcopy(out->str(), d->str(), pos);
	Subtype::pcopy(out->str()+pos, d->str()+pos+size, d->size-(pos+size));
	out->size = d->size-size;
	out->padzeroes();
	i_setNoref(out);
}

template <typename Subtype>
void String_c<Subtype>::removeRight(size_t size)
{
	if (size >= d->size)
		size = 0;
	else
		size = d->size-size;
	i_setNoref(createDgLen(d->str(), size));
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::removeLeftGet(size_t size)
{
	String_c r(left(size));
	removeLeft(size);
	return r;
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::removeMidGet(size_t pos)
{
	String_c r(mid(pos));
	removeMid(pos);
	return r;
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::removeMidGet(size_t pos, size_t size)
{
	String_c r(mid(pos, size));
	removeMid(pos, size);
	return r;
}

template <typename Subtype>
String_c<Subtype> String_c<Subtype>::removeRightGet(size_t size)
{
	String_c r(right(size));
	removeRight(size);
	return r;
}

template <typename Subtype>
void String_c<Subtype>::trimSpaces()
{
	size_t start_pos, end_pos;

	for (start_pos = 0; start_pos < d->size && isspace(d->str()[start_pos]); start_pos++) ;
	if (start_pos == d->size) {
		end_pos = start_pos;
	}
	else {
		for (end_pos = d->size; isspace(d->str()[end_pos-1]); end_pos--) ;
	}

	*this = mid(start_pos, end_pos-start_pos);
}

template <typename Subtype>
ssize_t String_c<Subtype>::skipSpaces(size_t start_pos)
{
	for (; start_pos < d->size && isspace(d->str()[start_pos]); start_pos++) ;
	return start_pos;
}

template <typename Subtype>
DR_EXPORT_MET ssize_t String_c<Subtype>::find(TC chr) const
{
	return find(chr, 0);
}

template <typename Subtype>
DR_EXPORT_MET ssize_t String_c<Subtype>::find(TC chr, size_t start_pos) const
{
	for (; ; start_pos++) {
		if (start_pos == d->size) {
			start_pos = (size_t)-1;
			break;
		}
		if (d->str()[start_pos] == chr)
			break;
	}
	return start_pos;
}

template <typename Subtype>
DR_EXPORT_MET ssize_t String_c<Subtype>::rfind(TC chr, size_t before_pos) const
{
	if (before_pos > d->size)
		before_pos = d->size;
	for (; (ssize_t)--before_pos >= 0;) {
		if (d->str()[before_pos] == chr)
			break;
	}
	return before_pos;
}

template <typename Subtype>
DR_EXPORT_MET String_c<Subtype> String_c<Subtype>::createNumber(Sint64 value, int base)
{
	String_c r; r.appendNumber(value, base);
	return r;
}

template <typename Subtype>
DR_EXPORT_MET String_c<Subtype> String_c<Subtype>::createNumber(Uint64 value, int base)
{
	String_c r; r.appendNumber(value, base);
	return r;
}

template <typename Subtype>
DR_EXPORT_MET String_c<Subtype> String_c<Subtype>::createNumber(double value)
{
	String_c r; r.appendNumber(value);
	return r;
}


DR_NS_END

#endif
