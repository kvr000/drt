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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/InvalidFormatExcept.hxx>

#include <dr/MemBase64.hxx>

DR_NS_BEGIN


DR_OBJECT_IMPL_SIMPLE(MemBase64);
DR_OBJECT_DEF(DR_NS_STR, MemBase64, Object);


MemBase64::~MemBase64()
{
}

static void encodeB64Block(const unsigned char in[3], unsigned char out[4], int len)
{
	static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	out[0] = cb64[ in[0] >> 2 ];
	out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
	out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
	out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

ssize_t MemBase64::encodeBase64(char *dst, const void *src, size_t size)
{
	size_t outsize = 0;
	while ((ssize_t)size > 0) {
		encodeB64Block((const unsigned char *)src, (unsigned char *)dst, size);
		src = (char *)src+3; size -= 3;
		dst = (char *)dst+4; outsize += 4;
	}
	return outsize;
}

static void decodeBlock(unsigned char out[3], unsigned char in[4])
{   
	out[0] = (unsigned char)(in[0] << 2 | in[1] >> 4);
	out[1] = (unsigned char)(in[1] << 4 | in[2] >> 2);
	out[2] = (unsigned char)(((in[2] << 6) & 0xc0) | in[3]);
}

ssize_t MemBase64::decodeBase64(void *dst, const char *src, size_t size)
{
	static const char cd64[] = "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfd\xfd\xff\xff\xfd\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfd\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x3e\xff\xff\xff\x3f\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\xff\xff\xff\xfe\xff\xff\xff\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\xff\xff\xff\xff\xff\xff\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33";
	unsigned char *dstc = (unsigned char *)dst;
	for (;;) {
		unsigned i;
		unsigned char bd[4];
		for (i = 0; i < 4; i++) {
retry_char:
			if (size-- == 0) {
				for (unsigned j = i; j < 4; j++)
					bd[j] = 0;
				break;
			}
			signed char v = *(const unsigned char *)src; src = (const char *)src+1;
			if (v > 122) {
				xthrownew(InvalidFormatExcept("base64", String((char *)&v, 1)));
				return -1;
			}
			else if ((v = cd64[v]) < 0) {
				if (v == -1) {
					xthrownew(InvalidFormatExcept("base64", String((char *)&v, 1)));
					return -1;
				}
				if (v == -2) {
					size = 0;
					for (unsigned j = i; j < 4; j++)
						bd[j] = 0;
					break;
				}
				goto retry_char;
			}
			bd[i] = v;
		}
		if (i == 0)
			break;
		decodeBlock(dstc, bd);
		dstc += i-1;
	}
	return dstc-(unsigned char *)dst;
}

BString MemBase64::encodeBase64(const Blob &data)
{
	BString out;
	out.unlock(MemBase64::encodeBase64(out.lock((data.getSize()+2)/3*4), data.toStr(), data.getSize()));
	return out;
}

Blob MemBase64::decodeBase64(const BString &data)
{
	Blob out;
	out.unlock(MemBase64::decodeBase64(out.lock(data.getSize()/4*3), data.toStr(), data.getSize()));
	return out;
}


DR_NS_END
