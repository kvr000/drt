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
		Node *				next;

		/* constructor */		Node(Node *next_)		: next(next_) {}
	};

	struct SNode: public Node
	{
		String				str;

		/* constructor */		SNode(SNode *next_, const String &str_): Node(next_), str(str_) {}
	};

	struct BNode: public Node
	{
		BString				bstr;
		/* constructor */		BNode(BNode *next_, const BString &bstr_): Node(next_), bstr(bstr_) {}
	};

	size_t				shash_size;
	SNode **			shash;
	size_t				bhash_size;
	BNode **			bhash;
	Mutex *				wlock;
} *volatile constants;

static void createConstants()
{
	String_constants *lcons = (String_constants *)AllocConst::alloc(sizeof(String_constants));
	lcons->shash_size = 1024;
	lcons->shash = (String_constants::SNode **)AllocConst::alloc(lcons->shash_size*sizeof(String_constants::SNode *));
	memset(lcons->shash, 0, lcons->shash_size*sizeof(String_constants::SNode *));
	lcons->bhash_size = 1024;
	lcons->bhash = (String_constants::BNode **)AllocConst::alloc(lcons->bhash_size*sizeof(String_constants::BNode *));
	memset(lcons->bhash, 0, lcons->bhash_size*sizeof(String_constants::BNode *));
	lcons->wlock = Mutex::create();

	if (!Atomic::cmpxchg((void **)(void *)&constants, NULL, lcons)) {
		//lcons->wlock->unref();
		//Alloc::free(lcons->bhash);
		//Alloc::free(lcons->shash);
		//Alloc::free(lcons);
	}
}

const BString &Const::bstring(const char *bstr_)
{
	BString bstr(bstr_);
	String_constants::BNode *node;

	if (!constants)
		createConstants();

	size_t hash = bstr.getHash();

	for (node = (String_constants::BNode *)constants->bhash[hash&(constants->bhash_size-1)]; node; node = (String_constants::BNode *)node->next) {
		if (bstr.beq(node->bstr))
			goto out;
	}

	{
		constants->wlock->lock();
		for (node = (String_constants::BNode *)constants->bhash[hash&(constants->bhash_size-1)]; node; node = (String_constants::BNode *)node->next) {
			if (bstr.beq(node->bstr))
				goto out_unlock;
		}
		node = new String_constants::BNode(constants->bhash[hash&(constants->bhash_size-1)], bstr);
		constants->bhash[hash&(constants->bhash_size-1)] = node;

out_unlock:
		constants->wlock->unlock();
	}
out:
	return node->bstr;
}

const String &Const::string(const char *str_)
{
	String str(str_);
	String_constants::SNode *node;

	if (!constants)
		createConstants();

	size_t hash = str.getHash();

	for (node = (String_constants::SNode *)constants->shash[hash&(constants->shash_size-1)]; node; node = (String_constants::SNode *)node->next) {
		if (str.beq(node->str))
			goto out;
	}

	{
		BString bstr(bstring(str_));
		str.setUtf8(bstr);
		constants->wlock->lock();
		for (node = (String_constants::SNode *)constants->shash[hash&(constants->shash_size-1)]; node; node = (String_constants::SNode *)node->next) {
			if (str.beq(node->str))
				goto out_unlock;
		}
		node = new String_constants::SNode(constants->shash[hash&(constants->shash_size-1)], str);
		constants->shash[hash&(constants->shash_size-1)] = node;

out_unlock:
		constants->wlock->unlock();
	}
out:
	return node->str;
}


DR_NS_END
