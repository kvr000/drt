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

#ifndef dr__Shared__hxx__
# define dr__Shared__hxx__

#include <dr/types.hxx>
#include <dr/Atomic.hxx>
#include <dr/Lockp.hxx>

DR_NS_BEGIN


/**
 * Shared object
 *
 * Base support for sharing the data by reference count
 */
class DR_PUB Shared: public Base
{
protected:
	Refcnt			refcnt;

public:
	/* constructor */		Shared()				: refcnt(0) {}
	DR_MINLINE virtual		~Shared()				{}
	DR_MINLINE virtual bool		unrefed()				{ delete this; return false; }

	DR_MINLINE Shared *		i_ref()					{ DR_LOG2(("%s: referencing %p, new refcnt %d+1\n", DR_FUNCTION, this, refcnt+1)); Atomic::inc(&refcnt); return this; }
	DR_MINLINE bool			i_unref()				{ DR_LOG2(("%s: unreferencing %p, new refcnt %d+1\n", DR_FUNCTION, this, refcnt-1)); if (!Atomic::decr(&refcnt)) return unrefed(); else return true; }
	bool				unref();
	DR_MINLINE Shared *		ref()					{ DR_LOG2(("%s: referencing %p, new refcnt %d+1\n", DR_FUNCTION, this, refcnt+1)); Atomic::inc(&refcnt); return this; }
};


DR_NS_END

#endif
