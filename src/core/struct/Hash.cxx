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
			struct Node_c *c, *n;
			for (c = list[i]; c; c = n) {
				n = c->next;
				node_destroy(c);
			}
		}
	}
	freeList();
	list = NULL;
}

Hash_c::Node_c *Hash_c::find_g(long hash_value, const void *key) const
{
	Node_c *r = NULL;
	if (list) {
		for (r = list[hash_value&hashmask]; r; r = r->next) {
			if (r->hash != hash_value)
				continue;
			if (!node_eq(r, key))
				continue;
			break;
		}
	}
	return r;
}

Hash_c::Node_c *Hash_c::create_g(long hash_value, const void *key, bool *created)
{
	Node_c *r = find_g(hash_value, key);
	if (r) {
		if (created)
			*created = false;
		return r;
	}
	if ((int)item_count >= hashmask-hashmask/4) {
		unsigned olim = hashmask+1;
		unsigned i;
		if (list) {
			list = (Node_c **)reallocList((hashmask+1)*2*sizeof(Node_c *));
			hashmask = 2*hashmask+1;
		}
		else {
			list = (Node_c **)reallocList(4*sizeof(Node_c *));
			hashmask = 3;
		}

		for (i = olim; i <= (unsigned)hashmask; i++)
			list[i] = NULL;

		for (i = 0; i < olim; i++) {
			Node_c **p;
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
	r = node_undef(key);
	r->hash = hash_value;
	r->next = list[hash_value&hashmask];
	list[hash_value&hashmask] = r;
	item_count++;
	if (created)
		*created = true;
	return r;
}

bool Hash_c::remove_g(long hash_value, const void *key)
{
	if (list) {
		Node_c *r;
		Node_c **s;
		for (s = &list[hash_value&hashmask]; *s; s = &r->next) {
			r = *s;
			if (r->hash != hash_value)
				continue;
			if (!node_eq(r, key))
				continue;
			*s = r->next;
			item_count--;
			node_destroy(r);
			return true;
		}
	}
	return false;
}

void Hash_c::clean_g()
{
	if (list) {
		for (ssize_t i = 0; i <= hashmask; i++) {
			for (Node_c *n, *r = list[i]; r != NULL; r = n) {
				n = r->next;
				item_count--;
				node_destroy(r);
			}
			list[i] = NULL;
		}
	}
}

Hash_c::Node_c *Hash_c::iterFirst_g() const
{
	long i;

	for (i = 0; i <= hashmask; i++) {
		if (list[i])
			return list[i];
	}
	return NULL;
}

Hash_c::Node_c *Hash_c::iterNext_g(Node_c *c) const
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

void Hash_c::moveFrom_g(Hash_c *source)
{
	destroy_g();
	hashmask = source->hashmask;
	item_count = source->item_count;
	list = source->list;
	source->hashmask = 0;
	source->list = NULL;
	source->item_count = 0;
}


DR_NS_END
