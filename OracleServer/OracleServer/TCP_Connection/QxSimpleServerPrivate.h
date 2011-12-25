#ifndef QXSIMPLESERVERPRIVATE_H
#define QXSIMPLESERVERPRIVATE_H

#include "QxSimpleServer.h"

#include <QtNetwork/QLocalServer>
#include <QtCore/QQueue>

class QxSimpleServer::Private : public QObject {
	Q_OBJECT
public:
	explicit Private( QObject * parent );

public slots:
	void onNewConnection();

signals:
	void newConnection();

public:
	QLocalServer * server;
	QQueue< QxSimpleSocket * > queue;
};

#endif
