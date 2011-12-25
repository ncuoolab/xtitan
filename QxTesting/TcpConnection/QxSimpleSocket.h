#ifndef QXSIMPLESOCKET_H
#define QXSIMPLESOCKET_H

#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtNetwork/QLocalSocket>

typedef QPair< QString, QVariant > QxPacket;

class QxSimpleSocket : public QObject {
	Q_OBJECT
public:
	explicit QxSimpleSocket( QObject * parent = 0 );

	void abort();
	bool atEnd() const;
	void close();
	void connectToServer( const QString & name, QIODevice::OpenMode openMode = QIODevice::ReadWrite );
	void disconnectFromServer();
	QLocalSocket::LocalSocketError error() const;
	QString errorString() const;
	bool flush();
	QString fullServerName() const;
	bool isValid() const;
	QString serverName() const;
	QLocalSocket::LocalSocketState state() const;
	QxPacket read();
	bool waitForReadyRead( int msecs = 30000 );
	void write( const QString & command, const QVariant & data ) const;

signals:
	void connected();
	void disconnected();
	void error( QLocalSocket::LocalSocketError socketError );
	void readyRead();
	void stateChanged( QLocalSocket::LocalSocketState socketState );

private:
	friend class QxSimpleServer;
	QxSimpleSocket( QLocalSocket *, QObject * );
	class Private;
	Private * p_;
};

#endif
