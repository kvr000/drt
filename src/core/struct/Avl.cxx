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

#include <stdio.h>

#include <dr/Avl.hxx>

DR_NS_BEGIN


void Avl_c::destroy_g()
{
	if (root)
		destroyRecursive_g(root);
	root = lowest = NULL;
	count = 0;
}

void Avl_c::destroyRecursive_g(Node_c *node)
{
	if (node->refs[0])
		destroyRecursive_g(node->refs[0]);
	if (node->refs[2])
		destroyRecursive_g(node->refs[2]);
}

Avl_c::Node_c *Avl_c::find_g(const void *key) const
{
	for (Node_c *cur = root; cur; ) {
		Sint8 c = -node_cmp(cur, key);
		if (c == 0)
			return cur;
		cur = cur->refs[1+c];
	}
	return NULL;
}

void Avl_c::rebalanceAdded_g(Node_c *added)
{
	Node_c *parent = added->parent;
next:
	if (parent->balance == 0) {
		parent->balance = added->direction;
		added = parent;
		if ((parent = added->parent) != NULL)
			goto next;
	}
	else if (parent->balance != added->direction) {
		parent->balance = 0;
	}
	else if (parent->balance != added->balance) {
		// double rotation
		//	      P                        AL
		//	  X       A                P       A
		//	       AL   AR           X  Y     Z AR
		//            Y  Z                 
		Node_c *child = added->refs[1-added->direction];
		if ((parent->refs[1+added->direction] = child->refs[1-added->direction]) != NULL) {
			parent->refs[1+added->direction]->parent = parent;
			parent->refs[1+added->direction]->direction = added->direction;
		}
		parent->balance = child->balance == added->direction ? -added->direction : 0;
		if ((added->refs[1-added->direction] = child->refs[1+added->direction]) != NULL) {
			added->refs[1-added->direction]->parent = added;
			added->refs[1-added->direction]->direction = -added->direction;
		}
		added->balance = child->balance == child->direction ? added->direction : 0;
		child->direction = parent->direction;
		if ((child->parent = parent->parent) != NULL) {
			child->parent->refs[1+child->direction] = child;
		}
		else {
			root = child;
		}
		parent->parent = child;
		parent->direction = -added->direction;
		added->parent = child;
		child->balance = 0;
		child->refs[1-added->direction] = parent;
		child->refs[1+added->direction] = added;
	}
	else {
		// single rotation
		//	      P                        A
		//	  X       A                P       AR
		//	       AL   AR           X  AL    Y
		//                 Y
		if ((parent->refs[1+added->direction] = added->refs[1-added->direction]) != NULL) {
			// AL
			parent->refs[1+added->direction]->parent = parent;
			parent->refs[1+added->direction]->direction = added->direction;
		}
		int oldpardir = parent->direction;
		parent->direction = -added->direction;
		parent->balance = 0;
		added->balance = 0;
		added->direction = oldpardir;
		if ((added->parent = parent->parent) != NULL) {
			added->parent->refs[1+added->direction] = added;
		}
		else {
			root = added;
		}
		added->refs[1+parent->direction] = parent;
		parent->parent = added;
	}
}

bool Avl_c::create_g(const void *key, const void *value)
{
	Node_c **cur;
	int last_step = 0;
	for (cur = &root; *cur && (last_step = -node_cmp(*cur, key)) != 0; cur = &(*cur)->refs[1+last_step]);
	if (*cur) {
		return false;
	}
	*cur = node_def(key, value);
	count++;
	(*cur)->balance = 0;
	if (((*cur)->direction = last_step) == 0) {
		(*cur)->parent = NULL;
		lowest = *cur;
	}
	else {
		(*cur)->parent = (Node_c *)(((char *)(void *)(cur-(1+last_step)))-(((char *)(void *)&((Node_c *)1024)->refs[0])-(char *)(void *)((Node_c *)1024)));
		if ((*cur)->parent == lowest && (*cur)->direction < 0)
			lowest = *cur;
		rebalanceAdded_g(*cur);
	}
	return true;
}

bool Avl_c::replace_g(const void *key, const void *value)
{
	Node_c **cur;
	int last_step = 0;
	for (cur = &root; *cur && (last_step = -node_cmp(*cur, key)) != 0; cur = &(*cur)->refs[1+last_step]);
	if (*cur) {
		node_updateValue(*cur, value);
		return false;
	}
	*cur = node_def(key, value);
	count++;
	(*cur)->balance = 0;
	if (((*cur)->direction = last_step) == 0) {
		(*cur)->parent = NULL;
		lowest = *cur;
	}
	else {
		(*cur)->parent = (Node_c *)(((char *)(void *)(cur-(1+last_step)))-(((char *)(void *)&((Node_c *)1024)->refs[0])-(char *)(void *)((Node_c *)1024)));
		if ((*cur)->parent == lowest && (*cur)->direction < 0)
			lowest = *cur;
		rebalanceAdded_g(*cur);
	}
	return true;
}

