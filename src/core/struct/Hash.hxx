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

#ifndef dr__Hash__hxx__
# define dr__Hash__hxx__

#include <dr/types.hxx>
#include <dr/cmp.hxx>
#include <dr/Alloc.hxx>
#include <dr/Object.hxx>
#include <dr/SerializeEncoder.hxx>
#include <dr/SerializeDecoder.hxx>

DR_NS_BEGIN


/**
 * ancestor for hashes
 */
class DR_PUB Hash_c: public Object
{
public:
	DR_RINLINE size_t		count() const				{ return item_count; }

public:
	struct Node_c
	{
		long			hash;
		Node_c *		next;
	};
protected:
	DR_MINLINE			Hash_c()				: hashmask(-1), list(NULL), item_count(0) {}
	DR_MINLINE virtual		~Hash_c()				{ DR_Assert(list == NULL); } // do not call it!
	void				destroy_g();				// must be called from child's destructor

protected: // interface to template
	virtual bool			node_eq(Node_c *pair, const void *key) const = 0;
	virtual Node_c *		node_def(const void *key, const void *val) = 0;
	virtual Node_c *		node_undef(const void *key) = 0;
	virtual void			node_destroy(Node_c *p) = 0;

	virtual void *			reallocList(size_t nsize) = 0;
	virtual void			freeList() = 0;

protected:
	Node_c *			find_g(long hash, const void *key) const;
	Node_c *			create_g(long hash, const void *key, bool *created);
	bool				remove_g(long hash, const void *key);
	void				clean_g();

protected:
	Node_c *			iterFirst_g() const;
	Node_c *			iterNext_g(Node_c *cur) const;

private:
	/* disallowed */		Hash_c(const Hash_c &);
	/* disallowed */		Hash_c &operator=(const Hash_c &);

protected:
	ssize_t				hashmask;
	Node_c **			list;
	unsigned			item_count;
};


template <typename K, typename V>
class HashCompar: public Compar<K>
{
	typedef Compar<K> KBase;
public:
	static DR_MINLINE long		khash(const K &k)			{ return hash(k); }
	static DR_MINLINE bool		keq(const K &k1, const K &k2)		{ return k1 == k2; }

	DR_RINLINE			HashCompar()				{}
	DR_RINLINE			HashCompar(const HashCompar &)		{}
};

template <typename K, typename V>
class THashNode: public Hash_c::Node_c
{
public:
	K				k;
	V				v;

public:
	DR_RINLINE			THashNode()				{}
	DR_RINLINE			THashNode(const K &key_)		: k(key_) { }
	DR_RINLINE			THashNode(const K &key_, const V &val_)	: k(key_), v(val_) { }
};

template <typename K, typename V, typename Allocator_ = Alloc>
class THashAlloc: public Allocator_
{
	typedef THashNode<K, V>		Node;
	typedef Allocator_		Base;

public:
	DR_MINLINE static Node *	allocK(const K &key)					{ Node *t; t = (Node *)Base::allocC(sizeof(Node)); new(t) Node(key); return t; }
	DR_MINLINE static Node *	alloc2(const K &key, const V &val)			{ Node *t; t = (Node *)Base::allocC(sizeof(Node)); new(t) Node(key, val); return t; }
	DR_MINLINE static void		freePair(Node *t)					{ t->~Node(); Base::freeC(t, sizeof(Node)); }
	DR_MINLINE static void *	reallocAr(void *m, size_t nsize, size_t osize)		{ return Base::realloc(m, nsize); }
	DR_MINLINE static void		freeAr(void *m, size_t size)				{ return Base::free(m); }
};

/**
 * Hash container
 */
template <typename K, typename V, typename Compar = HashCompar<K, V>, typename Allocator = THashAlloc<K, V> >
class THash: public Hash_c, private typestore<Compar, 0>, private typestore<Allocator, 1>
{
public:
	typedef THashNode<K, V>		Node;
	typedef typestore<Compar, 0>	ComparBase;
	typedef typestore<Allocator, 1>	AllocatorBase;

protected:
	DR_MINLINE virtual bool		node_eq(Node_c *pair, const void *key) const		{ return comp().keq(((Node *)pair)->k, *(const K *)key); }
	DR_MINLINE virtual Node_c *	node_def(const void *key, const void *val)		{ return allc().alloc2(*(const K *)key, *(const V *)val); }
	DR_MINLINE virtual Node_c *	node_undef(const void *key)				{ return allc().allocK(*(const K *)key); }
	DR_MINLINE virtual void		node_destroy(Node_c *p)					{ allc().freePair((Node *)p); }

	DR_MINLINE virtual void *	reallocList(size_t nsize)				{ return allc().reallocAr(list, nsize, (hashmask+1)*sizeof(Node_c *)); }
	DR_MINLINE virtual void		freeList()						{ allc().freeAr(list, (hashmask+1)*sizeof(Node_c *)); }

public:
	DR_MINLINE Compar &		comp() const						{ return *reinterpret_cast<Compar *>((ComparBase *)this); }
	DR_MINLINE Allocator &		allc() const						{ return *reinterpret_cast<Allocator *>((AllocatorBase *)this); }

public:
	DR_MINLINE			THash(const Compar &comp_ = Compar(), const Allocator &allc_ = Allocator())	: ComparBase(comp_), AllocatorBase(allc_) { }
	DR_MINLINE			THash(const None &)					{ }
	DR_MINLINE			~THash()						{ destroy_g(); }

public:
	DR_MINLINE Node *		find(const K &k) const					{ return (Node *)Hash_c::find_g(comp().khash(k), &k); }
	DR_MINLINE V *			accValue(const K &k) const				{ if (Node *kvp = (Node *)Hash_c::find_g(comp().khash(k), &k)) return &kvp->v; else return NULL; }
	DR_MINLINE Node *		create(const K &k, bool *created = NULL)		{ return (Node *)Hash_c::create_g(comp().khash(k), &k, created); }
	DR_MINLINE bool			remove(const K &k)					{ return Hash_c::remove_g(comp().khash(k), &k); }
	DR_MINLINE void			clean()							{ clean_g(); }

	DR_MINLINE Node *		iterFirst() const					{ return (Node *)Hash_c::iterFirst_g(); }
	DR_MINLINE Node *		iterNext(Node *cur) const				{ return (Node *)Hash_c::iterNext_g(cur); }


	DR_MINLINE V &			operator[](const K &k)					{ return create(k)->v; }
};

template <typename K, typename V, typename Compar = HashCompar<K, Ref<V> > >
class RHash: public THash<K, Ref<V>, Compar>
{
};


template <typename K, typename V, typename Compar = HashCompar<K, V>, typename Allocator = THashAlloc<K, V> >
class HashSerializer
{
	typedef THash<V, Compar, Allocator> HashType;
public:
	void				serializeHash(HashType *hash, SerializeEncoder *stream) { stream->writeArrayLength(hash->count()); for (typename HashType::Node *i = hash->iterFirst(); i != NULL; i = hash->iterNext(i)) { stream->writeStructLength(2); stream->writeMemberName("k"); stream->writeValue(ScalarPtr(&i->k)); stream->writeMemberName("v"); stream->writeValue(ScalarPtr(&i->v)); stream->writeStructEnd(); } stream->writeArrayEnd(); }
	void				unserializeHash(HashType *hash, SerializeDecoder *stream) { K k; V v; ScalarPtr kp(&k); ScalarPtr vp(&v); ssize_t cnt = stream->readArrayLength(); while (stream->checkArrayNext(&cnt)) { stream->readValue(vp); (*hash)[k] = v; } }
};


DR_NS_END

#endif
