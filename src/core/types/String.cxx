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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <limits.h>

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/String_c.t.hxx>
#include <dr/Throw.hxx>
#include <dr/CharConv.hxx>
#include <dr/CharConvExcept.hxx>
#include <dr/List.hxx>

#include <dr/String.hxx>

#if defined DR_CXX_VC
typedef DR_INT64 intmax_t;
typedef unsigned DR_INT64 uintmax_t;
#elif defined DR_OS_SYMBIAN
typedef DR_INT64 intmax_t;
typedef unsigned DR_INT64 uintmax_t;
#else
#include <stdint.h>
#endif

DR_NS_BEGIN


#define CASE_NULL \
	/**/ 0000|Binv|0000|Winv

#define CASE_BIN \
	/**/ 0000|0000|0000|0000: \
	case 0000|0000|0000|Winv: \
	case Bbin|0000|0000|0000: \
	case Bbin|0000|0000|Winv

#define CASE_WIDE \
	/**/ 0000|Binv|Wprf|0000: \
	case 0000|Binv|0000|0000: \
	case Bbin|Binv|Wprf|0000: \
	case Bbin|Binv|0000|0000: \
	case 0000|0000|Wprf|0000: \
	case Bbin|0000|Wprf|0000

#define CASE_ERR \
	/**/ Bbin|Binv|0000|Winv: \
	case 0000|0000|Wprf|Winv: \
	case Bbin|0000|Wprf|Winv: \
	case 0000|Binv|Wprf|Winv: \
	case Bbin|Binv|Wprf|Winv: \
	default

#define CASE_WIDEOK \
	/**/ 0000|0000|0000|0000: \
	case Bbin|0000|0000|0000: \
	case 0000|Binv|Wprf|0000: \
	case 0000|Binv|0000|0000: \
	case Bbin|Binv|Wprf|0000: \
	case Bbin|Binv|0000|0000: \
	case 0000|0000|Wprf|0000: \
	case Bbin|0000|Wprf|0000

#define CASE_WIDENO \
	/**/ 0000|0000|0000|Winv: \
	case Bbin|0000|0000|Winv

DR_EXPORT_DTS StrData StrData::null = { 0, 0, 0x45432344, &BString::Data::null, &WString::Data::null };
DR_EXPORT_DTS StrData StrData::zero = { 0, 0, -1, &BString::Data::zero, &WString::Data::zero };


long StrData::updateHash()
{
	DR_AssertCompile(sizeof(wchar_t) == sizeof(Uint16) || sizeof(wchar_t) == sizeof(Uint32), StringData_c__updateHash__unexpected_size_of_wchar);
	unsigned long h = 0;
	switch (getFlags()) {
	case CASE_NULL:
		return 0x45432344;
	case CASE_BIN:
		{
			unsigned char *cs = (unsigned char *)b_str();
			for (size_t i = length; i-- != 0; cs++)
				h = (h*19)^*cs;
		}
		break;
	case CASE_WIDE:
		{
			wchar_t *ws = (wchar_t *)w_str();
			for (size_t i = length; i-- != 0; ws++)
				h = (h*19)^(Uint32)*ws;
		}
		break;
	case CASE_ERR:
		DR_AssertInvalid();
	}
	hash = (h^(h>>10)^(h>>20))&LONG_MAX;
	return hash;
}

DR_EXPORT_MET StrData *StrData::allocAscii(size_t length)
{
	StrData *s = (StrData *)Alloc::allocS(sizeof(StrData));
	xtry {
		BString::Data *bytes = ((BString *)NULL)->allocDg(length+1);
		bytes->size = length;
		bytes->str()[length] = '\0';
		bytes->con()->encoding = CharConv::EF_Ascii7;
		s->b_init(bytes, 0);
		s->w_init(NULL, Winv);
	}
	xcatchany {
		Alloc::freeS(s, sizeof(StrData));
		xrethrowany;
	}
	xend;
	s->refcnt = 0;
	s->length = length;
	s->hash = -1;
	return s;
}

DR_EXPORT_MET StrData *StrData::allocBinary(size_t length)
{
	StrData *s = (StrData *)Alloc::allocS(sizeof(StrData));
	xtry {
		BString::Data *bytes = ((BString *)NULL)->allocDg(length+1);
		bytes->size = length;
		bytes->str()[length] = '\0';
		bytes->con()->encoding = CharConv::E_Binary|CharConv::E_Unknown;
		s->b_init(bytes, Bbin);
		s->w_init(NULL, Winv);
	}
	xcatchany {
		Alloc::freeS(s, sizeof(StrData));
		xrethrowany;
	}
	xend;
	s->refcnt = 0;
	s->length = length;
	s->hash = -1;
	return s;
}

DR_EXPORT_MET StrData *StrData::allocWide(size_t length)
{
	StrData *s = (StrData *)Alloc::allocS(sizeof(StrData));
	xtry {
		WString::Data *wide = ((WString *)NULL)->allocDg(length+1);
		wide->size = length;
		wide->str()[length] = '\0';
		s->b_init(NULL, Binv);
		s->w_init(wide, 0);
	}
	xcatchany {
		Alloc::freeS(s, sizeof(StrData));
		xrethrowany;
	}
	xend;
	s->refcnt = 0;
	s->length = length;
	s->hash = -1;
	return s;
}

DR_EXPORT_MET StrData *StrData::addAsciiSpace(size_t add)
{
	if (!b_ok())
		convertAscii();
	StrData *out = allocAscii(length+add);
	memcpy(out->b_str(), b_str(), length);
	unref();
	return out;
}

DR_EXPORT_MET StrData *StrData::addWideSpace(size_t add)
{
	if (!w_ok())
		convertWide();
	StrData *out = allocWide(length+add);
	wmemcpy(out->w_str(), w_str(), length);
	unref();
	return out;
}

DR_EXPORT_MET StrData *StrData::makeAsciiSpace(size_t newlen)
{
	if (!b_ok())
		convertAscii();
	StrData *out = allocAscii(newlen);
	memcpy(out->b_str(), b_str(), newlen > length ? length : newlen);
	unref();
	return out;
}

DR_EXPORT_MET StrData *StrData::makeWideSpace(size_t newlen)
{
	if (!w_ok())
		convertWide();
	StrData *out = allocWide(newlen);
	wmemcpy(out->w_str(), w_str(), newlen > length ? length : newlen);
	unref();
	return out;
}

DR_EXPORT_MET void StrData::destroy()
{
	if (b_get())
		((BString *)NULL)->unrefDg(b_get());
	if (w_get())
		((WString *)NULL)->unrefDg(w_get());
	Alloc::freeS(this, sizeof(StrData));
}

