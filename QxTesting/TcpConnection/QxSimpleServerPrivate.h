#ifndef QXSIMPLESERVERPRIVATE_H
#define QXSIMPLESERVERPRIVATE_H

#include "QxSimpleServer.h"

#include <QtNetwork/QLocalServer>

class QxSimpleServer::Private : public QObject {
	Q_OBJECT
public:
	explicit Private( QObject * parent );

public slots:
	void onNewConnection();

signals:
	void newConnection( QxSimpleSocket * socket );

public:
	QLocalServer * server;
};

#endif
