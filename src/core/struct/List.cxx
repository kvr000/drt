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

#include <dr/List.hxx>

DR_NS_BEGIN


DR_EXPORT_MET void List_c::destroy_g()
{
	clean_g();
}

DR_EXPORT_MET void List_c::clean_g()
{
	ListNode_c *p, *m;
	for (p = link.next; p; p = m) {
		m = p->next;
		item_count--;
		node_destroy(p);
	}
	link.prev = link.next = NULL;
}

DR_EXPORT_MET ListNode_c *List_c::append_g(ListNode_c *prev, const void *v)
{
	ListNode_c *n = node_def(v);
	if ((n->prev = prev)) {
		if ((n->next = prev->next))
			n->next->prev = n;
		else
			link.prev = n;
		prev->next = n;
	}
	else {
		if ((n->next = link.next))
			n->next->prev = n;
		else
			link.prev = n;
		link.next = n;
	}
	item_count++;
	return n;
}

DR_EXPORT_MET ListNode_c *List_c::appendNew_g(ListNode_c *prev)
{
	ListNode_c *n = node_undef();
	if ((n->prev = prev)) {
		if ((n->next = prev->next))
			n->next->prev = n;
		else
			link.prev = n;
		prev->next = n;
	}
	else {
		if ((n->next = link.next))
			n->next->prev = n;
		else
			link.prev = n;
		link.next = n;
	}
	item_count++;
	return n;
}

DR_EXPORT_MET ListNode_c *List_c::insert_g(ListNode_c *next, const void *v)
{
	ListNode_c *n = node_def(v);
	if ((n->next = next)) {
		if ((n->prev = next->prev))
			n->prev->next = n;
		else
			link.next = n;
		next->prev = n;
	}
	else {
		if ((n->prev = link.prev))
			n->prev->next = n;
		else
			link.next = n;
		link.prev = n;
	}
	item_count++;
	return n;
}

DR_EXPORT_MET ListNode_c *List_c::insertNew_g(ListNode_c *next)
{
	ListNode_c *n = node_undef();
	if ((n->next = next)) {
		if ((n->prev = next->prev))
			n->prev->next = n;
		else
			link.next = n;
		next->prev = n;
	}
	else {
		if ((n->prev = link.prev))
			n->prev->next = n;
		else
			link.next = n;
		link.prev = n;
	}
	item_count++;
	return n;
}

DR_EXPORT_MET void List_c::remove_g(ListNode_c *n)
{
	if (n->prev) {
		if ((n->prev->next = n->next))
			n->next->prev = n->prev;
		else
			link.prev = n->prev;
	}
	else {
		if ((link.next = n->next))
			link.next->prev = NULL;
		else
			link.prev = NULL;
	}
	item_count--;
	node_destroy(n);
}

DR_EXPORT_MET void List_c::moveFrom_g(List_c *source)
{
	destroy_g();
	link = source->link;
	item_count = source->item_count;
	source->link.prev = NULL;
	source->link.next = NULL;
	source->item_count = 0;
}

DR_NS_END
