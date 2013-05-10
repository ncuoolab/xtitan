#ifndef XTITAN_NETWORK_SIMPLESOCKETPRIVATE_HPP
#define XTITAN_NETWORK_SIMPLESOCKETPRIVATE_HPP

#include "xTitan/Network/SimpleSocket.hpp"

#include <QtCore/QQueue>
#include <QtNetwork/QLocalSocket>


namespace xtitan {

class SimpleSocket::Private : public QObject {
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
	QQueue< Packet > queue;
};

}

#endif
