#ifndef QXSIMPLESERVER_H
#define QXSIMPLESERVER_H

#include <QtCore/QObject>
#include <QtNetwork/QLocalSocket>

class QxSimpleSocket;

class QxSimpleServer : public QObject {
	Q_OBJECT
public:
	explicit QxSimpleServer( QObject * parent = 0 );

	void close();
	QString errorString() const;
	QString fullServerName() const;
	bool hasPendingConnections() const;
	bool isListening() const;
	bool listen( const QString & name );
	QxSimpleSocket * nextPendingConntion();
	QAbstractSocket::SocketError serverError() const;
	QString serverName() const;

signals:
	void newConnection();

private:
	class Private;
	Private * p_;
};

#endif