DR_EXPORT_MET bool StrData::beq(StrData *sd)
{
	int ff, sf;
	bool ret = false;

	if (this == sd) {
		ret = true;
		goto out;
	}
	if (this->hash < 0) {
		this->updateHash();
	}
	if (sd->hash != this->hash) {
		if (sd->hash >= 0)
			goto out;
		sd->updateHash();
		if (sd->hash != this->hash)
			goto out;
	}
	if (this->length != sd->length)
		goto out;

	ff = this->getFlags(); sf = sd->getFlags();
	switch ((ff|sf)&(Binv|Winv)) {
	case 0:
	case Winv:
		ret = memcmp(this->b_str(), sd->b_str(), length) == 0;
		break;
	case Binv:
		ret = wmemcmp(this->w_str(), sd->w_str(), length) == 0;
		break;
	case Binv|Winv:
		if (length == 0) {
			ret = true;
			break;
		}
		if (ff&Winv)
			this->convertWide();
		else
			sd->convertWide();
		ret = wmemcmp(this->w_str(), sd->w_str(), length) == 0;
		break;
	default:
		DR_AssertInvalid();
		ret = false;
	}

out:
	return ret;
}

DR_EXPORT_MET BString::Data *StrData::convertAscii()
{
	BString::Data *ob, *rb, *nb;
retry:
	ob = bytes;
	rb = b_real(ob);
	if (b_ok(ob)) {
		if (rb->con()->encoding&CharConv::EF_Ascii7)
			return rb->ref();
		/* hack for now */
		return rb->ref();
	}

	nb = ((BString *)NULL)->allocDg(length+1);
	nb->size = length;

	unsigned char *cs = (unsigned char *)nb->str();
	wchar_t *ws = (wchar_t *)w_get()->str();
	for (size_t i = length; i-- != 0; cs++, ws++)
		*cs = ((Uint32)*ws > 127) ? '?' : (char)*ws;

	if (!b_ok(ob)) {
		if (!Atomic::cmpxchg((void *volatile *)(void *)&bytes, ob, nb)) {
			((BString *)NULL)->unrefDg(nb);
			goto retry;
		}
		if (b_real(ob))
			((BString *)NULL)->unrefDg(b_real(ob));
		nb->ref();
	}
	return nb;
}

DR_EXPORT_MET void StrData::convertWide()
{
	WString::Data *ow, *nw;
retry:
	ow = wide;
	if (w_ok(ow))
		return;

	nw = ((WString *)NULL)->allocDg(length+1);
	nw->size = length;

	unsigned char *cs = (unsigned char *)b_get()->str();
	wchar_t *ws = (wchar_t *)nw->str();
	for (size_t i = length; i-- != 0; cs++, ws++)
		*ws = (*cs > 127) ? '?' : *cs;

	if (!Atomic::cmpxchg((void *volatile *)(void *)&wide, ow, nw)) {
		((WString *)NULL)->unrefDg(nw);
		goto retry;
	}
	if (w_real(ow))
		((WString *)NULL)->unrefDg(w_real(ow));
}

DR_EXPORT_MET StrData *StrData::append(StrData *sd)
{
	StrData *out;

	if (sd->length <= 0)
		return this;

	if (this->length <= 0) {
		sd->ref();
		this->unref();
		return sd;
	}

retry:
	int ff = this->getFlags();
	int sf = sd->getFlags();

	if (!((ff|sf)&Binv)) {
		out = allocBinary(this->length+sd->length);
		memcpy(out->b_str(), this->b_str(), this->length);
		memcpy(out->b_str()+this->length, sd->b_str(), sd->length);
	}
	else if (!((ff|sf)&Winv)) {
		out = allocWide(this->length+sd->length);
		wmemcpy(out->w_str(), this->w_str(), this->length);
		wmemcpy(out->w_str()+this->length, sd->w_str(), sd->length);
	}
	else {
		if (ff&Winv)
			this->convertWide();
		if (sf&Winv)
			sd->convertWide();
		goto retry;
	}

	unref();
	return out;
}

DR_EXPORT_MET StrData *StrData::appendAsciiChars(char asciic, size_t count)
{
	size_t origlen = length;
	StrData *out;

	switch (getFlags()) {
	case CASE_NULL:
		out = allocAscii(count);
		unref();
		memset(out->b_str(), asciic, count);
		break;
	case CASE_BIN:
		out = addAsciiSpace(count);
		memset(out->b_str()+origlen, asciic, count);
		break;
	case CASE_WIDE:
		out = addWideSpace(count);
		wmemset(out->w_str()+origlen, asciic, count);
		break;
	case CASE_ERR:
		DR_AssertInvalid();
		out = this;
	}
	return out;
}

DR_EXPORT_MET StrData *StrData::appendWideChars(wchar_t widec, size_t count)
{
	size_t origlen = length;
	StrData *out;

	switch (getFlags()) {
	case CASE_NULL:
		out = allocWide(count);
		unref();
		wmemset(out->w_str(), widec, count);
		break;
	case CASE_BIN:
		if (widec < 128) {
			out = addAsciiSpace(count);
			memset(out->b_str()+origlen, widec, count);
		}
		else {
			out = addWideSpace(count);
			wmemset(out->w_str()+origlen, widec, count);
		}
		break;
	case CASE_WIDE:
		out = addWideSpace(count);
		wmemset(out->w_str()+origlen, widec, count);
		break;
	case CASE_ERR:
		DR_AssertInvalid();
		out = this;
	}
	return out;
}

DR_EXPORT_MET StrData *StrData::appendAscii(const char *appstr, size_t applen)
{
	size_t origlen = length;
	StrData *out;

	switch (getFlags()) {
	case CASE_NULL:
		out = allocAscii(applen);
		unref();
		{
			unsigned char *cs = (unsigned char *)out->b_str();
			while (applen-- > 0)
				*cs++ = *appstr++;
		}
		break;
	case CASE_BIN:
		out = addAsciiSpace(applen);
		{
			unsigned char *cs = (unsigned char *)out->b_str()+origlen;
			while (applen-- > 0)
				*cs++ = *appstr++;
		}
		break;
	case CASE_WIDE:
		out = addWideSpace(applen);
		{
			wchar_t *ws = (wchar_t *)out->w_str()+origlen;
			while (applen-- > 0)
				*ws++ = *appstr++;
		}
		break;
	case CASE_ERR:
		DR_AssertInvalid();
		out = this;
	}
	return out;
}

DR_EXPORT_MET StrData *StrData::appendWide(const wchar_t *appstr, size_t applen)
{
	size_t origlen = length;
	StrData *out;

	out = addWideSpace(applen);
	wmemcpy(out->w_str()+origlen, appstr, applen);

	return out;
}

