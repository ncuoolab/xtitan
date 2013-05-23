#ifndef XTITAN_TESTUNIT_TESTUNIT_HPP_
#define XTITAN_TESTUNIT_TESTUNIT_HPP_

#include "TestUnit.hpp"

#include <functional>
#include <map>
#include <vector>

#include "xTitan/Network/SimpleSocket.hpp"
#include "TestUnitServer.hpp"
#include "CheckPoint.hpp"

namespace xtitan {

class TestUnit::Private: public QObject {
	Q_OBJECT
public:
	typedef std::function< void ( const QVariant & ) > Command;

	Private( int id, SimpleSocket * socket, TestUnitServer * server );
	virtual ~Private();

public slots:
	void onReadyRead();

signals:
	void asyncCheckReceived( int id, const AsyncCheckPoint & acp );
	void checkReceived( int id, const CheckPoint & cp );
	void inputReceived( int id, int delay, const QString & object, const QString & method, const QStringList & args );

public:
	int id;
	SimpleSocket * socket;
	TestUnitServer * server;
	std::map< QString, Command > commands;
	qint64 lastTimestamp;
	std::vector< CheckPoint > sutCheckPoints;
	std::vector< AsyncCheckPoint > sutAsyncCheckPoints;
	std::vector< CheckPoint > oracleCheckPoints;
	std::vector< AsyncCheckPoint > oracleAsyncCheckPoints;
};

}

#endif
