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

/*drt
 * include: 	dr/app/def.hxx
 * include: 	dr/Ref.hxx
 * include: 	dr/Array.hxx
 * include: 	dr/Object.hxx
 * include:	dr/Avl.hxx
 * include:	dr/Hash.hxx
 * include:	dr/Mutex.hxx
 * use:		dr
 * ns:		dr::app
 */

#include <dr/x_kw.hxx>
#include <dr/DataNotFoundExcept.hxx>

#include <dr/app/ApplicationCache.hxx>

#include "_gen/ApplicationCache-def.hxx"

DR_APP_NS_BEGIN

/*drt
 * type:	struct
 * ancestor:	Hash_c::Node_c
 * class:	ApplicationCache::ValueHashNode
 *
 * ap:	ValueHashNode **		timed_pprev;
 * ap:	ValueHashNode *			timed_next;
 * ap:	int				key_id;
 * ap:	Ref<Object>			key;
 * ap:	Ref<Object>			value;
 */

DR_MET(public)
ApplicationCache::ValueHashNode::ValueHashNode(int key_id_, Object *key_, Object *value_):
	timed_pprev(NULL),
	timed_next(NULL),
	key_id(key_id_),
	key(key_, true),
	value(value_, true)
{
}

DR_MET(public virtual)
ApplicationCache::ValueHashNode::~ValueHashNode()
{
	if ((*timed_pprev = timed_next) != NULL)
		timed_next->timed_pprev = timed_pprev;
}


/*drt
 * type:	struct
 * class:	ApplicationCache::TimedTree::Node
 * ancestor:	Node_c
 *
 * ap:	SysTime				expires;
 * ap:	ValueHashNode *			list_first;
 */

DR_MET(public)
ApplicationCache::TimedTree::Node::Node(SysTime expires_, ValueHashNode *first_entry):
	expires(expires_),
	list_first(first_entry)
{
	first_entry->timed_pprev = &list_first;
}

/*drt
 * type:	noinfo
 * class:	ApplicationCache::TimedTree
 * ancestor:	Avl_c
 *
 * at:	ApplicationCache *		owner;
 */

DR_MET(public)
ApplicationCache::TimedTree::TimedTree(ApplicationCache *owner_):
	owner(owner_)
{
}

DR_MET(public)
ApplicationCache::TimedTree::~TimedTree()
{
	destroy_g();
}

DR_MET(protected virtual)
int ApplicationCache::TimedTree::node_cmp(Node_c *node, const void *key) const
{
	if (((Node *)node)->expires == *(SysTime *)key)
		return 0;
	return ((Node *)node)->expires < *(SysTime *)key ? -1 : 1;
}

DR_MET(protected virtual)
Avl_c::Node_c *ApplicationCache::TimedTree::node_def(const void *key, const void *value)
{
	return new Node(*(SysTime *)key, (ValueHashNode *)value);
}

DR_MET(protected virtual)
void ApplicationCache::TimedTree::node_set(Node_c *node_, const void *value_)
{
	Node *node = (Node *)node_;
	ValueHashNode *value = (ValueHashNode *)value_;
	if ((value->timed_next = node->list_first) != NULL) {
		value->timed_next->timed_pprev = &value->timed_next;
	}
	node->list_first = value;
	value->timed_pprev = &node->list_first;
}

DR_MET(protected virtual)
void ApplicationCache::TimedTree::node_destroy(Node_c *node_)
{
	Node *node = (Node *)node_;
	while (node->list_first != NULL) {
		owner->value_hash.remove(node->list_first->key_id, node->list_first->key);
	}
	delete node;
}

DR_MET(public)
bool ApplicationCache::TimedTree::replace(SysTime expires, ValueHashNode *value)
{
	return replace_g(&expires, value);
}

DR_MET(public)
void ApplicationCache::TimedTree::clean()
{
	clean_g();
}

DR_MET(public)
SysTime ApplicationCache::TimedTree::getLowestTime()
{
	if (lowest) {
		return ((Node *)lowest)->expires;
	}
	else {
		return Time::INVAL_TIME;
	}
}

DR_MET(public)
void ApplicationCache::TimedTree::remove(SysTime expires)
{
	remove_g(&expires);
}

/*drt
 * type:	noinfo
 * class:	ApplicationCache::ValueHash
 * ancestor:	Hash_c
 *
 * at:	ApplicationCache *		owner;
 */

DR_MET(public)
ApplicationCache::ValueHash::ValueHash(ApplicationCache *owner_):
	owner(owner_)
{
}

DR_MET(public virtual)
ApplicationCache::ValueHash::~ValueHash()
{
	destroy_g();
}

DR_MET(protected virtual)
void ApplicationCache::ValueHash::node_addrs(dr::Hash_c::Node_c *entry, const void **key, const void **val) const
{
	DR_AssertInvalid();
}

DR_MET(protected virtual)
void ApplicationCache::ValueHash::node_set(dr::Hash_c::Node_c *entry, const void *val) const
{
	DR_AssertInvalid();
}

DR_MET(protected virtual)
bool ApplicationCache::ValueHash::node_eq(dr::Hash_c::Node_c *entry, const void *key) const
{
	int key_id = (int)(SintPtr)((void **)key)[0];
	Object *key_obj = (Object *)((void **)key)[1];
	if (((ValueHashNode *)entry)->key_id != key_id)
		return false;
	return key_id < 0 ? ((ValueHashNode *)entry)->key->eq(key_obj) : ((ValueHashNode *)entry)->key->keyEq(key_obj, key_id);
}