void Avl_c::rebalanceRemoved_g(Node_c *node, int direction)
{
next:
	if (node->balance == 0) {
		node->balance -= direction;
	}
	else if (node->balance == direction) {
		node->balance = 0;
		direction = node->direction;
		if ((node = node->parent) != NULL)
			goto next;
	}
	else {
		Node_c *other = node->refs[1-direction];
		if (other->balance == direction) {
			// double rotation - direction -1
			//         N                        C
			//   Y           O             N         O
			//            C      Z        Y CL     CR Z
			//          CL CR
			Node_c *child = other->refs[1+direction];
			if ((node->refs[1-direction] = child->refs[1+direction]) != NULL) {
				node->refs[1-direction]->parent = node;
				node->refs[1-direction]->direction = -direction;
			}
			node->balance = child->balance != -direction ? 0 : direction;
			if ((other->refs[1+direction] = child->refs[1-direction]) != NULL) {
				other->refs[1+direction]->parent = other;
				other->refs[1+direction]->direction = direction;
			}
			other->balance = child->balance == direction ? -direction : 0;
			child->direction = node->direction;
			if ((child->parent = node->parent) != NULL) {
				child->parent->refs[1+child->direction] = child;
			}
			else {
				root = child;
			}
			child->refs[1+direction] = node;
			node->parent = child;
			node->direction = direction;
			child->refs[1-direction] = other;
			other->parent = child;
			child->balance = 0;

			if ((node = child->parent) != NULL) {
				direction = child->direction;
				goto next;
			}
		}
		else {
			// one rotation
			//        N               O
			//           O         N     Z
			//          Y Z         Y
			if ((node->refs[1-direction] = other->refs[1+direction]) != NULL) {
				node->refs[1-direction]->parent = node;
				node->refs[1-direction]->direction = -direction;
			}
			node->balance = other->balance == -direction ? 0 : -direction;
			other->refs[1+direction] = node;
			other->direction = node->direction;
			bool decreased;
			if ((other->parent = node->parent) != NULL) {
				other->parent->refs[1+other->direction] = other;
				decreased = other->balance == -direction;
			}
			else {
				root = other;
				decreased = false;
			}
			other->balance += direction;
			node->parent = other;
			node->direction = direction;
			if (decreased) {
				node = other->parent;
				direction = other->direction;
				goto next;
			}
		}
	}
}

bool Avl_c::remove_g(const void *key)
{
	if (Node_c *node = find_g(key)) {
		count--;
		if (node == lowest) {
			if ((lowest = node->refs[2]) == NULL && (lowest = node->parent) == NULL) {
				root = NULL;
				node_destroy(node);
				return true;
			}
		}
		int dir;
		Node_c *repl;
		if (node->balance == 0) {
			if (node->refs[0] == NULL) {
				// parent always exists in this block, handled in the
				// previous block
				node->parent->refs[1+node->direction] = NULL;
				rebalanceRemoved_g(node->parent, node->direction);
				node_destroy(node);
				return true;
			}
			dir = 1;
		}
		else {
			dir = node->balance;
		}
		for (repl = node->refs[1+dir]; repl->refs[1-dir] != NULL; repl = repl->refs[1-dir]);
		DR_Assert(repl != NULL);
		Node_c *parent = repl->parent;
		if (parent == node) {
			if ((repl->refs[1-dir] = node->refs[1-dir]) != NULL)
				repl->refs[1-dir]->parent = repl;
			parent = repl;
		}
		else {
			if ((parent->refs[1-dir] = repl->refs[1+dir]) != NULL) {
				parent->refs[1-dir]->direction = -dir;
				parent->refs[1-dir]->parent = parent;
			}
			if ((repl->refs[1-dir] = node->refs[1-dir]))
				repl->refs[1-dir]->parent = repl;
			if ((repl->refs[1+dir] = node->refs[1+dir]))
				repl->refs[1+dir]->parent = repl;
			dir = -dir;
		}
		repl->balance = node->balance;
		repl->direction = node->direction;
		if ((repl->parent = node->parent) != NULL) {
			repl->parent->refs[1+node->direction] = repl;
		}
		else {
			root = repl;
		}
		rebalanceRemoved_g(parent, dir);
		node_destroy(node);
		return true;
	}
	else {
		return false;
	}
}

void Avl_c::runOne_g(Node_c *n, Eslot1<void, Node_c *> &cb)
{
	if (n->refs[0])
		runOne_g(n->refs[0], cb);
	cb(n);
	if (n->refs[1])
		runOne_g(n->refs[2], cb);
}

void Avl_c::run_g(Eslot1<void, Node_c *> &cb)
{
	if (root) {
		runOne_g(root, cb);
	}
}

void Avl_c::clean_g()
{
	if (root) {
		destroyRecursive_g(root);
		root = lowest = NULL;
		count = 0;
	}
}

Avl_c::Node_c *Avl_c::iterFirst_g() const
{
	return lowest;
}

Avl_c::Node_c *Avl_c::iterNext_g(Node_c *c) const
{
	if (c->refs[2] != NULL) {
		for (c = c->refs[2]; c->refs[0] != NULL; c = c->refs[0]);
	}
	else if (c->direction < 0) {
		c = c->parent;
	}
	else {
		for (c = c->parent; c != NULL; c = c->parent) {
			if (c->direction < 0) {
				c = c->parent;
				break;
			}
		}
	}
	return c;
}


DR_NS_END
