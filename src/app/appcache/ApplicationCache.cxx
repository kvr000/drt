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
 * at:	ValueHashNode **		timed_pprev;
 * at:	ValueHashNode *			timed_next;
 * at:	Ref<Object>			key;
 * at:	Ref<Object>			value;
 */

DR_MET(public)
ApplicationCache::ValueHashNode::ValueHashNode(Object *key_, Object *value_):
	timed_pprev(NULL),
	timed_next(NULL),
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
 * at:	SysTime				expires;
 * at:	ValueHashNode *			list_first;
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
int ApplicationCache::TimedTree::keycmp(Node_c *node, const void *key) const
{
	if (((Node *)node)->expires == *(SysTime *)key)
		return 0;
	return ((Node *)node)->expires < *(SysTime *)key ? -1 : 1;
}

DR_MET(protected virtual)
Avl_c::Node_c *ApplicationCache::TimedTree::nodeDef(const void *key, const void *value)
{
	return new Node(*(SysTime *)key, (ValueHashNode *)value);
}

DR_MET(protected virtual)
void ApplicationCache::TimedTree::nodeUpdateValue(Node_c *node_, const void *value_)
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
void ApplicationCache::TimedTree::nodeDestroy(Node_c *node_)
{
	Node *node = (Node *)node_;
	while (node->list_first != NULL) {
		owner->value_hash.remove(node->list_first->key);
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
		return INVALID_SYSTIME;
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
bool ApplicationCache::ValueHash::keyeq(dr::Hash_c::Node_c *entry, const void *key) const
{
	return ((ValueHashNode *)entry)->key->eq((Iface *)key);
}

DR_MET(protected virtual)
dr::Hash_c::Node_c *ApplicationCache::ValueHash::pairDef(const void *key, const void *value)
{
	return new ValueHashNode((Object *)key, (Object *)value);
}

DR_MET(protected virtual)
dr::Hash_c::Node_c *ApplicationCache::ValueHash::pairUndef(const void *key)
{
	return new ValueHashNode((Object *)key, NULL);
	return NULL;
}

DR_MET(protected virtual)
void ApplicationCache::ValueHash::pairDestroy(dr::Hash_c::Node_c *entry)
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
void ApplicationCache::ValueHash::remove(Object *key)
{
	remove_g(key->hash(), key);
}

DR_MET(public)
ApplicationCache::ValueHashNode *ApplicationCache::ValueHash::createRetCreated(Object *key, Object *value)
{
	bool created;
	ValueHashNode *entry = (ValueHashNode *)create_g(key->hash(), key, &created);
	entry->value.setDoref(value);
	if (!created)
		return NULL;
	return entry;
}

DR_MET(public)
ApplicationCache::ValueHashNode *ApplicationCache::ValueHash::find(Object *key)
{
	return (ValueHashNode *)find_g(key->hash(), key);
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
	if (ValueHashNode *entry = value_hash.find(key)) {
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

	if (ValueHashNode *entry = value_hash.createRetCreated(key, value)) {
		xtry {
			timed_tree.replace(expires, entry);
		}
		xcatchany {
			xsafe(value_hash.remove(key));
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
		if (lowest_time == INVALID_SYSTIME)
			return INVALID_SYSTIME;
		if (lowest_time > time && value_hash.count() <= max_items) {
			return lowest_time-time;
		}
		timed_tree.remove(lowest_time);
	}
}


DR_APP_NS_END
