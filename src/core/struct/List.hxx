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

#ifndef dr__List__hxx__
# define dr__List__hxx__

#include <dr/cmp.hxx>
#include <dr/types.hxx>
#include <dr/Assert.hxx>
#include <dr/Alloc.hxx>
#include <dr/Object.hxx>

DR_NS_BEGIN


struct ListNode_c
{
	ListNode_c *			prev, *next;
};

/**
 * generic ancestor for List
 */
class DR_PUB List_c: public Object
{
protected:
	ListNode_c			link;
	unsigned			cnt;

public:
	DR_MINLINE			List_c()				{ link.prev = NULL; link.next = NULL; cnt = 0; }
	DR_MINLINE virtual		~List_c()				{ DR_Assert(link.prev == NULL && link.next == NULL); }
	void				destroy_g();

	ListNode_c *			append_g(ListNode_c *prev, const void *v);
	ListNode_c *			appendNew_g(ListNode_c *prev);
	ListNode_c *			insertNew_g(ListNode_c *prev);
	ListNode_c *			insert_g(ListNode_c *next, const void *v);
	void				remove_g(ListNode_c *n);

protected: // alloc and compare interface
	virtual ListNode_c *		el_create(const void *v) = 0;
	virtual ListNode_c *		el_new() = 0;
	virtual void			el_destroy(ListNode_c *d) = 0;

	virtual bool			el_eq(ListNode_c *n, const void *v) = 0;
};

template <typename V>
struct ListNode: public ListNode_c
{
	V				v;
	DR_RINLINE			ListNode(const V &v_)			: v(v_) { }
	DR_RINLINE			ListNode(const None &)			{}
	DR_RINLINE ListNode *		prev()					{ return (ListNode *)ListNode_c::prev; }
	DR_RINLINE ListNode *		next()					{ return (ListNode *)ListNode_c::next; }
};

/**
 * List container
 */
template <typename V, typename Compar_ = Compar<V>, typename Allocator_ = Alloc>
class List: public List_c, typestore<Compar_, 0>, typestore<Allocator_, 1>
{
public:
	typedef Compar_			Compar;
	typedef Allocator_		Allocator;
	typedef typestore<Compar, 0>	ComparBase;
	typedef typestore<Allocator, 1> AllocatorBase;

	typedef ListNode<V>		Node;

	struct iterator
	{
		Node *			n;
		DR_RINLINE		iterator(Node *n_)			: n(n_) { }
		DR_RINLINE V &		operator*()				{ return n->v; }
		DR_RINLINE const V &	operator*() const			{ return n->v; }
		DR_RINLINE V *		operator->()				{ return &n->v; }
		DR_RINLINE V &		operator++()				{ n = n->next(); return n->v; }
		DR_RINLINE V &		operator++(int)				{ V *o = &n->v; n = n->next(); return *o; }
		DR_RINLINE V &		operator--()				{ n = n->prev(); return n->v; }
		DR_RINLINE V &		operator--(int)				{ V *o = &n->v; n = n->prev(); return *o; }
		DR_RINLINE bool		valid()					{ return n != NULL; }
	};

public:
	DR_RINLINE			List()					: List_c() { }
	DR_RINLINE			~List()					{ destroy_g(); }

	DR_MINLINE Node *		getFirst()				{ return (Node *)link.next; }
	DR_MINLINE Node *		getLast()				{ return (Node *)link.prev; }

	DR_RINLINE Node *		append(const V &v)			{ return (Node *)append_g(link.prev, &v); }
	DR_RINLINE Node *		insert(const V &v)			{ return (Node *)insert_g(link.next, &v); }
	DR_RINLINE void			remove(Node *n)				{ remove_g(n); }
	DR_RINLINE V			removeFirst()				{ Node *n = getFirst(); V v = n->v; remove_g(n); return v; }
	DR_RINLINE V *			accAppendingNew()			{ return &((Node *)appendNew_g(link.prev))->v; }
	DR_RINLINE V *			accInsertingNew()			{ return &((Node *)insertNew_g(link.prev))->v; }

	DR_MINLINE iterator		begin()					{ return getFirst(); }
	DR_MINLINE iterator		end()					{ return getLast(); }

	size_t				getCount()				{ return cnt; }

protected: // alloc and compare interface
	virtual ListNode_c *		el_create(const void *v)		{ Node *n = (Node *)allc().allocC(sizeof(Node)); new(n) Node(*(const V *)v); return n; }
	virtual ListNode_c *		el_new()				{ Node *n = (Node *)allc().allocC(sizeof(Node)); new(n) Node(None()); return n; }
	virtual void			el_destroy(ListNode_c *n_)		{ Node *n = (Node *)n_; n->~Node(); allc().freeC(n, sizeof(Node)); }
	virtual bool			el_eq(ListNode_c *n_, const void *v)	{ Node *n = (Node *)n_; return comp().teq(n->v, *(const V *)v); }

public:
	DR_MINLINE Compar &		comp()					{ return *reinterpret_cast<Compar *>((ComparBase *)this); }
	DR_MINLINE Allocator &		allc()					{ return *reinterpret_cast<Allocator *>((AllocatorBase *)this); }
};

template <typename V>
class SList: public List<V, ComparInv<V>, Alloc>
{
};

template <typename V>
class RList: public List<Ref<V>, RCompar<V>, Alloc>
{
};


DR_NS_END

#endif
