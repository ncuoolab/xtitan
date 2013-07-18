/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_NETWORK_SIMPLESERVER_PRIVATE_HPP
#define XTITAN_NETWORK_SIMPLESERVER_PRIVATE_HPP

#include "xTitan/Network/SimpleServer.hpp"

#include <QtCore/QQueue>
#include <QtNetwork/QLocalServer>

namespace xtitan {

class SimpleServer::Private: public QObject {
	Q_OBJECT
public:
	explicit Private( QObject * parent );

public slots:
	void onNewConnection();

signals:
	void newConnection();

public:
	QLocalServer * server;
	QQueue< SimpleSocket * > queue;
};

}

#endif
