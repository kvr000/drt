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
#include <dr/pe_error.hxx>

#include <dr/Ref.hxx>
#include <dr/Exception.hxx>
#include <dr/ThreadSimple.hxx>
#include <dr/Array.hxx>

#include <dr/io/Vfs.hxx>
#include <dr/io/File.hxx>
#include <dr/io/Directory.hxx>
#include <dr/io/IoException.hxx>
#include <dr/io/FileNotFoundException.hxx>
#include <dr/io/SocketStream.hxx>
#include <dr/io/SocketServer.hxx>
#include <dr/io/SocketDatagram.hxx>
#include <dr/io/NetAddressInet4.hxx>
#include <dr/io/NetAddressLocal.hxx>

#include <dr/tenv/tenv.hxx>

DR_IO_NS_USE;
DR_TENV_NS_USE


#define TEST_BENCHSOCKET


#ifdef TEST_BENCHSOCKET
TENV_NS(benchsocket);

static const size_t CONN_COUNT = 100000-10;

void accept_th(SocketServer *server_sock)
{
	RList<SocketStream> clients;
	xtry {
		for (size_t i = 0; i < CONN_COUNT; i++) {
			ERef<SocketStream> client(server_sock->accept());
			//Fatal::plog("accepted connection from %s\n", tref(client->getRemote())->formatAddress().utf8().toStr());
			//client->write("bleee");
			clients.append(client);
		}
		clients.clean();
	}
	xcatch (Exception, ex) {
		Fatal::plog("server: caught unexpected exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}

void test()
{
	Fatal::plog("sock stream local: in\n");
	xtry {
		ERef<NetAddressLocal> server_address(new NetAddressLocal("srv_tbenchsocket"));
		ERef<SocketServer> server_sock(new SocketServer(Socket::domain_local, Socket::proto_ip));
		xtry {
			File::unlink(server_address->getPath());
		}
		xcatch (FileNotFoundException, ex) {
		}
		xend;
		server_sock->bind(server_address);
		server_sock->listen(-1);
		ERef<Thread> srv_thread(ThreadSimple::go(Eslot(&accept_th).a1Set<IRef<SocketServer> >(server_sock)));
		RList<SocketStream> clients;
		xtry {
			for (size_t i = 0; i < CONN_COUNT; i++) {
				ERef<SocketStream> client(new SocketStream(Socket::domain_local, Socket::proto_ip));
				client->connect(server_address);
				clients.append(client);
				if ((i+1)%1000 == 0) {
					Fatal::plog("%d clients running\n", i+1);
				}
			}
			clients.clean();
		}
		xcatchany {
			srv_thread->wait();
			throw;
		}
		xend;
		srv_thread->wait();
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
	Fatal::plog("sock stream local: out\n");
}
TENV_NSE(benchsocket);
#endif

DR_TENV_MAIN()
{
	tenv_init();
	dr::MM::enableThreadCache(1);
#ifdef TEST_BENCHSOCKET
	TENV_RUN(benchsocket);
#endif
	return 0;
}
