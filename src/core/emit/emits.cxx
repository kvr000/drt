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
#include <stdlib.h>

#include <dr/emits.hxx>

#include "emits.cxx.hxx"

DR_NS_BEGIN


DR_EXPORT_FUN void *Ealloc(size_t size)
{
	void *m = new char[size];
	return m;
}

DR_EXPORT_FUN void Efree(void *m)
{
	delete [](char *)m;
}


DR_EXPORT_MET Eslot_g::Eslot_g(const Eslot_g &r)
{
	if ((impl = r.impl))
		impl->ref();
}

DR_EXPORT_MET Eslot_g &Eslot_g::operator=(const Eslot_g &r)
{
	Eimpl_g *o = impl;
	if ((impl = r.impl))
		impl->i_ref();
	if (o)
		o->i_unref();
	return *this;
}

DR_EXPORT_MET Eslot_g::~Eslot_g()
{
	if (impl)
		impl->i_unref();
}

DR_EXPORT_MET Emitter_g::~Emitter_g()
{
	getclearVret();
	volatile Enode_g *node;

	for (node = firstv; node; node = firstv) {
		firstv = node->next;
		delete node;
	}
}

DR_EXPORT_MET Enode_g *Emitter_g::append_g(const Eslot_g &s)
{
	bool vr = getclearVret();
	Enode_g *node = new Enode_g(s);
	node->next = NULL;
	if (firstv) {
		node->prev = firstv->prev;
		firstv->prev->next = node;
	}
	else {
		firstv = node;
		}
	firstv->prev = node;
	setVret(vr);
	return node;
}

DR_EXPORT_MET Enode_g *Emitter_g::insert_g(const Eslot_g &s)
{
	bool vr = getclearVret();
	Enode_g *node = new Enode_g(s);
	if ((node->next = firstv)) {
		node->prev = firstv->prev;
		firstv->prev = node;
	}
	else
		node->prev = node;
	firstv = node;
	setVret(vr);
	return node;
}

DR_EXPORT_MET void Emitter_g::remove_g(Enode_g *node)
{
	bool vr = getclearVret();
	if (firstv == node) {
		if ((firstv = node->next))
			firstv->prev = node->prev;
	}
	else if (node->next == NULL) {
		node->prev->next = NULL;
	}
	setVret(vr);
	delete node;
}


DR_NS_END
