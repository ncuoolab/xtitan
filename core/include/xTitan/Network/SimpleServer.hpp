/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_NETWORK_SIMPLESERVER_HPP
#define XTITAN_NETWORK_SIMPLESERVER_HPP

#include <QtCore/QObject>
#include <QtNetwork/QLocalSocket>

#include "xTitan/Utility/CoreConfig.hpp"


namespace xtitan {

class SimpleSocket;

class XTITAN_CORE_DLL SimpleServer: public QObject {
	Q_OBJECT
public:
	explicit SimpleServer( QObject * parent = 0 );

	void close();
	QString errorString() const;
	QString fullServerName() const;
	bool hasPendingConnections() const;
	bool isListening() const;
	bool listen( const QString & name, bool replace = false );
	SimpleSocket * nextPendingConnection();
	QAbstractSocket::SocketError serverError() const;
	QString serverName() const;

signals:
	void newConnection();

private:
	class Private;
	Private * p_;
};

}

#endif