DR_EXPORT_MET StrData *StrData::appendAsciiReverse(const char *appstr, size_t applen)
{
	size_t origlen = length;
	StrData *out;

	switch (getFlags()) {
	case CASE_NULL:
		out = allocAscii(applen);
		unref();
		for (size_t i = applen; i-- > 0; appstr++)
			out->b_str()[i] = *appstr;
		break;
	case CASE_BIN:
		out = addAsciiSpace(applen);
		{
			unsigned char *cs = (unsigned char *)out->b_str()+origlen;
			while (applen-- > 0)
				cs[applen] = *appstr++;
		}
		break;
	case CASE_WIDE:
		out = addWideSpace(applen);
		{
			wchar_t *ws = (wchar_t *)out->w_str()+origlen;
			while (applen-- > 0)
				ws[applen] = *appstr++;
		}
		break;
	case CASE_ERR:
		DR_AssertInvalid();
		out = this;
	}
	return out;
}

DR_EXPORT_MET StrData *StrData::vformatAscii(const char *asciifmt, va_list args)
{
	enum Length {
		L_NONE	= 0,
		L_hh,
		L_h,
		L_l,
		L_ll,
		L_L,
		L_j,
		L_z,
		L_t,
	};

	enum Output {
		O_Decimal	= 0,
		O_Binary	= 1,
		O_Unsigned	= 2,
		O_Octal		= 3,
		O_Hexa		= 4,

		O_Exp		= 0,
		O_Dot		= 1,
		O_Adapt		= 2,
		O_HexFloat	= 3,

		O_TypeMask	= 7,

		F_BIG		= 16,
		F_ZERO		= 64,
		F_PLUS		= 128,
		F_MINUS		= 256,
		F_SPACE		= 512,
		F_ALT		= 1024,
	};

	StrData *data = this->ref();

	xtry {
s_loop:
		for (;;) {
			switch (*asciifmt) {
			case '\0':
				goto out;
			case '%':
				asciifmt++;
				goto s_read;
			default:
				{
s_copy:
					const char *start;
					for (start = asciifmt++; *asciifmt != '\0' && *asciifmt != '%'; asciifmt++) ;
					data = data->appendAscii(start, asciifmt-start);
				}
				goto s_loop;
			}
		}

s_read:
		{
			ssize_t width = 0;
			ssize_t prec = 0;
			Length flength = L_NONE;
			int output = 0;

s_flag:
			switch (*asciifmt) {
			case 'd': case 'i': case 'b': case 'o': case 'u': case 'x': case 'X': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': case 'a': case 'A': case 'c': case 's': case 'p': case 'n':
				goto s_type;
			case '-':
				output |= F_MINUS;
				asciifmt++;
				goto s_flag;
			case '+':
				output |= F_PLUS;
				asciifmt++;
				goto s_flag;
			case '0':
				output |= F_ZERO;
				asciifmt++;
				goto s_flag;
			case ' ':
				output |= F_SPACE;
				asciifmt++;
				goto s_flag;
			case '#':
				output |= F_ALT;
				asciifmt++;
				goto s_flag;
			case '%':
				goto s_copy;
			default:
				goto s_width;
			}

s_width:
			switch (*asciifmt) {
			case 'd': case 'i': case 'b': case 'o': case 'u': case 'x': case 'X': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': case 'a': case 'A': case 'c': case 's': case 'p': case 'n':
				goto s_type;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				width = 10*width+*asciifmt++-'0';
				goto s_width;
			case '.':
				asciifmt++;
				goto s_prec;
			case '*':
				DR_Assert(width == 0);
				width = va_arg(args, int);
				asciifmt++;
				goto s_width;
			default:
				goto s_length;
			}

s_prec:
			switch (*asciifmt) {
			case 'd': case 'i': case 'b': case 'o': case 'u': case 'x': case 'X': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': case 'a': case 'A': case 'c': case 's': case 'p': case 'n':
				goto s_type;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				prec = 10*prec+*asciifmt++-'0';
				goto s_prec;
			case '*':
				DR_Assert(prec == 0);
				prec = va_arg(args, int);
				asciifmt++;
				goto s_length;
			default:
				goto s_length;
			}

s_length:
			switch (*asciifmt) {
			case 'd': case 'i': case 'b': case 'o': case 'u': case 'x': case 'X': case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': case 'a': case 'A': case 'c': case 's': case 'p': case 'n':
				goto s_type;
			case 'h':
				flength = L_h;
				if (*++asciifmt == 'h')
					flength = L_hh;
				goto s_type;
			case 'l':
				flength = L_l;
				if (*++asciifmt == 'l')
					flength = L_ll;
				goto s_type;
			case 'L':
				flength = L_L;
				asciifmt++;
				goto s_type;
			case 'j':
				flength = L_j;
				asciifmt++;
				goto s_type;
			case 'z':
				flength = L_z;
				asciifmt++;
				goto s_type;
			case 't':
				flength = L_t;
				asciifmt++;
				goto s_type;
			default:
				goto s_type;
			}

s_type:
			switch (*asciifmt) {
			case 'd':
				output |= O_Decimal;
				asciifmt++;
				goto out_int;
			case 'i':
				output |= O_Decimal;
				asciifmt++;
				goto out_int;
			case 'b':
				output |= O_Binary;
				asciifmt++;
				goto out_uint;
			case 'o':
				output |= O_Octal;
				asciifmt++;
				goto out_uint;
			case 'u':
				output |= O_Unsigned;
				asciifmt++;
				goto out_uint;
			case 'x':
				output |= O_Hexa;
				asciifmt++;
				goto out_uint;
			case 'X':
				output |= O_Hexa|F_BIG;
				asciifmt++;
				goto out_uint;
			case 'e':
				output |= O_Dot;
				asciifmt++;
				goto out_float;
			case 'E':
				output |= O_Dot|F_BIG;
				asciifmt++;
				goto out_float;
			case 'f':
				output |= O_Exp;
				asciifmt++;
				goto out_float;
			case 'F':
				output |= O_Exp|F_BIG;
				asciifmt++;
				goto out_float;
			case 'g':
				output |= O_Adapt;
				asciifmt++;
				goto out_float;
			case 'G':
				output |= O_Adapt|F_BIG;
				asciifmt++;
				goto out_float;
			case 'a':
				output |= O_HexFloat;
				asciifmt++;
				goto out_float;
			case 'A':
				output |= O_HexFloat|F_BIG;
				asciifmt++;
				goto out_float;
			case 'c':
				asciifmt++;
				goto out_char;
			case 's':
				asciifmt++;
				goto out_string;
			case 'p':
				DR_Assert(flength == L_NONE);
				flength = L_l;
				output |= F_ALT;
				output |= O_Hexa;
				asciifmt++;
				goto out_uint;
			case 'n':
				asciifmt++;
				goto out_printed;
			default:
				DR_AssertInvalid();
				asciifmt++;
				break;
			}
out_int:
			{
				int base = 10;
				long value = 0;
				Sint64 bigvalue = 0;
				char o_sign = '\0';
				char o_bufs[1024];
				char *o_buf = o_bufs+sizeof(o_bufs);

				switch (flength) {
				case L_NONE:
					value = va_arg(args, int);
					goto format_long;
				case L_hh:
					value = (char)va_arg(args, int);
					goto format_long;
				case L_h:
					value = (short)va_arg(args, int);
					goto format_long;
				case L_l:
					value = va_arg(args, long);
					goto format_long;
				case L_ll:
					if (sizeof(long long) <= sizeof(long)) {
						value = (long)va_arg(args, long long);
						goto format_long;
					}
					else {
						DR_AssertCompile(sizeof(long long) <= sizeof(Sint64), longlong_bigger_than_int64);
						bigvalue = va_arg(args, long long);
						goto format_biglong;
					}
				case L_L:
					DR_AssertInvalid();
					break;
				case L_j:
					if (sizeof(intmax_t) <= sizeof(long)) {
						value = (long)va_arg(args, intmax_t);
						goto format_long;
					}
					else {
						DR_AssertCompile(sizeof(intmax_t) <= sizeof(Sint64), intmax_t_bigger_than_int64);
						bigvalue = va_arg(args, intmax_t);
						goto format_biglong;
					}
					break;
				case L_z:
					if (sizeof(ssize_t) <= sizeof(long)) {
						value = (long)va_arg(args, ssize_t);
						goto format_long;
					}
					else {
						DR_AssertCompile(sizeof(ssize_t) <= sizeof(Sint64), ssize_t_bigger_than_int64);
						bigvalue = va_arg(args, ssize_t);
						goto format_biglong;
					}
					break;
				case L_t:
					if (sizeof(SintPtr) <= sizeof(long)) {
						value = (long)va_arg(args, SintPtr);
						goto format_long;
					}
					else {
						DR_AssertCompile(sizeof(SintPtr) <= sizeof(Sint64), ptr_bigger_than_int64);
						bigvalue = va_arg(args, SintPtr);
						goto format_biglong;
					}
					break;
				}
				DR_AssertInvalid();
format_long:
				if (value < 0) {
					o_sign = '-';
					value = -value;
				}
				else if (output&F_PLUS) {
					o_sign = '+';
				}

				do {
					*--o_buf = (char)((unsigned long)value%base+'0');
					value = (unsigned long)value/base;
				} while (value != 0);

				goto format_long_common;
format_biglong:
				if (bigvalue < 0) {
					o_sign = '-';
					bigvalue = -bigvalue;
				}
				else if (output&F_PLUS) {
					o_sign = '+';
				}

				do {
					*--o_buf = (char)((Uint64)bigvalue%base+'0');
					bigvalue = (Uint64)bigvalue/base;
				} while (bigvalue != 0);

				goto format_long_common;

format_long_common:
				if ((prec -= (o_bufs+sizeof(o_bufs)-o_buf)) > 0) {
					if (prec < 16) {
						memset(o_buf -= prec, '0', prec);
						prec = 0;
					}
				}
				else
					prec = 0;
				if ((width -= (o_bufs+sizeof(o_bufs)-o_buf)+prec+(o_sign!='\0')) > 0 && !(output&F_MINUS)) {
					if (prec > 0 || width > 16) {
						if (output&F_ZERO) {
							if (o_sign) {
								data = data->appendAsciiChars(o_sign, 1);
							}
							data = data->appendAsciiChars('0', width);
						}
						else {
							data = data->appendAsciiChars(' ', width);
						}
					}
					else {
						if (output&F_ZERO) {
							memset(o_buf -= width, '0', width);
						}
						else {
							if (o_sign) {
								*--o_buf = o_sign;
								o_sign = '\0';
							}
							memset(o_buf -= width, ' ', width);
						}
					}
					width = 0;
				}
				if (o_sign) {
					if (prec > 0)
						data = data->appendAsciiChars(o_sign, 1);
					else
						*--o_buf = o_sign;
				}
				else if (prec > 0)
					data = data->appendAsciiChars('0', prec);
				data = data->appendAscii(o_buf, (o_bufs+sizeof(o_bufs)-o_buf));
				if (width > 0) {
					data = data->appendAsciiChars(' ', width);
				}
			}
			goto s_loop;
out_uint:
			{
				int base;
				unsigned long value = 0;
				Uint64 bigvalue = 0;
				ssize_t o_signlen = 0;
				char o_bufs[1024];
				char *o_buf = o_bufs+sizeof(o_bufs);
				const char *chars = "0123456789abcdefghijklmnopqrstuvwxyz";

				switch (flength) {
				case L_NONE:
					value = va_arg(args, unsigned int);
					goto format_ulong;
				case L_hh:
					value = (unsigned char)va_arg(args, unsigned int);
					goto format_ulong;
				case L_h:
					value = (unsigned short)va_arg(args, unsigned int);
					goto format_ulong;
				case L_l:
					value = va_arg(args, unsigned long);
					goto format_ulong;
				case L_ll:
					if (sizeof(unsigned long long) <= sizeof(unsigned long)) {
						value = (unsigned long)va_arg(args, unsigned long long);
						goto format_ulong;
					}
					else {
						DR_AssertCompile(sizeof(unsigned long long) <= sizeof(Uint64), ulonglong_bigger_than_uint64);
						bigvalue = va_arg(args, unsigned long long);
						goto format_bigulong;
					}
				case L_L:
					DR_AssertInvalid();
					break;
				case L_j:
					if (sizeof(uintmax_t) <= sizeof(unsigned long)) {
						value = (unsigned long)va_arg(args, uintmax_t);
						goto format_ulong;
					}
					else {
						DR_AssertCompile(sizeof(uintmax_t) <= sizeof(Uint64), uintmax_t_bigger_than_uint64);
						bigvalue = va_arg(args, uintmax_t);
						goto format_bigulong;
					}
					break;
				case L_z:
					if (sizeof(size_t) <= sizeof(unsigned long)) {
						value = va_arg(args, size_t);
						goto format_ulong;
					}
					else {
						DR_AssertCompile(sizeof(size_t) <= sizeof(Uint64), size_t_bigger_than_uint64);
						bigvalue = va_arg(args, size_t);
						goto format_bigulong;
					}
					break;
				case L_t:
					if (sizeof(UintPtr) <= sizeof(unsigned long)) {
						value = va_arg(args, UintPtr);
						goto format_ulong;
					}
					else {
						DR_AssertCompile(sizeof(UintPtr) <= sizeof(Uint64), ptr_bigger_than_uint64);
						bigvalue = va_arg(args, UintPtr);
						goto format_bigulong;
					}
					break;
				}
				DR_AssertInvalid();
format_ulong:
				switch (output&O_TypeMask) {
				default:
					DR_AssertInvalid();
					base = 0;
					break;
				case O_Binary:
					base = 2;
					break;
				case O_Unsigned:
					base = 10;
					break;
				case O_Octal:
					base = 8;
					if (output&F_ALT) {
						if (value != 0)
							o_signlen = 1;
					}
					break;
				case O_Hexa:
					base = 16;
					if (output&F_BIG)
						chars = "0123456789ABCDEF";
					if (output&F_ALT)
						o_signlen = 2;
					break;
				}

				do {
					*--o_buf = chars[value%base];
					value /= base;
				} while (value != 0);

				goto format_ulong_common;
format_bigulong:
				switch (output&O_TypeMask) {
				default:
					DR_AssertInvalid();
					base = 0;
					break;
				case O_Unsigned:
					base = 10;
					break;
				case O_Octal:
					base = 8;
					if (output&F_ALT) {
						if (bigvalue != 0)
							o_signlen = 1;
					}
					break;
				case O_Hexa:
					base = 16;
					if (output&F_BIG)
						chars = "0123456789ABCDEF";
					if (output&F_ALT)
						o_signlen = 2;
					break;
				}

				do {
					*--o_buf = chars[bigvalue%base];
					bigvalue /= base;
				} while (bigvalue != 0);

				goto format_ulong_common;

format_ulong_common:
				if ((prec -= (o_bufs+sizeof(o_bufs)-o_buf)) > 0) {
					if (o_signlen == 1)
						o_signlen = 0;
					if (prec < 16) {
						memset(o_buf -= prec, '0', prec);
						prec = 0;
					}
				}
				else
					prec = 0;
				if ((width -= (o_bufs+sizeof(o_bufs)-o_buf)+prec+o_signlen) > 0 && !(output&F_MINUS)) {
					if (prec > 0 || width > 16) {
						if (output&F_ZERO) {
							if (o_signlen) {
								data = data->appendAscii((output&F_BIG) ? "0X" : "0x", o_signlen);
								o_signlen = 0;
							}
							data = data->appendAsciiChars('0', width);
						}
						else {
							data = data->appendAsciiChars(' ', width);
						}
					}
					else {
						if (output&F_ZERO) {
							memset(o_buf -= width, '0', width);
						}
						else {
							if (o_signlen) {
								if (o_signlen > 1)
									*--o_buf = asciifmt[-1];
								*--o_buf = '0';
								o_signlen = 0;
							}
							memset(o_buf -= width, ' ', width);
						}
					}
					width = 0;
				}
				if (o_signlen) {
					if (prec > 0)
						data = data->appendAscii("0x", o_signlen);
					else {
						if (o_signlen > 1)
							*--o_buf = asciifmt[-1];
						*--o_buf = '0';
					}
				}
				else if (prec > 0)
					data = data->appendAsciiChars('0', prec);
				data = data->appendAscii(o_buf, (o_bufs+sizeof(o_bufs)-o_buf));
				if (width > 0) {
					data = data->appendAsciiChars(' ', width);
				}
			}

out_float:
			{
			}
			goto s_loop;
out_char:
			{
				switch (flength) {
				default:
					DR_AssertInvalid();
					break;
				case L_NONE:
					{
						char asciic = (char)va_arg(args, int);
						data = data->appendAsciiChars(asciic, 1);
					}
					break;
				case L_l:
					{
						wchar_t widec = (wchar_t)va_arg(args, wint_t);
						data = data->appendWideChars(widec, 1);
					}
					break;
				}
			}
			goto s_loop;
out_string:
			{
				switch (flength) {
				default:
					DR_AssertInvalid();
					break;
				case L_NONE:
					{
						const char *str = va_arg(args, const char *);
						if (str) {
							data = data->appendAscii(str, strlen(str));
						}
						else {
							data = data->appendAscii("(null)", 6);
						}
					}
					break;
				case L_l:
					{
						const wchar_t *str = va_arg(args, const wchar_t *);
						if (str) {
							data = data->appendWide(str, wcslen(str));
						}
						else {
							data = data->appendAscii("(null)", 6);
						}
					}
					break;
				}
			}
			goto s_loop;
out_printed:
			{
				switch (flength) {
				case L_NONE:
					*va_arg(args, unsigned int *) = data->length;
					break;
				case L_hh:
					*va_arg(args, unsigned char *) = (unsigned char)data->length;
					break;
				case L_h:
					*va_arg(args, unsigned short *) = (unsigned short)data->length;
					break;
				case L_l:
					*va_arg(args, unsigned long *) = (unsigned long)data->length;
					break;
				case L_ll:
					*va_arg(args, unsigned long long *) = (unsigned long long)data->length;
					break;
				case L_L:
					DR_AssertInvalid();
					break;
				case L_j:
					*va_arg(args, uintmax_t *) = data->length;
					break;
				case L_z:
					*va_arg(args, size_t *) = data->length;
					break;
				case L_t:
					*va_arg(args, ptrdiff_t *) = data->length;
					break;
				default:
					DR_AssertInvalid();
					break;
				}
			}
			goto s_loop;
		}
out:
		this->unref();
	}
	xcatchany {
		data->unref();
		xrethrowany;
	}
	xend;

	return data;
}

