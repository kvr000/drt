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

#include <dr/testenv/testenv.hxx>

DR_IO_NS_USE;
DR_TESTENV_NS_USE


#define TEST_FILE
#define TEST_DIR
#define TEST_SOCK

#ifdef TEST_FILE
TESTNS(file);
void test()
{
	Fatal::plog("test file start\n");
	xtry {
		ERef<File> file(new File("testfile", File::M_WRITE|File::M_CREATE));
		file->write("blekeke\n");
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
	Fatal::plog("test file done\n");
}
TESTNSE(file);
#endif


#ifdef TEST_DIR
TESTNS(dir);
void test()
{
	xtry {
		ERef<Directory> dir(new Directory("."));
		String entry;
		while (dir->getNext(&entry)) {
			printf("%s\n", entry.utf8().toStr());
		}
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}
TESTNSE(dir);
#endif


#ifdef TEST_SOCK
TESTNS(sock);

void sender_th(unsigned short port)
{
	xtry {
		ERef<SocketDatagram> sock(new SocketDatagram(Socket::domain_inet4, "udp"));
		ERef<SocketDatagram> vsock(sock->openVirtual(tref(new NetAddressInet4("localhost", port))));
		//sock->connect(tref(new NetAddressInet4(0x7f000001, port)));
		Fatal::plog("sending the data...\n");
		vsock->write("tt0");
		if (vsock->waitData(Handle::M_READ, 2000000000) != 0) {
			Fatal::plog("received: %s\n", sock->read(1024).toStr());
		}
		else {
			Fatal::plog("sender: did not received any data within 2s\n");
		}
	}
	xcatch (Exception, ex) {
		Fatal::plog("sender: catched unexpected exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}

void connectInet4_th(NetAddress *server_address)
{
	ERef<SocketStream> sock(new SocketStream(Socket::domain_inet4, "ip"));
	xtry {
		sock->connect(server_address);
		sock->write("tt0");
		Fatal::plog("client read: %s\n", sock->read(1024).toStr());
	}
	xcatch (Exception, ex) {
		Fatal::plog("connect inet sender: catched unexpected exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}

void connectLocal_th(NetAddress *server_address)
{
	ERef<NetAddressLocal> client_address(new NetAddressLocal("client_socket"));
	ERef<SocketStream> sock(new SocketStream(Socket::domain_local, "ip"));
	xtry {
		File::unlink(client_address->getPath());
	}
	xcatch (FileNotFoundException, ex) {
	}
	xend;
	sock->bind(client_address);
	sock->connect(server_address);
	sock->write("tt0");
	Fatal::plog("client read: %s\n", sock->read(1024).toStr());
}

void test()
{
	Fatal::plog("sock stream: in\n");
	xtry {
		ERef<SocketStream> sock(new SocketStream(Socket::domain_inet4, "tcp"));
		sock->bind(tref(new NetAddressInet4("localhost:0")));
		sock->connect(tref(new NetAddressInet4("localhost:22")));
		sock->write("bflm\n");
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
	Fatal::plog("sock stream: out\n");

	Fatal::plog("sock stream inner: in\n");
	xtry {
		ERef<SocketServer> server(new SocketServer(Socket::domain_inet4, "tcp"));
		server->bind(tref(new NetAddressInet4("localhost", 0)));
		server->listen(-1);
		ERef<Thread> sender(ThreadSimple::go(Eslot(&connectInet4_th).a1Set<ERef<NetAddress> >(server->getLocalAddress())));
		xtry {
			ERef<SocketStream> client(server->accept());
			Fatal::plog("accepted connection from %s\n", tref(client->getRemote())->formatAddress().utf8().toStr());
			Fatal::plog("server read: %s\n", client->read(1024).toStr());
			client->write("bla");
		}
		xcatchany {
			sender->wait();
			xrethrowany;
		}
		xend;
		sender->wait();
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
	Fatal::plog("sock stream inner: out\n");

	Fatal::plog("sock datagram: in\n");
	xtry {
		ERef<NetAddressInet4> srv_addr(new NetAddressInet4("localhost:2030"));
		ERef<SocketDatagram> sock(new SocketDatagram(Socket::domain_inet4, "udp"));
		sock->bind(srv_addr);
		xtry {
			sock->write("bflm\n");
		}
		xcatch (Exception, ex) {
			Fatal::plog("caught ok exception: %s\n", ex->stringify().utf8().toStr());
		}
		xend;
		ERef<SocketDatagram> sockv(sock->openVirtual(tref(new NetAddressInet4("localhost", 804))));
		xtry {
			sockv->write("bla");
		}
		xcatch (Exception, ex) {
			Fatal::plog("caught rel exception: %s\n", ex->stringify().utf8().toStr());
		}
		xend;
		ERef<Thread> sender(ThreadSimple::go(Eslot(&sender_th).a1Set(srv_addr->getPort())));
		ERef<SocketDatagram> sockr;
		xtry {
			if (sock->waitData(Handle::M_READ, 1000000000) == 0) {
				Fatal::plog("failed to receive data in 1s\n");
			}
			else {
				Blob pack = sock->recvVirtual(sockr.mem(), 1024);
				ERef<NetAddress> sockr_remote(sockr->getRemote());
				Fatal::plog("received from %s: %s\n", ERef<NetAddress>(sockr->getRemote())->formatAddress().utf8().toStr(), pack.toStr());
				sockr->write("answer");
			}
		}
		xcatchany {
			sender->wait();
			xrethrowany;
		}
		xend;
		sender->wait();
	}
	xcatch (Exception, ex2) {
		Fatal::plog("caught exception: %s\n", ex2->stringify().utf8().toStr());
	}
	xend;
	Fatal::plog("sock datagram: out\n");

	Fatal::plog("sock stream local: in\n");
	xtry {
		ERef<NetAddressLocal> server_address(new NetAddressLocal("server_socket"));
		ERef<SocketServer> sock(new SocketServer(Socket::domain_local, "ip"));
		xtry {
			File::unlink(server_address->getPath());
		}
		xcatch (FileNotFoundException, ex) {
		}
		xend;
		sock->bind(server_address);
		sock->listen(-1);
		ERef<Thread> cl_thread(ThreadSimple::go(Eslot(&connectLocal_th).a1Set<IRef<NetAddress> >(server_address)));
		xtry {
			ERef<SocketStream> client(sock->accept());
			Fatal::plog("accepted connection from %s\n", tref(client->getRemote())->formatAddress().utf8().toStr());
			Fatal::plog("server read: %s\n", client->read(1024).toStr());
			client->write("bleee");
		}
		xcatchany {
			cl_thread->wait();
			throw;
		}
		xend;
		cl_thread->wait();
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
	Fatal::plog("sock stream local: out\n");
}
TESTNSE(sock);
#endif

DR_TESTENV_MAIN()
{
	test_init();
#ifdef TEST_FILE
	TESTRUN(file);
#endif
#ifdef TEST_DIR
	TESTRUN(dir);
#endif
#ifdef TEST_SOCK
	TESTRUN(sock);
#endif
	return 0;
}
