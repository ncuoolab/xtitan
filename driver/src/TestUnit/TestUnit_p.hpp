#ifndef XTITAN_TESTUNIT_TESTUNIT_HPP_
#define XTITAN_TESTUNIT_TESTUNIT_HPP_

#include "TestUnit.hpp"

#include <functional>
#include <map>
#include <vector>

#include "xTitan/Network/SimpleSocket.hpp"
#include "TestUnitServer.hpp"

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
	void checkReceived( int id, const QString & label, const QString & value );
	void inputReceived( int id, int delay, const QString & object, const QString & method, const QStringList & args );

public:
	int id;
	SimpleSocket * socket;
	TestUnitServer * server;
	std::map< QString, Command > commands;
	qint64 lastTimestamp;
	std::vector< QString > sutCheckPoints;
	std::vector< QString > oracleCheckPoints;
};

}

#endif
