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

#include <stdarg.h>
#include <stdio.h>

#include <dr/Iface.hxx>
#include <dr/Static.hxx>
#include <dr/Object.hxx>

DR_NS_BEGIN


DR_EXPORT_MTS const Static *Iface::createIfaceStatic(const String &iface_name, const Static **out, ...)
{
	size_t count;
	va_list args;
	Static *os;
	Static::IfaceNode *free_node;

	if (*out)
		return *out;

	count = 1;
	va_start(args, out);
	for (;;) {
		SintPtr offs = va_arg(args, SintPtr);
		if (offs >= 0) {
			Static *defs = va_arg(args, Static *);
			if (!defs)
				break;
			count += defs->getCount();
		}
		else if (offs < 0) {
			va_arg(args, String *);
			va_arg(args, void *); // removed due to buggy VC: void *(*)(Object *, SintPtr)
			count++;
		}
	}
	va_end(args);

	os = Static::create(iface_name, count);
	free_node = (Static::IfaceNode *)(void *)(os+1);

	os->addNode(&free_node, iface_name, 0, NULL);
	va_start(args, out);
	for (;;) {
		SintPtr offs = va_arg(args, SintPtr);
		if (offs >= 0) {
			Static *defs = va_arg(args, Static *);
			if (!defs)
				break;
			os->merge(&free_node, offs, defs);
		}
		else if (offs < 0) {
			String *if_name = va_arg(args, String *);
			void *(*create_func)(Object *, SintPtr) = (void *(*)(Object *, SintPtr))va_arg(args, void *);
			os->addNode(&free_node, *if_name, offs, create_func);
		}
	}
	va_end(args);

	if (!Atomic::cmpxchg((void **)out, NULL, os))
		os->free();

	return *out;
}


DR_NS_END