DR_EXPORT_MET String::String(const char *s, ssize_t length)
{
	if (length < 0) {
		if (s == NULL) {
			d = StrData::null.ref();
			return;
		}
		length = strlen(s);
	}
	d = StrData::allocAscii(length);
	memcpy(d->b_str(), s, length);
}

DR_EXPORT_MET String::String(const wchar_t *s, ssize_t length)
{
	if (length < 0) {
		if (s == NULL) {
			d = StrData::null.ref();
			return;
		}
		length = wcslen(s);
	}
	d = StrData::allocWide(length);
	wmemcpy(d->w_str(), s, length);
}

DR_EXPORT_MET String String::createUtf8(const BString &utf8)
{
	return String(utf8);
}

DR_EXPORT_MET String String::createUtf8(const char *utf8, size_t size)
{
	StrData *nd;
	if (size == 0) {
		nd = StrData::zero.ref();
	}
	else {
		nd = StrData::allocAscii(size);
		while (size--)
			nd->b_str()[size] = utf8[size];
	}
	return nd;
}

DR_EXPORT_MET String String::createLowerUtf8(const char *utf8, size_t size)
{
	StrData *nd;
	if (size == 0) {
		nd = StrData::zero.ref();
	}
	else {
		nd = StrData::allocAscii(size);
		while (size--)
			nd->b_str()[size] = tolower(utf8[size]);
	}
	return nd;
}

