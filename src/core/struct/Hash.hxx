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
	struct kvpair_g
	{
		long			hash;
		kvpair_g *		next;
	};
protected:
	DR_MINLINE			Hash_c()				: hashmask(-1), list(NULL), count(0) {}
	DR_MINLINE virtual		~Hash_c()				{ DR_Assert(list == NULL); } // do not call it!
	void				destroy_g();				// must be called from child's destructor

protected: // interface to template
	virtual bool			keyeq(kvpair_g *pair, const void *key) const = 0;
	virtual kvpair_g *		pairDef(const void *key, const void *val) = 0;
	virtual kvpair_g *		pairUndef(const void *key) = 0;
	virtual void			pairDestroy(kvpair_g *p) = 0;

	virtual void *			reallocList(size_t nsize) = 0;
	virtual void			freeList() = 0;

protected:
	kvpair_g *			find_g(long hash, const void *key) const;
	kvpair_g *			create_g(long hash, const void *key, bool *created);
	bool				remove_g(long hash, const void *key);
	void				clean_g();

protected:
	kvpair_g *			iterFirst_g() const;
	kvpair_g *			iterNext_g(kvpair_g *cur) const;

private:
	/* disallowed */		Hash_c(const Hash_c &);
	/* disallowed */		Hash_c &operator=(const Hash_c &);

protected:
	ssize_t				hashmask;
	kvpair_g **			list;
	unsigned			count;
};


template <typename K, typename V>
class HashCompar: public Compar<K>
{
	typedef Compar<K> KBase;
public:
	static DR_MINLINE long		khash(const K &k)				{ return hash(k); }
	static DR_MINLINE bool		keq(const K &k1, const K &k2)			{ return k1 == k2; }

	DR_RINLINE			HashCompar()				{}
	DR_RINLINE			HashCompar(const HashCompar &)	{}
};

template <typename K, typename V>
class HashPair: public Hash_c::kvpair_g
{
public:
	K				k;
	V				v;

public:
	DR_RINLINE			HashPair()						{}
	DR_RINLINE			HashPair(const K &key_)				: k(key_) { }
	DR_RINLINE			HashPair(const K &key_, const V &val_)		: k(key_), v(val_) { }
};

template <typename K, typename V, typename Allocator_ = Alloc>
class HashAlloc: public Allocator_
{
	typedef HashPair<K, V> Type;
	typedef Allocator_ Base;

public:
	DR_MINLINE static Type *	allocK(const K &key)					{ Type *t; t = (Type *)Base::allocC(sizeof(Type)); new(t) Type(key); return t; }
	DR_MINLINE static Type *	alloc2(const K &key, const V &val)			{ Type *t; t = (Type *)Base::allocC(sizeof(Type)); new(t) Type(key, val); return t; }
	DR_MINLINE static void		freePair(Type *t)					{ t->~Type(); Base::freeC(t, sizeof(Type)); }
	DR_MINLINE static void *	reallocAr(void *m, size_t nsize, size_t osize)		{ return Base::realloc(m, nsize); }
	DR_MINLINE static void		freeAr(void *m, size_t size)				{ return Base::free(m); }
};

/**
 * Hash container
 */
template <typename K, typename V, typename Compar = HashCompar<K, V>, typename Allocator = HashAlloc<K, V> >
class Hash: public Hash_c, private typestore<Compar, 0>, private typestore<Allocator, 1>
{
public:
	typedef HashPair<K, V> kvpair;
	typedef typestore<Compar, 0> ComparBase;
	typedef typestore<Allocator, 1> AllocatorBase;

protected:
	DR_MINLINE virtual bool		keyeq(kvpair_g *pair, const void *key) const		{ return comp().keq(((kvpair *)pair)->k, *(const K *)key); }
	DR_MINLINE virtual kvpair_g *	pairDef(const void *key, const void *val)		{ return allc().alloc2(*(const K *)key, *(const V *)val); }
	DR_MINLINE virtual kvpair_g *	pairUndef(const void *key)				{ return allc().allocK(*(const K *)key); }
	DR_MINLINE virtual void		pairDestroy(kvpair_g *p)				{ allc().freePair((kvpair *)p); }

	DR_MINLINE virtual void *	reallocList(size_t nsize)				{ return allc().reallocAr(list, nsize, (hashmask+1)*sizeof(kvpair_g *)); }
	DR_MINLINE virtual void		freeList()						{ allc().freeAr(list, (hashmask+1)*sizeof(kvpair_g *)); }

public:
	DR_MINLINE Compar &		comp() const						{ return *reinterpret_cast<Compar *>((ComparBase *)this); }
	DR_MINLINE Allocator &		allc() const						{ return *reinterpret_cast<Allocator *>((AllocatorBase *)this); }

public:
	DR_MINLINE			Hash(const Compar &comp_ = Compar(), const Allocator &allc_ = Allocator())	: ComparBase(comp_), AllocatorBase(allc_) { }
	DR_MINLINE			Hash(const None &)					{ }
	DR_MINLINE			~Hash()							{ destroy_g(); }

public:
	DR_RINLINE size_t		getCount() const					{ return count; }

public:
	DR_MINLINE kvpair *		find(const K &k) const					{ return (kvpair *)Hash_c::find_g(comp().khash(k), &k); }
	DR_MINLINE kvpair *		create(const K &k, bool *created = NULL){ return (kvpair *)Hash_c::create_g(comp().khash(k), &k, created); }
	DR_MINLINE bool			remove(const K &k)					{ return Hash_c::remove_g(comp().khash(k), &k); }
	DR_MINLINE void			clean()							{ clean_g(); }

	DR_MINLINE kvpair *		iterFirst() const					{ return (kvpair *)Hash_c::iterFirst_g(); }
	DR_MINLINE kvpair *		iterNext(kvpair *cur) const				{ return (kvpair *)Hash_c::iterNext_g(cur); }


	DR_MINLINE V &			operator[](const K &k)					{ return create(k)->v; }
};

template <typename K, typename V, typename Compar = HashCompar<K, Ref<V> > >
class RHash: public Hash<K, Ref<V>, Compar>
{
};


template <typename K, typename V, typename Compar = HashCompar<K, V>, typename Allocator = HashAlloc<K, V> >
class HashSerializer
{
	typedef Hash<V, Compar, Allocator> HashType;
public:
	void				serializeHash(HashType *hash, SerializeEncoder *stream) { stream->writeArrayLength(hash->count()); for (typename HashType::kvpair *i = hash->iterFirst(); i != NULL; i = hash->iterNext(i)) { stream->writeStructLength(2); stream->writeMemberName("k"); stream->writeValue(ScalarPtr(&i->k)); stream->writeMemberName("v"); stream->writeValue(ScalarPtr(&i->v)); stream->writeStructEnd(); } stream->writeArrayEnd(); }
	void				unserializeHash(HashType *hash, SerializeDecoder *stream) { K k; V v; ScalarPtr kp(&k); ScalarPtr vp(&v); ssize_t cnt = stream->readArrayLength(); while (stream->checkArrayNext(&cnt)) { stream->readValue(vp); (*hash)[k] = v; } }
};


DR_NS_END

#endif
