/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TESTUNIT_TESTSERVER_HPP_
#define XTITAN_TESTUNIT_TESTSERVER_HPP_

#include "TestServer.hpp"

#include <vector>

#include "xTitan/Network/SimpleServer.hpp"
#include "TestClient.hpp"


namespace xtitan {

class TestServer::Private: public QObject {
	Q_OBJECT
public:
	Private( TestServer * owner );

public slots:
	void onNewConnection();
	void onClientDisconnected();

public:
	TestServer * owner;
	SimpleServer * server;
	std::vector< TestClient * > clients;
	bool recording;
};

}

#endif