DR_EXPORT_MET String String::createUpperUtf8(const char *utf8, size_t size)
{
	StrData *nd;
	if (size == 0) {
		nd = StrData::zero.ref();
	}
	else {
		nd = StrData::allocAscii(size);
		while (size--)
			nd->b_str()[size] = toupper(utf8[size]);
	}
	return nd;
}

DR_EXPORT_MET String String::createLower(const String &str)
{
	BString o(str.utf8());
	return createLowerUtf8(o.toStr(), o.getSize());
}

DR_EXPORT_MET String String::createUpper(const String &str)
{
	BString o(str.utf8());
	return createUpperUtf8(o.toStr(), o.getSize());
}

DR_EXPORT_MET String String::createNumber(Sint64 value)
{
	BString s;
	s.appendNumber(value);
	return String(s);
}

DR_EXPORT_MET String String::createNumber(Uint64 value)
{
	BString s;
	s.appendNumber(value);
	return String(s);
}

DR_EXPORT_MET String String::createNumber(Sint32 value)
{
	BString s;
	s.appendNumber(value);
	return String(s);
}

DR_EXPORT_MET String String::createNumber(Uint32 value)
{
	BString s;
	s.appendNumber(value);
	return String(s);
}

DR_EXPORT_MET String &String::operator=(const String &s)
{
	i_setDoref(s.d);
	return *this;
}

