
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
 * include:	dr/Hash.hxx
 * include:	dr/net/def_net.hxx
 * include:	dr/io/NetAddress.hxx
 *
 * ns:		dr::net
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>


#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/MessagedException.hxx>
#include <dr/EndOfDataException.hxx>
#include <dr/InvalidFormatException.hxx>

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/HttpException.hxx>

#include <dr/net/SnmpAgent.hxx>
#include "_gen/SnmpAgent-all.hxx"

DR_NET_NS_BEGIN


/*drt
 * class:	SnmpAgent
 * type:	object
 * ancestor:	dr::Object
 *
 * at:	String				agent_name;
 */

DR_MET(public)
SnmpAgent::SnmpAgent(const String &agent_name_):
	agent_name(agent_name_)
{
	if (init_agent(agent_name.utf8().toStr()) != 0)
		xthrownew(MessagedException("failed to init_agent"));
	init_snmp(agent_name.utf8().toStr());
}

DR_MET(protected virtual)
SnmpAgent::~SnmpAgent()
{
	snmp_shutdown(agent_name.utf8().toStr());
}


DR_NET_NS_END
