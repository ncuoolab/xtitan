#ifndef XTITAN_TESTUNIT_TESTUNITSERVER_HPP_
#define XTITAN_TESTUNIT_TESTUNITSERVER_HPP_

#include "TestUnitServer.hpp"

#include <vector>

#include "xTitan/Network/SimpleServer.hpp"
#include "TestUnit.hpp"


namespace xtitan {

class TestUnitServer::Private: public QObject {
	Q_OBJECT
public:
	Private( TestUnitServer * owner );

public slots:
	void onNewConnection();
	void onClientDisconnected();

public:
	TestUnitServer * owner;
	SimpleServer * server;
	std::vector< TestUnit * > clients;
	bool recording;
};

}

#endif