DR_EXPORT_MET String &String::operator=(const char *asciistr)
{
	return *this = String(asciistr);
}

DR_EXPORT_MET String &String::operator=(const wchar_t *widestr)
{
	return *this = String(widestr);
}

DR_EXPORT_MET BString String::ascii() const
{
	return BString(d->convertAscii());
}

DR_EXPORT_MET WString String::wide() const
{
	d->convertWide();
	return WString(d->wide->ref());
}

DR_EXPORT_MET BString String::utf8() const
{
	/* TODO: this should be fixed, now simple ascii string is returned */
	return BString(d->convertAscii());
}

DR_EXPORT_MET String &String::setNull()
{
	i_setDoref(&StrData::null);
	return *this;
}

DR_EXPORT_MET String &String::setEmpty()
{
	i_setDoref(&StrData::zero);
	return *this;
}

DR_EXPORT_MET String &String::setAscii(const char *ascii_str, ssize_t length)
{
	if (length < 0)
		length = strlen(ascii_str);
	StrData *s = StrData::allocAscii(length);
	memcpy(s->bytes->str(), ascii_str, length);
	i_setNoref(s);
	return *this;
}

DR_EXPORT_MET String &String::setUtf8(const char *utf8_str, ssize_t length)
{
	/* TODO: this should be fixed, now simple ascii string is converted */
	if (length < 0)
		length = strlen(utf8_str);
	StrData *s = StrData::allocAscii(length);
	memcpy(s->bytes->str(), utf8_str, length);
	i_setNoref(s);
	return *this;
}

DR_EXPORT_MET String &String::setUtf8(const BString &utf8_str)
{
	/* TODO: this should be fixed, now simple ascii string is converted */
	size_t length = utf8_str.getSize();
	StrData *s = StrData::allocAscii(length);
	memcpy(s->bytes->str(), utf8_str.toStr(), length);
	i_setNoref(s);
	return *this;
}

DR_EXPORT_MET String &String::setWide(const wchar_t *wide_str, ssize_t length)
{
	/* TODO: this should be fixed, now simple ascii string is converted */
	if (length < 0)
		length = WStringSubtype::zlen(wide_str);
	StrData *s = StrData::allocWide(length);
	WStringSubtype::pcopy(s->wide->str(), wide_str, length);
	i_setNoref(s);
	return *this;
}

DR_EXPORT_MET String &String::insert(ssize_t pos, const String &s)
{
	if (pos < 0) {
		if ((pos = getLength()+pos) < 0)
			pos = 0;
	}
	else {
		if (pos > (ssize_t)getLength())
			return append(s);
	}
	return *this = left(pos)+s+mid(pos);
}

DR_EXPORT_MET String &String::insert(ssize_t pos, const BString &s)
{
	return insert(pos, String(s));
}

DR_EXPORT_MET String &String::insert(ssize_t pos, const WString &s)
{
	return insert(pos, String(s));
}

DR_EXPORT_MET String &String::append(const String &s)
{
	d = d->append(s.d);
	return *this;
}

DR_EXPORT_MET String &String::append(const BString &s)
{
	return append(String(s));
}

DR_EXPORT_MET String &String::append(const WString &s)
{
	return append(String(s));
}

DR_EXPORT_MET String &String::append(const char *asciistr, ssize_t length)
{
	return append(String(asciistr, length));
}

DR_EXPORT_MET String &String::append(const wchar_t *widestr, ssize_t length)
{
	return append(String(widestr, length));
}

DR_EXPORT_MET String &String::appendAsciiReverse(const char *asciistr, size_t length)
{
	d = d->appendAsciiReverse(asciistr, length);
	return *this;
}

DR_EXPORT_MET String &String::appendNumber(Sint32 number, int base)
{
	char bufs[32];
	char *buf = bufs+sizeof(bufs);

	char sign = '\0';
	if (number < 0) {
		sign = '-';
		number = -number;
	}
	do {
		*--buf = "0123456789abcdefghijklmnopqrstuvwxyz"[(Uint32)number%base];
		number = (Uint32)number/base;
	} while (number != 0);
	if (sign)
		--*buf = sign;
	return i_appendAscii(buf, bufs+sizeof(bufs)-buf);
}

DR_EXPORT_MET String &String::appendNumber(Uint32 number, int base)
{
	char bufs[32];
	char *buf = bufs+sizeof(bufs);

	do {
		*--buf = "0123456789abcdefghijklmnopqrstuvwxyz"[number%base];
		number /= base;
	} while (number != 0);
	return i_appendAscii(buf, bufs+sizeof(bufs)-buf);
}

DR_EXPORT_MET String &String::appendNumber(Sint64 number, int base)
{
	char bufs[64];
	char *buf = bufs+sizeof(bufs);

	char sign = '\0';
	if (number < 0) {
		sign = '-';
		number = -number;
	}
	do {
		*--buf = "0123456789abcdefghijklmnopqrstuvwxyz"[(Uint64)number%base];
		number = (Uint64)number/base;
	} while (number != 0);
	if (sign)
		--*buf = sign;
	return i_appendAscii(buf, bufs+sizeof(bufs)-buf);
}

DR_EXPORT_MET String &String::appendNumber(Uint64 number, int base)
{
	char bufs[64];
	char *buf = bufs+sizeof(bufs);

	do {
		*--buf = "0123456789abcdefghijklmnopqrstuvwxyz"[number%base];
		number /= base;
	} while (number != 0);
	return i_appendAscii(buf, bufs+sizeof(bufs)-buf);
}

DR_EXPORT_MET String &String::appendPointer(const void *ptr)
{
	UintPtr number = (UintPtr)ptr;
	char bufs[sizeof(ptr)*2+2];
	char *buf = bufs+sizeof(bufs);

	do {
		*--buf = "0123456789abcdefghijklmnopqrstuvwxyz"[number%16];
		number /= 16;
	} while (number != 0);
	*--buf = 'x';
	*--buf = '0';
	return i_appendAscii(buf, bufs+sizeof(bufs)-buf);
}

