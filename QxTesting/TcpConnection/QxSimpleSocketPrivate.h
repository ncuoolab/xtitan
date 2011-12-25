#ifndef QXSIMPLESOCKETPRIVATE_H
#define QXSIMPLESOCKETPRIVATE_H

#include "QxSimpleSocket.h"

#include <QtNetwork/QLocalSocket>
#include <QtCore/QQueue>

class QxSimpleSocket::Private : public QObject {
	Q_OBJECT
public:
	Private( QLocalSocket * socket, QObject * parent );

	bool readBlockSize();

public slots:
	void onReadyRead();

signals:
	void readyRead();

public:
	QLocalSocket * socket;
	QByteArray buffer;
	qint64 blockSize;
	QDataStream din;
	QQueue< QxPacket > queue;
};

#endif
