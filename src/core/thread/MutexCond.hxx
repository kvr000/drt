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

#ifndef dr__MutexCond__hxx__
# define dr__MutexCond__hxx__

#include <dr/Shared.hxx>

DR_NS_BEGIN


/**
 * Mutex-Condition pair interface
 */
class DR_PUB MutexCond: public Shared
{
public:
	static MutexCond *		create();

public:
	DR_MINLINE void			close()					{ unref(); }
	virtual void			lock() = 0;
	virtual void			unlock() = 0;
	virtual void			wait() = 0;
	virtual void			signal() = 0;
	virtual void			broadcast() = 0;

protected:
	DR_MINLINE			MutexCond() {}
	DR_MINLINE virtual		~MutexCond()				{}

private:
	/* disallowed */		MutexCond(const MutexCond &);
};


DR_NS_END

#endif