DR_EXPORT_MET String &String::appendDump(const void *mem, size_t size)
{
	char buf[1024];
	
	while (size > 0) {
		size_t offs;
		for (offs = 0; offs <= sizeof(buf)-2 && size > 0; offs += 2, mem = (const unsigned char *)mem+1, size--) {
			buf[offs+0] = "0123456789abcdefghijklmnopqrstuvwxyz"[*(const unsigned char *)mem>>4];
			buf[offs+1] = "0123456789abcdefghijklmnopqrstuvwxyz"[*(const unsigned char *)mem&15];
		}
		i_appendAscii(buf, offs);
	}
	return *this;
}

DR_EXPORT_MET char *String::lockAscii(size_t length)
{
	d = d->makeAsciiSpace(length);
	return d->b_str();
}

DR_EXPORT_MET wchar_t *String::lockWide(size_t length)
{
	d = d->makeWideSpace(length);
	return d->w_str();
}

DR_EXPORT_MET void String::unlock(size_t newlen)
{
	DR_Assert(newlen <= d->length);
	if (d->b_ok())
		d->b_setlen(newlen);
	else
		d->w_setlen(newlen);
	d->length = newlen;
}

DR_EXPORT_MET void String::unlockAuto()
{
	size_t newlen;
	if (d->b_ok()) {
		newlen = strlen(d->b_str());
		DR_Assert(newlen <= d->length);
		d->b_setlen(newlen);
	}
	else {
		newlen = wcslen(d->w_str());
		DR_Assert(newlen <= d->length);
		d->w_setlen(newlen);
	}
	d->length = newlen;
}

DR_EXPORT_MET bool String::isNull() const
{
	return d == &StrData::null;
}

DR_EXPORT_MET bool String::isEmpty() const
{
	return d->length <= 0;
}

DR_EXPORT_MET bool String::beq(const String &s) const
{
	return d->beq(s.d);
}

DR_EXPORT_MET bool String::eqUtf8(const char *str, size_t size)
{
	BString str_utf8(d->convertAscii());
	if (size != str_utf8.getSize())
		return false;
	return memcmp(str, str_utf8.toStr(), size) == 0;
}

DR_EXPORT_MET String String::operator+(const String &s) const
{
	return String(*this).append(s);
}

DR_EXPORT_MET String String::operator+(const char *asciistr) const
{
	return String(*this).append(asciistr, BStringSubtype::zlen(asciistr));
}

DR_EXPORT_MET String String::operator+(const wchar_t *widestr) const
{
	return String(*this).append(widestr, WStringSubtype::zlen(widestr));
}

DR_EXPORT_MET String String::left(size_t length) const
{
	return mid(0, length);
}

DR_EXPORT_MET String String::right(size_t length) const
{
	if (length > d->length)
		return *this;
	return mid(d->length-length, length);
}

DR_EXPORT_MET String String::mid(size_t pos) const
{
	if (pos > d->length)
		return String();
	return mid(pos, d->length-pos);
}

DR_EXPORT_MET String String::mid(size_t pos, size_t length) const
{
	if (pos > d->length)
		return String();
	if (length > d->length-pos)
		length = d->length-pos;
	BString b = ascii();
	return String(b.toStr()+pos, length);
}

DR_EXPORT_MET ssize_t String::find(const String &s) const
{
	return find(s, 0);
}

DR_EXPORT_MET ssize_t String::find(const String &s, size_t start) const
{
	BString am(ascii()), as(s.ascii());
	const char *as_s = as.toStr(); size_t as_l = as.getSize();
	const char *am_s = am.toStr(); size_t am_f = am.getSize()-as_l;
	if (as_l > am.getSize())
		return -1;
	for (; start <= am_f; start++) {
		if (am_s[start] != *as_s)
			continue;
		if (memcmp(am_s+start, as_s, as_l) == 0)
			return start;
	}
	return -1;
}

DR_EXPORT_MET ssize_t String::rfind(const String &s) const
{
	return rfind(s, getLength());
}

DR_EXPORT_MET ssize_t String::rfind(const String &s, size_t before) const
{
	BString am(ascii()), as(s.ascii());
	if (as.getSize() > am.getSize())
		return -1;
	if (before > am.getSize()-as.getSize())
		before = am.getSize()-as.getSize()+1;
	for (before--; (ssize_t)before >= 0; before--) {
		if (memcmp(am.toStr()+before, as.toStr(), as.getSize()) == 0)
			return before;
	}
	return -1;
}

DR_EXPORT_MET String &String::replace(const String &what, const String &rep)
{
	return replace(what, rep, (size_t)-1);
}

DR_EXPORT_MET String &String::replace(const String &what, const String &rep, size_t count)
{
	if (what.isEmpty())
		return *this;
	for (ssize_t p = 0; (p = find(what, p)) >= 0; p += rep.getLength())
		*this = left(p)+rep+mid(p+what.getLength());
	return *this;
}

DR_EXPORT_MET String &String::format(const char *asciifmt, ...)
{
	va_list args;
	va_start(args, asciifmt);
	vformat(asciifmt, args);
	va_end(args);
	return *this;
}

DR_EXPORT_MET String String::formats(const char *asciifmt, ...)
{
	String ns;
	va_list args;
	va_start(args, asciifmt);
	ns = vformats(asciifmt, args);
	va_end(args);
	return ns;
}

DR_EXPORT_MET String String::vformats(const char *asciifmt, va_list args)
{
	StrData *nd = StrData::zero.ref();
	xtry {
		nd = nd->vformatAscii(asciifmt, args);
	}
	xcatchany {
		nd->unref();
		xrethrowany;
	}
	xend;
	return String(nd);
}

DR_EXPORT_MET String &String::vformat(const char *asciifmt, va_list args)
{
	StrData *nd = StrData::zero.ref();
	xtry {
		nd = nd->vformatAscii(asciifmt, args);
		i_setNoref(nd);
	}
	xcatchany {
		nd->unref();
		xrethrowany;
	}
	xend;
	return *this;
}


DR_EXPORT_MET void StringIndex::clean()
{
	if (hash_list == (element **)(void *)&hash_mask)
		return;
	for (UintPtr i = 0; i <= hash_mask; i++) {
		element *n;
		for (element *e = hash_list[i]; e; e = n) {
			n = e->next;
			e->~element();
		}
	}
	Alloc::free(hash_list);
	hash_list = (element **)(void *)&hash_mask;
}


DR_EXPORT_MET void StringIndex::init(size_t count, const String *const *strs)
{
	element *epool;
	if ((hash_mask = 3*count) == 0) {
		hash_list = (element **)(void *)&hash_mask;
		return;
	}
	while (hash_mask&(hash_mask-1))
		hash_mask &= hash_mask-1;
	hash_mask--;
	xtry {
		hash_list = (element **)Alloc::alloc((hash_mask+1)*sizeof(element *)+count*sizeof(element));
		epool = (element *)&hash_list[hash_mask+1];

		memset(hash_list, 0, (hash_mask+1)*sizeof(element *));
	}
	xcatchany {
		hash_mask = 0;
		hash_list = (element **)(void *)&hash_mask;
		xrethrowany;
	}
	xend;
	while (count--) {
		UintPtr hash = strs[count]->getHash();
		element *ep = hash_list[hash&hash_mask];
		for (; ; ep = ep->next) {
			if (!ep) {
				hash_list[hash&hash_mask] = new (epool++) element(count, *strs[count], hash_list[hash&hash_mask]);
				break;
			}
			else if (ep->str == *strs[count]) {
				ep->str = *strs[count];
				break;
			}
		}
	}
}

