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

#ifndef dr__Avl__hxx__
# define dr__Avl__hxx__

#include <dr/types.hxx>
#include <dr/cmp.hxx>
#include <dr/emits.hxx>
#include <dr/Alloc.hxx>
#include <dr/Object.hxx>

DR_NS_BEGIN


/**
 * ancestor for Avl trees
 */
class DR_PUB Avl_c: public Object
{
public:
	struct Node_c
	{
		union {
			Node_c *			refs[3];
			struct {
				Node_c *			lchild;
				Node_c *			parent;
				Node_c *			rchild;
			};
		};
		Sint8				balance;			// -1 left heavier, 0 balanced, 1 right heavier
		Sint8				direction;			// direction relative to its parent, root has 0

		DR_CONSTRUCT			Node_c()				{ refs[0] = refs[1] = refs[2] = NULL; }
	};

protected:
	DR_MINLINE			Avl_c()					: root(NULL), lowest(NULL), count(0) {}
	DR_MINLINE virtual		~Avl_c()				{ DR_Assert(root == NULL); } // do not call it!
	void				destroy_g();				// must be called from child's destructor

protected: // interface to template
	virtual int			keycmp(Node_c *node, const void *key) const = 0;
	virtual Node_c *		nodeDef(const void *key, const void *val) = 0;
	virtual void			nodeUpdateValue(Node_c *node, const void *key) = 0;
	virtual void			nodeDestroy(Node_c *p) = 0;

protected:
	Node_c *			find_g(const void *key) const;
	bool				create_g(const void *key, const void *value);
	bool				replace_g(const void *key, const void *value);
	bool				remove_g(const void *key);
	void				clean_g();
	void				runOne_g(Node_c *node, Eslot1<void, Node_c *> &cb);
	void				run_g(Eslot1<void, Node_c *> &cb);

protected:
	void				destroyRecursive_g(Node_c *node);
	static void			rotateLeft_g(Node_c **cur);
	static void			rotateRight_g(Node_c **cur);
	void				rebalanceAdded_g(Node_c *added);
	void				rebalanceRemoved_g(Node_c *parent, int direction);

protected:
	Node_c *			iterFirst_g() const;
	Node_c *			iterNext_g(Node_c *cur) const;

private:
	/* disallowed */		Avl_c(const Avl_c &);
	/* disallowed */		Avl_c &operator=(const Avl_c &);

protected:
	Node_c *			root;
	Node_c *			lowest;
	size_t				count;
};


template <typename K, typename V>
class AvlCompar: public Compar<K>
{
	typedef Compar<K> KBase;
public:
	static DR_MINLINE int		kcmp(const K &k1, const K &k2)			{ return k1 == k2 ? 0 : k1 < k2 ? -1 : 1; }

	DR_RINLINE			AvlCompar()				{}
	DR_RINLINE			AvlCompar(const AvlCompar &)	{}
};

template <typename K, typename V>
class AvlPair: public Avl_c::Node_c
{
public:
	K				k;
	V				v;

public:
	DR_RINLINE			AvlPair(const K &k_, const V &v_)		: k(k_), v(v_) { }
};

template <typename K, typename V, typename Allocator_ = Alloc>
class AvlAlloc: public Allocator_
{
	typedef AvlPair<K, V> Type;
	typedef Allocator_ Base;

public:
	DR_MINLINE static Type *	allocK(const K &key)					{ Type *t; t = (Type *)Base::allocC(sizeof(Type)); new(t) Type(key); return t; }
	DR_MINLINE static Type *	alloc2(const K &key, const V &val)			{ Type *t; t = (Type *)Base::allocC(sizeof(Type)); new(t) Type(key, val); return t; }
	DR_MINLINE static void		freePair(Type *t)					{ t->~Type(); Base::freeC(t, sizeof(Type)); }
	DR_MINLINE static void *	reallocAr(void *m, size_t nsize, size_t osize)		{ return Base::realloc(m, nsize); }
	DR_MINLINE static void		freeAr(void *m, size_t size)				{ return Base::free(m); }
};

/**
 * Avl container
 */
template <typename K, typename V, typename Compar = AvlCompar<K, V>, typename Allocator = AvlAlloc<K, V> >
class Avl: protected Avl_c, private typestore<Compar, 0>, private typestore<Allocator, 1>
{
public:
	typedef AvlPair<K, V> Node;
	typedef typestore<Compar, 0> ComparBase;
	typedef typestore<Allocator, 1> AllocatorBase;

protected:
	DR_MINLINE virtual int		keycmp(Node_c *pair, const void *key) const		{ return comp().kcmp(((Node *)pair)->k, *(const K *)key); }
	DR_MINLINE virtual Node_c *	nodeDef(const void *key, const void *val)		{ return allc().alloc2(*(const K *)key, *(const V *)val); }
	DR_MINLINE virtual void		nodeUpdateValue(Node_c *node, const void *value)	{ ((Node *)node)->v = *(V *)value; }
	DR_MINLINE virtual void		nodeDestroy(Node_c *p)					{ allc().freePair((Node *)p); }

public:
	DR_MINLINE Compar &		comp() const						{ return *reinterpret_cast<Compar *>((ComparBase *)this); }
	DR_MINLINE Allocator &		allc() const						{ return *reinterpret_cast<Allocator *>((AllocatorBase *)this); }

public:
	DR_MINLINE			Avl(const Compar &comp_ = Compar(), const Allocator &allc_ = Allocator())	: ComparBase(comp_), AllocatorBase(allc_) { }
	DR_MINLINE			Avl(const None &)					{ }
	DR_MINLINE			~Avl()							{ destroy_g(); }

public:
	DR_RINLINE size_t		getCount() const					{ return count; }

public:
	DR_MINLINE Node *		find(const K &k) const					{ return (Node *)Avl_c::find_g(&k); }
	DR_MINLINE bool			create(const K &k, const V &v)				{ return Avl_c::create_g(&k, &v); }
	DR_MINLINE bool			replace(const K &k, const V &v)				{ return Avl_c::replace_g(&k, &v); }
	DR_MINLINE bool			remove(const K &k)					{ return Avl_c::remove_g(&k); }
	DR_MINLINE void			clean()							{ clean_g(); }
	DR_MINLINE void			run(Eslot1<void, Node *> &cb)				{ run_g((Eslot1<void, Node_c *> &)cb); }

	DR_MINLINE Node *		iterTop() const						{ return (Node *)root; }
	DR_MINLINE Node *		iterFirst() const					{ return (Node *)Avl_c::iterFirst_g(); }
	DR_MINLINE Node *		iterNext(Node *cur) const				{ return (Node *)Avl_c::iterNext_g(cur); }
};


DR_NS_END

#endif
