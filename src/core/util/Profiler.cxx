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

/*drt
 * include:	dr/Time.hxx
 *
 * ns:		dr
 */

/*drt
 * class:	Profiler
 * type:	struct
 *
 * co:{
 * 	#define DR_PROFILE(name, ind) Profiler name(DR_FUNCTION, #name)
 * 	#define DR_PROFILE_FUNC() Profiler func_profiler(DR_FUNCTION, "full")
 * }co
 *
 * at:	const char *			function_name;
 * at:	const char *			part_name;
 * at:	Time::SysTime			start_time;
 */

#include <dr/Ref.hxx>

#include <dr/Profiler.hxx>
#include "_gen/Profiler-all.hxx"

DR_NS_BEGIN


DR_MET(public)
Profiler::Profiler(const char *function, const char *part):
	function_name(function),
	part_name(part),
	start_time(Time::getTime())
{
	Fatal::plog("start %s.%s at %lld\n", function_name, part_name, (long long)Time::getTime());
}

DR_MET(public)
Profiler::~Profiler()
{
	Time::SysTime t = Time::getTime();
	Fatal::plog("leave %s.%s at %lld (%lld)\n", function_name, part_name, (long long)t, (long long)(t-start_time));
}

DR_MET(public)
void Profiler::restart()
{
}

DR_MET(protected)
void Profiler::print()
{
}


DR_NS_END
