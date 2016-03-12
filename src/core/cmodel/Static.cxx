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

#include <dr/Object.hxx>
#include <dr/Static.hxx>

DR_NS_BEGIN


DR_EXPORT_MET size_t Static::getCount() const
{
	size_t count = 0;

	for (size_t i = 0; i < sizeof(ifaces)/sizeof(ifaces[0]); i++) {
		for (IfaceNode *node = ifaces[i]; node; node = node->next)
			count++;
	}
	return count;
}

DR_EXPORT_MTS Static *Static::create(const String &cname, size_t count)
{
	Static *ret = (Static *)Alloc::alloc(sizeof(Static)+count*sizeof(IfaceNode));

	new (ret) Static;
	ret->comp_name = cname;
	memset(ret->ifaces, 0, sizeof(ret->ifaces));

	return ret;
}

DR_EXPORT_MET void Static::free()
{
	/* not interested in not freeing string ids as they are constant anyway */
	Alloc::free(this);
}

DR_EXPORT_MET void Static::addNode(IfaceNode **node_mem, const String &name, SintPtr offs, void *(*create_func)(Object *obj, SintPtr offs))
{
	size_t idx = name.getHash()%(sizeof(ifaces)/sizeof(ifaces[0]));

	IfaceNode *node = new (*node_mem) IfaceNode;
	node->name = name;
	node->offs = offs;
	node->create_func = create_func;

	node->next = ifaces[idx];
	ifaces[idx] = node;

	++*node_mem;
}

DR_EXPORT_MET void Static::merge(IfaceNode **node_mem, SintPtr offs, const Static *second)
{
	for (size_t i = 0; i < sizeof(second->ifaces)/sizeof(second->ifaces[0]); i++) {
		for (IfaceNode *node = second->ifaces[i]; node; node = node->next) {
			size_t idx = node->name.getHash()%(sizeof(ifaces)/sizeof(ifaces[0]));
			for (IfaceNode *tnode = ifaces[idx]; ; tnode = tnode->next) {
				if (tnode) {
					if (tnode->name == node->name)
						break;
				}
				else {
					addNode(node_mem, node->name, offs+node->offs, node->create_func);
					break;
				}
			}
		}
	}
}


DR_NS_END
