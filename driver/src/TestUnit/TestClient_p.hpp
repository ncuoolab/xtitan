#ifndef XTITAN_TESTUNIT_TESTCLIENT_HPP_
#define XTITAN_TESTUNIT_TESTCLIENT_HPP_

#include "TestClient.hpp"

#include <functional>
#include <map>
#include <vector>

#include "xTitan/Network/SimpleSocket.hpp"
#include "TestServer.hpp"
#include "CheckPoint.hpp"

namespace xtitan {

class TestClient::Private: public QObject {
	Q_OBJECT
public:
	typedef std::function< void ( const QVariant & ) > Command;

	Private( int id, SimpleSocket * socket, TestServer * server );
	virtual ~Private();

public slots:
	void onReadyRead();

signals:
	void asyncCheckReceived( int id, const xtitan::AsyncCheckPoint & acp );
	void checkReceived( int id, const xtitan::CheckPoint & cp );
	void inputReceived( int id, int delay, const QString & object, const QString & method, const QStringList & args );

public:
	int id;
	SimpleSocket * socket;
	TestServer * server;
	std::map< QString, Command > commands;
	qint64 lastTimestamp;
	std::vector< CheckPoint > sutCPs;
	std::vector< AsyncCheckPoint > sutACPs;
	std::vector< CheckPoint > oracleCPs;
	std::vector< AsyncCheckPoint > oracleACPs;
};

}

#endif
