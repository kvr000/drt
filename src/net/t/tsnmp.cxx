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

#include <dr/x_kw.hxx>

#include <dr/net/SnmpAgent.hxx>

#include <dr/tenv/tenv.hxx>

DR_IO_NS_USE;
DR_NET_NS_USE;
DR_TENV_NS_USE


#define TEST_SNMP

#ifdef TEST_SNMP
TENV_NS(snmp);

void test()
{
	ERef<dr::net::SnmpAgent> agent(new dr::net::SnmpAgent("tsnmp"));
}
TENV_NSE(snmp);
#endif


DR_TENV_MAIN()
{
	tenv_init();
#ifdef TEST_SNMP
	TENV_RUN(snmp);
#endif
	return 0;
}
