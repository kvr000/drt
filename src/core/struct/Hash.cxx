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

#include <dr/Hash.hxx>

DR_NS_BEGIN


void Hash_c::destroy_g()
{
	if (list) {
		int i;
		for (i = 0; i <= hashmask; i++) {
			struct kvpair_g *c, *n;
			for (c = list[i]; c; c = n) {
				n = c->next;
				pairDestroy(c);
			}
		}
	}
	freeList();
	list = NULL;
}

Hash_c::kvpair_g *Hash_c::find_g(long hash_value, const void *key) const
{
	kvpair_g *r = NULL;
	if (list) {
		for (r = list[hash_value&hashmask]; r; r = r->next) {
			if (r->hash != hash_value)
				continue;
			if (!keyeq(r, key))
				continue;
			break;
		}
	}
	return r;
}

Hash_c::kvpair_g *Hash_c::create_g(long hash_value, const void *key, bool *created)
{
	kvpair_g *r = find_g(hash_value, key);
	if (r) {
		if (created)
			*created = false;
		return r;
	}
	if ((int)count >= hashmask-hashmask/4) {
		unsigned olim = hashmask+1;
		unsigned i;
		if (list) {
			list = (kvpair_g **)reallocList((hashmask+1)*2*sizeof(kvpair_g *));
			hashmask = 2*hashmask+1;
		}
		else {
			list = (kvpair_g **)reallocList(4*sizeof(kvpair_g *));
			hashmask = 3;
		}

		for (i = olim; i <= (unsigned)hashmask; i++)
			list[i] = NULL;

		for (i = 0; i < olim; i++) {
			kvpair_g **p;
			for (p = &list[i]; *p; ) {
				if ((unsigned)((*p)->hash&hashmask) != i) {
					r = *p;
					*p = r->next;
					r->next = list[r->hash&hashmask];
					list[r->hash&hashmask] = r;
				}
				else
					p = &(*p)->next;
			}
		}
	}
	else {
		DR_Assert(list != NULL);
	}
	r = pairUndef(key);
	r->hash = hash_value;
	r->next = list[hash_value&hashmask];
	list[hash_value&hashmask] = r;
	count++;
	if (created)
		*created = true;
	return r;
}

bool Hash_c::remove_g(long hash_value, const void *key)
{
	if (list) {
		kvpair_g *r;
		kvpair_g **s;
		for (s = &list[hash_value&hashmask]; *s; s = &r->next) {
			r = *s;
			if (r->hash != hash_value)
				continue;
			if (!keyeq(r, key))
				continue;
			*s = r->next;
			count--;
			pairDestroy(r);
			return true;
		}
	}
	return false;
}

void Hash_c::clean_g()
{
	if (list) {
		for (ssize_t i = 0; i <= hashmask; i++) {
			for (kvpair_g *n, *r = list[i]; r != NULL; r = n) {
				n = r->next;
				count--;
				pairDestroy(r);
			}
			list[i] = NULL;
		}
	}
}

Hash_c::kvpair_g *Hash_c::iterFirst_g() const
{
	long i;

	for (i = 0; i <= hashmask; i++) {
		if (list[i])
			return list[i];
	}
	return NULL;
}

Hash_c::kvpair_g *Hash_c::iterNext_g(kvpair_g *c) const
{
	long i;

	if (c->next)
		return c->next;

	for (i = (c->hash&hashmask)+1; i <= hashmask; i++) {
		if (list[i])
			return list[i];
	}
	return NULL;
}


DR_NS_END
