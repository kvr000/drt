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

#include <stdlib.h>
#include <string.h>
//#include <alloca.h>

#include <dr/Mem.hxx>

#include <dr/Array.hxx>

DR_NS_BEGIN


DR_EXPORT_MET void Array_c::destroy_g()
{
	if (data) {
		freeList(len);
	}
	data = NULL;
}

DR_EXPORT_MET void Array_c::resize_g(size_t nsize)
{
	data = (char *)reallocList(nsize, len);
	len = nsize;
}

struct sortdata
{
	Array_c *ar;
	size_t tsize;
	int (*cmpfunc)(void *d1, void *d2, sortdata *sd);
	void (*xchgfunc)(void *d1, void *d2, size_t size);

	union
	{
		int (*userF)(void *d1, void *d2);
		int (*userD)(void *d1, void *d2, void *data);
	};
	void *userDdata;

	union {
		Aligned al[0];
		char pivot[0];
	};
};

static int cmp_tcmp(void *d1, void *d2, sortdata *sd)
{
	return sd->ar->el_cmp(d1, d2);
}

static int cmp_userF(void *d1, void *d2, sortdata *sd)
{
	return sd->userF(d1, d2);
}

static int cmp_userD(void *d1, void *d2, sortdata *sd)
{
	return sd->userD(d1, d2, sd->userDdata);
}

static int rcmp_tcmp(void *d1, void *d2, sortdata *sd)
{
	return -sd->ar->el_cmp(d1, d2);
}

static int rcmp_userF(void *d1, void *d2, sortdata *sd)
{
	return -sd->userF(d1, d2);
}

static int rcmp_userD(void *d1, void *d2, sortdata *sd)
{
	return -sd->userD(d1, d2, sd->userDdata);
}

void Array_c::sort_i(char *beg, char *end, sortdata *sd)
{
	size_t tsize = sd->tsize;
	char *med;
	char *mi, *mj;
	med = beg+(end-beg)/tsize/2*tsize;
	memcpy(sd->pivot, med, tsize);
	mi = beg; mj = end;
	do {
		while (sd->cmpfunc(mi, sd->pivot, sd) < 0)
			mi += tsize;
		while (sd->cmpfunc(sd->pivot, mj, sd) < 0)
			mj -= tsize;
		if (mi < mj) {
			sd->xchgfunc(mi, mj, tsize);
			mi += tsize;
			mj -= tsize;
		}
		else {
			if (mi == mj) {
				mi += tsize;
				mj -= tsize;
			}
			break;
		}
	} while (mi <= mj);
	if (beg < mj)
		sort_i(beg, mj, sd);
	if (mi < end)
		sort_i(mi, end, sd);
}

DR_EXPORT_MET void Array_c::sort_g(size_t tsize)
{
	sortdata *sd;
	if (len <= tsize)
		return;
	sd = (sortdata *)alloca(sizeof(sortdata)+tsize);
	sd->ar = this;
	sd->tsize = tsize;
	sd->cmpfunc = &cmp_tcmp;
	sd->xchgfunc = Mem::getXchgFunc(tsize);
	sort_i((char *)data, (char *)data+len-tsize, sd);
}

DR_EXPORT_MET void Array_c::sortF_g(size_t tsize, int (*f)(void *d1, void *d2))
{
	sortdata *sd;
	if (len <= tsize)
		return;
	sd = (sortdata *)alloca(sizeof(sortdata)+tsize);
	sd->ar = this;
	sd->tsize = tsize;
	sd->userF = f;
	sd->cmpfunc = &cmp_userF;
	sd->xchgfunc = Mem::getXchgFunc(tsize);
	sort_i((char *)data, (char *)data+len-tsize, sd);
}

DR_EXPORT_MET void Array_c::sortD_g(size_t tsize, int (*f)(void *d1, void *d2, void *pass), void *pass)
{
	sortdata *sd;
	if (len <= tsize)
		return;
	sd = (sortdata *)alloca(sizeof(sortdata)+tsize);
	sd->ar = this;
	sd->tsize = tsize;
	sd->userD = f;
	sd->userDdata = pass;
	sd->cmpfunc = &cmp_userD;
	sd->xchgfunc = Mem::getXchgFunc(tsize);
	sort_i((char *)data, (char *)data+len-tsize, sd);
}

DR_EXPORT_MET void Array_c::rsort_g(size_t tsize)
{
	sortdata *sd;
	if (len <= tsize)
		return;
	sd = (sortdata *)alloca(sizeof(sortdata)+tsize);
	sd->ar = this;
	sd->tsize = tsize;
	sd->cmpfunc = &rcmp_tcmp;
	sd->xchgfunc = Mem::getXchgFunc(tsize);
	sort_i((char *)data, (char *)data+len-tsize, sd);
}

DR_EXPORT_MET void Array_c::rsortF_g(size_t tsize, int (*f)(void *d1, void *d2))
{
	sortdata *sd;
	if (len <= tsize)
		return;
	sd = (sortdata *)alloca(sizeof(sortdata)+tsize);
	sd->ar = this;
	sd->tsize = tsize;
	sd->userF = f;
	sd->cmpfunc = &rcmp_userF;
	sd->xchgfunc = Mem::getXchgFunc(tsize);
	sort_i((char *)data, (char *)data+len-tsize, sd);
}

DR_EXPORT_MET void Array_c::rsortD_g(size_t tsize, int (*f)(void *d1, void *d2, void *pass), void *pass)
{
	sortdata *sd;
	if (len <= tsize)
		return;
	sd = (sortdata *)alloca(sizeof(sortdata)+tsize);
	sd->ar = this;
	sd->tsize = tsize;
	sd->userD = f;
	sd->userDdata = pass;
	sd->cmpfunc = &rcmp_userD;
	sd->xchgfunc = Mem::getXchgFunc(tsize);
	sort_i((char *)data, (char *)data+len-tsize, sd);
}

DR_EXPORT_MET void Array_c::moveFrom_g(Array_c *source)
{
	destroy_g();
	data = source->data;
	len = source->len;
	source->data = NULL;
	source->len = 0;
}

DR_NS_END
