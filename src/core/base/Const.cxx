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
#include <dr/Mutex.hxx>

DR_NS_BEGIN


struct String_constants
{
	struct Node: public Base
	{
		String			str;
		Node *			next;

		/* constructor */	Node(const String &str_, Node *next_): str(str_), next(next_) {}
	};

	size_t				hash_size;
	Node **				hash;
	Mutex *				wlock;
} *volatile constants;

static void createConstants()
{
	String_constants *lcons = (String_constants *)AllocConst::alloc(sizeof(String_constants));
	lcons->hash_size = 1024;
	lcons->hash = (String_constants::Node **)AllocConst::alloc(lcons->hash_size*sizeof(String_constants::Node *));
	memset(lcons->hash, 0, lcons->hash_size*sizeof(String_constants::Node *));
	lcons->wlock = Mutex::create();

	if (!Atomic::cmpxchg((void **)(void *)&constants, NULL, lcons)) {
		//Alloc::free(lcons->hash);
		//lcons->wlock->unref();
		//Alloc::free(lcons);
	}
}

const String &Const::string(const char *str_)
{
	String str(str_);
	String_constants::Node *node;

	if (!constants)
		createConstants();

	size_t hash = str.getHash();

	for (node = constants->hash[hash&(constants->hash_size-1)]; node; node = node->next) {
		if (str.beq(node->str))
			goto out;
	}

	constants->wlock->lock();
	for (node = constants->hash[hash&(constants->hash_size-1)]; node; node = node->next) {
		if (str.beq(node->str))
			goto out_unlock;
	}
	node = new String_constants::Node(str, constants->hash[hash&(constants->hash_size-1)]);
	constants->hash[hash&(constants->hash_size-1)] = node;

out_unlock:
	constants->wlock->unlock();
out:
	return node->str;
}


DR_NS_END