DR_MET(protected virtual)
dr::Hash_c::Node_c *ApplicationCache::ValueHash::node_def(const void *key, const void *value)
{
	return new ValueHashNode((int)((void **)key)[0], (Object *)((void **)key)[1], (Object *)value);
}

DR_MET(protected virtual)
dr::Hash_c::Node_c *ApplicationCache::ValueHash::node_undef(const void *key)
{
	return new ValueHashNode((int)((void **)key)[0], (Object *)((void **)key)[1], NULL);
	return NULL;
}

DR_MET(protected virtual)
void ApplicationCache::ValueHash::node_destroy(dr::Hash_c::Node_c *entry)
{
	delete (ValueHashNode *)entry;
}

DR_MET(protected virtual)
void* ApplicationCache::ValueHash::reallocList(size_t size)
{
	return MM::realloc(list, size);
}

DR_MET(protected virtual)
void ApplicationCache::ValueHash::freeList()
{
	return MM::free(list);
}

DR_MET(public)
void ApplicationCache::ValueHash::remove(int key_id, Object *key)
{
	void *hash_key[2] = { (void *)key_id, key };
	remove_g(key_id < 0 ? key->hash() : key->keyHash(key_id), hash_key);
}

DR_MET(public)
ApplicationCache::ValueHashNode *ApplicationCache::ValueHash::createRetCreated(int key_id, Object *key, Object *value)
{
	bool created;
	void *hash_key[2] = { (void *)key_id, key };
	ValueHashNode *entry = (ValueHashNode *)create_g(key_id < 0 ? key->hash() : key->keyHash(key_id), hash_key, &created);
	entry->value.setDoref(value);
	if (!created)
		return NULL;
	return entry;
}

DR_MET(public)
ApplicationCache::ValueHashNode *ApplicationCache::ValueHash::find(int key_id, Object *key)
{
	void *hash_key[2] = { (void *)key_id, key };
	return (ValueHashNode *)find_g(key_id < 0 ? key->hash() : key->keyHash(key_id), hash_key);
}


/*drt
 * class:	ApplicationCache
 * ancestor:	dr::Object
 * ifaces:	
 *
 * at:	ValueHash			value_hash;
 * at:	TimedTree			timed_tree;
 * at:	size_t				max_items;
 * at:	Ref<Mutex>			access_lock;
 */

DR_MET(public)
ApplicationCache::ApplicationCache():
	value_hash(this),
	timed_tree(this),
	max_items(1024),
	access_lock(Mutex::create(), false)
{
}

DR_MET(protected virtual)
ApplicationCache::~ApplicationCache()
{
	timed_tree.clean();
}

DR_MET(public virtual)
/**
 * finds a resource in the cache
 *
 * @param key
 * 	key to be found
 *
 * @return NULL
 * 	if key is not found in the cache
 * @return object
 * 	cached object
 */
Object *ApplicationCache::findResource(Object *key)
{
	if (ValueHashNode *entry = value_hash.find(-1, key)) {
		return entry->value.getDoref();
	}
	else {
		return NULL;
	}
}

DR_MET(public virtual)
/**
 * adds a resource to cache
 *
 * @param expires
 * 	specifies the time the resource expires
 * @param key
 * 	specifies the key of the object
 * @param value
 * 	specifies value of the cached object
 */
void ApplicationCache::addResource(SysTime expires, Object *key, Object *value)
{
	MutexLocker locker(access_lock);

	if (ValueHashNode *entry = value_hash.createRetCreated(-1, key, value)) {
		xtry {
			timed_tree.replace(expires, entry);
		}
		xcatchany {
			xsafe(value_hash.remove(-1, key));
			xrethrowany;
		}
		xend;
	}
}

DR_MET(public virtual)
/**
 * finds a resource in the cache
 *
 * @param key_id
 * 	key_id to be used for comparision
 * @param key
 * 	key to be found
 *
 * @return NULL
 * 	if key is not found in the cache
 * @return object
 * 	cached object
 */
Object *ApplicationCache::findKeyResource(Object *key, int key_id)
{
	if (ValueHashNode *entry = value_hash.find(key_id, key)) {
		return entry->value.getDoref();
	}
	else {
		return NULL;
	}
}

DR_MET(public virtual)
/**
 * adds a resource to cache
 *
 * @param expires
 * 	specifies the time the resource expires
 * @param key
 * 	specifies the key of the object
 * @param value
 * 	specifies value of the cached object
 */
void ApplicationCache::addKeyResource(SysTime expires, Object *key, int key_id)
{
	MutexLocker locker(access_lock);

	if (ValueHashNode *entry = value_hash.createRetCreated(-1, key, key)) {
		xtry {
			timed_tree.replace(expires, entry);
		}
		xcatchany {
			xsafe(value_hash.remove(key_id, key));
			xrethrowany;
		}
		xend;
	}
}

DR_MET(public virtual)
/**
 * removes all expired entries from cache
 *
 * @param time
 * 	current time to compare
 * @return INVALID_SYSTIME
 * 	timeout to next expire
 * @return timeout
 */
SysTime ApplicationCache::expire(SysTime time)
{
	MutexLocker locker(access_lock);
	for (;;) {
		SysTime lowest_time = timed_tree.getLowestTime();
		if (lowest_time == Time::INVAL_TIME)
			return Time::INVAL_TIME;
		if (lowest_time > time && value_hash.count() <= max_items) {
			return lowest_time-time;
		}
		timed_tree.remove(lowest_time);
	}
}

DR_MET(public virtual)
/**
 * cleans the object
 */
void ApplicationCache::clean()
{
	MutexLocker locker(access_lock);
	timed_tree.clean();
}


DR_APP_NS_END
