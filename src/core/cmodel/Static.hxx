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

#ifndef dr__Static__hxx__
# define dr__Static__hxx__

DR_NS_BEGIN


class Throwable;

/**
 * object static information
 */
struct DR_PUB Static
{
	struct IfaceNode
	{
		String				name;
		SintPtr				offs;
		void *				(*create_func)(Object *obj, SintPtr offs);

		IfaceNode *			next;
	};

	String				comp_name;

	IfaceNode *			ifaces[32];

	size_t				getCount() const;
	void				addNode(IfaceNode **node_mem, const String &name, SintPtr offs, void *(*create_func)(Object *obj, SintPtr offs));
	void				merge(IfaceNode **node_mem, SintPtr offs, const Static *second);

	static Static *			create(const String &cname, size_t count);
	void				free();
};


DR_NS_END

#endif
