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

#ifndef dr__String__t__hxx__
# define dr__String__t__hxx__

#include <stdarg.h>

#if defined DR_OS_WNT
#elif defined DR_OS_SYMBIAN
#else
# include <stdint.h>
#endif

#include <dr/Assert.hxx>
#include <dr/String_c.t.hxx>
#include <dr/Bstring.hxx>

DR_NS_BEGIN


#if 0
template <typename Allocator>
DR_EXPORT_MET StringA<Allocator> StringA<Allocator>::format(const TC *format, ...)
{
	va_list args;
	va_start(args, format);
	ssize_t ret = vformat(format, args);
	va_end(args);
	return ret;
}

template <typename Allocator>
DR_EXPORT_MET StringA<Allocator> StringA<Allocator>::vformat(const TC *format, va_list args)
{
	enum {
		F_ZERO	= 1,
		F_PLUS	= 2,
		F_MINUS	= 4,
		F_SPACE	= 8,
		F_ALT	= 16,
	};

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
		O_Unsigned	= 1,
		O_Octal		= 2,
		O_Hexa		= 3,

		O_Exp		= 0,
		O_Dot		= 1,
		O_Adapt		= 2,
		O_HexFloat	= 3,

		O_Big		= 16,
	};

	StringA<Allocator> data;

s_loop:
	for (;;) {
		switch (*format) {
		case '\0':
			goto out;
		case '%':
			format++;
			goto s_read;
		default:
			{
s_copy:
				const TC *start;
				for (start = format++; *format != '\0' && *format != '%'; format++);
				data.append(start, format-start);
			}
			goto s_loop;
		}
	}

s_read:
	{
		int flags = 0;
		int width = 0;
		int prec = 0;
		Length length = L_NONE;
		int output;

s_flag:
		switch (*format) {
		case '-':
			flags |= F_MINUS;
			format++;
			goto s_flag;
		case '+':
			flags |= F_PLUS;
			format++;
			goto s_flag;
		case '0':
			flags |= F_ZERO;
			format++;
			goto s_flag;
		case ' ':
			flags |= F_SPACE;
			format++;
			goto s_flag;
		case '#':
			flags |= F_ALT;
			format++;
			goto s_flag;
		case '%':
			goto s_copy;
		default:
			goto s_width;
		}

s_width:
		switch (*format) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			width = 10*width+*format++-'0';
			goto s_width;
		case '.':
			format++;
			goto s_prec;
		case '*':
			Assert(width == 0);
			width = va_arg(args, int);
			format++;
			goto s_width;
		default:
			goto s_length;
		}

s_prec:
		switch (*format) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			prec = 10*prec+*format++-'0';
			goto s_prec;
		case '*':
			Assert(prec == 0);
			prec = va_arg(args, int);
			format++;
			goto s_length;
		default:
			goto s_length;
		}

s_length:
		switch (*format) {
		case 'h':
			length = L_h;
			if (*++format == 'h')
				length = L_hh;
			goto s_type;
		case 'l':
			length = L_l;
			if (*++format == 'l')
				length = L_ll;
			goto s_type;
		case 'L':
			length = L_L;
			format++;
			goto s_type;
		case 'j':
			length = L_j;
			format++;
			goto s_type;
		case 'z':
			length = L_z;
			format++;
			goto s_type;
		case 't':
			length = L_t;
			format++;
			goto s_type;
		default:
			goto s_type;
		}

s_type:
		switch (*format) {
		case 'd':
			output = O_Decimal;
			goto out_int;
		case 'i':
			output = O_Decimal;
			goto out_int;
		case 'o':
			output = O_Octal;
			goto out_uint;
		case 'u':
			output = O_Unsigned;
			goto out_uint;
		case 'x':
			output = O_Hexa;
			goto out_uint;
		case 'X':
			output = O_Hexa|O_Big;
			goto out_uint;
		case 'e':
			output = O_Dot;
			goto out_float;
		case 'E':
			output = O_Dot|O_Big;
			goto out_float;
		case 'f':
			output = O_Exp;
			goto out_float;
		case 'F':
			output = O_Exp|O_Big;
			goto out_float;
		case 'g':
			output = O_Adapt;
			goto out_float;
		case 'G':
			output = O_Adapt|O_Big;
			goto out_float;
		case 'a':
			output = O_HexFloat;
			goto out_float;
		case 'A':
			output = O_HexFloat|O_Big;
			goto out_float;
		case 'c':
			goto out_char;
		case 's':
			goto out_string;
		case 'p':
			Assert(length == L_NONE && flags == 0);
			length = L_l;
			flags = F_ALT;
			output = O_Hexa;
			goto out_int;
		case 'n':
			goto out_printed;
		default:
			AssertInvalid();
			break;
		}
out_int:
		{
			long value;
			Sint64_t bigvalue;
			switch (length) {
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
					value = va_arg(args, long long);
					goto format_long;
				}
				else {
					AssertCompile(sizeof(long long) <= sizeof(Sint64_t), longlong_bigger_than_int64);
					bigvalue = va_arg(args, long long);
					goto format_biglong;
				}
			case L_L:
				AssertInvalid();
				break;
			case L_j:
				if (sizeof(intmax_t) <= sizeof(long)) {
					value = va_arg(args, intmax_t);
					goto format_long;
				}
				else {
					AssertCompile(sizeof(intmax_t) <= sizeof(Sint64_t), intmax_t_bigger_than_int64);
					bigvalue = va_arg(args, intmax_t);
					goto format_biglong;
				}
				break;
			case L_z:
				if (sizeof(ssize_t) <= sizeof(long)) {
					value = va_arg(args, ssize_t);
					goto format_long;
				}
				else {
					AssertCompile(sizeof(ssize_t) <= sizeof(Sint64_t), ssize_t_bigger_than_int64);
					bigvalue = va_arg(args, ssize_t);
					goto format_biglong;
				}
				break;
			case L_t:
				if (sizeof(SintPtr) <= sizeof(long)) {
					value = va_arg(args, SintPtr);
					goto format_long;
				}
				else {
					AssertCompile(sizeof(SintPtr) <= sizeof(Sint64_t), ssize_t_bigger_than_int64);
					bigvalue = va_arg(args, SintPtr);
					goto format_biglong;
				}
				break;
			}
			AssertInvalid();
format_long:
			if (value < 0) {
			}
format_biglong:
			;
		}
		goto s_loop;
out_uint:
		{
		}
		goto s_loop;
out_float:
		{
		}
		goto s_loop;
out_char:
		{
		}
		goto s_loop;
out_string:
		{
		}
		goto s_loop;
out_printed:
		{
		}
		goto s_loop;
	}

out:
	return data;
}
#endif

#define DRSTRING_TI(Allocator) \
	DRSTRING_C_TI(StringSubtype, Allocator); \
	template class StringA<Allocator> 


DR_NS_END

#endif