DR_EXPORT_MET void StringIndex::init(size_t count, const String *const *strs, const long *values)
{
	element *epool;
	if ((hash_mask = 3*count) == 0) {
		hash_list = (element **)(void *)&hash_mask;
		return;
	}
	while (hash_mask&(hash_mask-1))
		hash_mask &= hash_mask-1;
	hash_mask--;
	xtry {
		hash_list = (element **)Alloc::alloc((hash_mask+1)*sizeof(element *)+count*sizeof(element));
		epool = (element *)&hash_list[hash_mask+1];

		memset(hash_list, 0, (hash_mask+1)*sizeof(element *));
	}
	xcatchany {
		hash_mask = 0;
		hash_list = (element **)(void *)&hash_mask;
		xrethrowany;
	}
	xend;
	while (count--) {
		UintPtr hash = strs[count]->getHash();
		element *ep = hash_list[hash&hash_mask];
		for (; ; ep = ep->next) {
			if (!ep) {
				hash_list[hash&hash_mask] = new (epool++) element(values[count], *strs[count], hash_list[hash&hash_mask]);
				break;
			}
			else if (ep->str == *strs[count]) {
				ep->str = *strs[count];
				break;
			}
		}
	}
}

DR_EXPORT_MET StringIndex::StringIndex(size_t count, const String *const *strs)
{
	init(count, strs);
}

DR_EXPORT_MET StringIndex::StringIndex(size_t count, const String *strs)
{
	if (count == 0) {
		init(0, NULL);
		return;
	}
	const String **s = (const String **)alloca(count*(sizeof(String *)));
	for (size_t i = 0; i < count; i++)
		s[i] = &strs[i];
	init(count, s);
}

DR_EXPORT_MET StringIndex::StringIndex(const String *str0, const String *str1, ...)
{
	if (!str0) {
		init(0, NULL);
		return;
	}
	else if (!str1) {
		init(1, &str0);
		return;
	}
	va_list vl;
	size_t count = 2;
	va_start(vl, str1);
	const String **s = (const String **)alloca(32*sizeof(String *));
	s[0] = str0;
	s[1] = str1;
	while ((str0 = va_arg(vl, String *))) {
		if ((count&(count-1)) == 0 && count >= 32)
			s = (const String **)alloca(count*2*sizeof(String *));
		s[count++] = str0;
	}
	init(count, s);
}

DR_EXPORT_MET StringIndex::StringIndex(size_t count, const char *const *strs)
{
	if (count == 0) {
		init(0, NULL);
		return;
	}
	String **s = (String **)alloca(count*sizeof(String *));
	size_t i = 0;
	xtry {
		for (i = 0; i < count; i++) {
			s[i] = (String *)alloca(sizeof(String));
			new (s[i]) String(strs[i]);
		}
		init(count, s);
	}
	xcatchany {
		while (i--)
			s[i]->~String();
	}
	xend;
	while (i--)
		s[i]->~String();
}

DR_EXPORT_MET StringIndex::StringIndex(const String *str0, long value0, ...)
{
	if (!str0) {
		init(0, NULL);
		return;
	}
	va_list vl;
	size_t count = 1;
	va_start(vl, value0);
	const String **s = (const String **)alloca(32*sizeof(String *));
	long *l = (long *)alloca(32*sizeof(long));
	s[0] = str0;
	l[0] = value0;
	for (; (str0 = va_arg(vl, String *)); count++) {
		value0 = va_arg(vl, long);
		if ((count&(count-1)) == 0 && count >= 32) {
			const String **os = s;
			s = (const String **)alloca(count*2*sizeof(String *));
			memcpy(s, os, count*sizeof(String *));
			const long *ol = l;
			l = (long *)alloca(count*2*sizeof(long));
			memcpy(l, ol, count*sizeof(long));
		}
		s[count] = str0;
		l[count] = value0;
	}
	init(count, s, l);
}

DR_EXPORT_MET void StringIndex::reinitArraySized(size_t count, const char *const *strs, size_t el_size)
{
	clean();
	if (count == 0) {
		init(0, NULL);
		return;
	}
	String **s = (String **)alloca(count*sizeof(String *));
	size_t i = 0;
	xtry {
		for (i = 0; i < count; i++) {
			s[i] = (String *)alloca(sizeof(String));
			new (s[i]) String(*strs);
			strs = (const char *const *)((const char *)strs+el_size);
		}
		init(count, s);
	}
	xcatchany {
		while (i--)
			s[i]->~String();
	}
	xend;
	while (i--)
		s[i]->~String();
}

DR_EXPORT_MET void StringIndex::reinitArraySizedConst(size_t count, const char *const *strs, size_t el_size)
{
	clean();
	if (count == 0) {
		init(0, NULL);
		return;
	}
	const String **s = (const String **)alloca(count*sizeof(String *));
	size_t i = 0;
	for (i = 0; i < count; i++) {
		s[i] = &Const::string(*strs);
		strs = (const char *const *)((const char *)strs+el_size);
	}
	init(count, s);
}

DR_EXPORT_MET StringIndex::~StringIndex()
{
	if (hash_list != (element **)(void *)&hash_mask) {
		clean();
	}
}

DR_EXPORT_MET long StringIndex::find(const String &s) const
{
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str.d == s.d)
			return e->index;
	}
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str == s)
			return e->index;
	}
	return -1;
}

DR_EXPORT_MET long StringIndex::findXchg(const String &s)
{
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str.d == s.d)
			return e->index;
	}
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str == s) {
			e->str = s;
			return e->index;
		}
	}
	return -1;
}

DR_EXPORT_MET void *StringIndex::findPtr(const String &s) const
{
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str.d == s.d)
			return (void *)e->index;
	}
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str == s)
			return (void *)e->index;
	}
	return NULL;
}

DR_EXPORT_MET void *StringIndex::findPtrXchg(const String &s)
{
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str.d == s.d)
			return (void *)e->index;
	}
	for (element *e = hash_list[s.getHash()&hash_mask]; e; e = e->next) {
		if (e->str == s) {
			e->str = s;
			return (void *)e->index;
		}
	}
	return NULL;
}

DR_EXPORT_MET void StringIndex::appendList(SList<String> *list)
{
	for (SintPtr hi = hash_mask; hi >= 0; hi--) {
		for (element *e = hash_list[hi]; e; e = e->next)
			list->append(e->str);
	}
}


DR_NS_END
