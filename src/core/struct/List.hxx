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
public:
	size_t				count()					{ return item_count; }

public:
	DR_MINLINE			List_c()				: item_count(0) { link.prev = NULL; link.next = NULL; }
	DR_MINLINE virtual		~List_c()				{ DR_Assert(link.prev == NULL && link.next == NULL && item_count == 0); }
	void				destroy_g();

protected: // alloc and compare interface
	virtual ListNode_c *		node_def(const void *v) = 0;
	virtual ListNode_c *		node_undef() = 0;
	virtual void			node_destroy(ListNode_c *d) = 0;

	virtual bool			node_eq(ListNode_c *n, const void *v) = 0;

protected:
	void				clean_g();
	ListNode_c *			append_g(ListNode_c *prev, const void *v);
	ListNode_c *			appendNew_g(ListNode_c *prev);
	ListNode_c *			insertNew_g(ListNode_c *prev);
	ListNode_c *			insert_g(ListNode_c *next, const void *v);
	void				remove_g(ListNode_c *n);

public:
	void				moveFrom_g(List_c *source);

protected:
	ListNode_c			link;
	unsigned			item_count;

private:
	List_c &			operator=(const List_c &);
};

template <typename V>
struct TListNode: public ListNode_c
{
	V				v;
	DR_RINLINE			TListNode(const V &v_)			: v(v_) { }
	DR_RINLINE			TListNode(const None &)			{}
};

/**
 * List container
 */
template <typename V, typename Compar_ = Compar<V>, typename Allocator_ = Alloc>
class TList: public List_c, typestore<Compar_, 0>, typestore<Allocator_, 1>
{
public:
	typedef Compar_			Compar;
	typedef Allocator_		Allocator;
	typedef typestore<Compar, 0>	ComparBase;
	typedef typestore<Allocator, 1> AllocatorBase;

	typedef TListNode<V>		Node;

public:
	DR_RINLINE			TList()					: List_c() { }
	DR_RINLINE			~TList()				{ destroy_g(); }

	DR_MINLINE Node *		iterFirst()				{ return (Node *)link.next; }
	DR_MINLINE Node *		iterLast()				{ return (Node *)link.prev; }
	DR_MINLINE Node *		iterNext(Node *iter)			{ return (Node *)iter->next; }
	DR_MINLINE Node *		iterPrev(Node *iter)			{ return (Node *)iter->prev; }

	DR_RINLINE void			clean()					{ clean_g(); }
	DR_RINLINE Node *		append(const V &v)			{ return (Node *)append_g(link.prev, &v); }
	DR_RINLINE Node *		insert(const V &v)			{ return (Node *)insert_g(link.next, &v); }
	DR_RINLINE void			remove(Node *n)				{ remove_g(n); }
	DR_RINLINE V			removeFirst()				{ Node *n = iterFirst(); V v = n->v; remove_g(n); return v; }
	DR_RINLINE V *			accAppendingNew()			{ return &((Node *)appendNew_g(link.prev))->v; }
	DR_RINLINE V *			accInsertingNew()			{ return &((Node *)insertNew_g(link.prev))->v; }

public:
	DR_MINLINE void			moveFrom(TList *source)			{ moveFrom_g(source); }

protected: // alloc and compare interface
	virtual ListNode_c *		node_def(const void *v)			{ Node *n = (Node *)allc().allocC(sizeof(Node)); new(n) Node(*(const V *)v); return n; }
	virtual ListNode_c *		node_undef()				{ Node *n = (Node *)allc().allocC(sizeof(Node)); new(n) Node(None()); return n; }
	virtual void			node_destroy(ListNode_c *n_)		{ Node *n = (Node *)n_; n->~Node(); allc().freeC(n, sizeof(Node)); }
	virtual bool			node_eq(ListNode_c *n_, const void *v)	{ Node *n = (Node *)n_; return comp().teq(n->v, *(const V *)v); }

public:
	DR_MINLINE Compar &		comp()					{ return *reinterpret_cast<Compar *>((ComparBase *)this); }
	DR_MINLINE Allocator &		allc()					{ return *reinterpret_cast<Allocator *>((AllocatorBase *)this); }
};

template <typename V>
class SList: public TList<V, ComparInv<V>, Alloc>
{
};

template <typename V>
class RList: public TList<Ref<V>, RCompar<V>, Alloc>
{
};


DR_NS_END

#endif
