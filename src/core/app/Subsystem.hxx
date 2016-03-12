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

#ifndef dr__Subsystem__hxx__
# define dr__Subsystem__hxx__

#include <dr/Object.hxx>

DR_NS_BEGIN


/**
 * Subsystem object
 *
 * Subsystem allows dynamic loading and use of particular functionality
 * implementation
 */
class DR_PUB Subsystem: public Object
{
public:
	static Subsystem *		getSubsystem(const String &name, Subsystem *(*create_func)(const String &name));
	void				releaseSubsystem();

	static void			s_init();

protected:
	/* constructor */		Subsystem();
	virtual				~Subsystem();

public:
	DR_RINLINE void			busy_ref()				{ Atomic::inc(&busy_count); }
	DR_RINLINE void			busy_unref()				{ Atomic::dec(&busy_count); }

public:
	virtual Object *		create(const String &object);
	virtual void *			create(const String &object, const String &iface);

protected:
	Refcnt				busy_count;
};


DR_NS_END

#endif
