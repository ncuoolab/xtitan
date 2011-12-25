#ifndef QX_SPY_SOCKET_SERVER_H_
#define QX_SPY_SOCKET_SERVER_H_

#include <QObject>
#include <QTcpServer>
#include "QxTesting/TcpConnection/QxSpySocketServer.h"
#include "QxTesting/TCPConnection/QxSpySocket.h"

class QxSpySocketServer : public QObject {
	Q_OBJECT
public:
	QxSpySocketServer(int port);

	void start();
	void stop();

	QxSpySocket* getNewSocket();

signals:
	void sig_newConnectionOK();

private slots:
	void slot_acceptConnection();

private:
	int _port;
	QTcpServer* tcpServer;
	QxSpySocket* clientSocket;
};


#endif	//QX_SPY_SOCKET_SERVER_H_