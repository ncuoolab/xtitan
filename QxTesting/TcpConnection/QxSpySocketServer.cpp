#include "QxTesting/TcpConnection/QxSpySocketServer.h"
#include <QMessageBox>
#include <QtNetwork>

QxSpySocketServer::QxSpySocketServer(int port)
:_port(port)
{}

void QxSpySocketServer::start(){
	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) {
				ipAddress = ipAddressesList.at(i).toString();
				break;
		}
	}
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

	tcpServer = new QTcpServer(this);
	if (!tcpServer->listen(QHostAddress::Any, this->_port )) {
		/*QMessageBox::critical(this, tr("Oracle Server"),
		tr("Unable to start the server: %1.")
		.arg(tcpServer->errorString()));*/
		tcpServer->close();
		return;
	} 
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slot_acceptConnection()));
}

void QxSpySocketServer::stop(){
	tcpServer->close();
}

void QxSpySocketServer::slot_acceptConnection(){
	QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
	clientSocket = new QxSpySocket(clientConnection);
	emit sig_newConnectionOK();
}

QxSpySocket* QxSpySocketServer::getNewSocket(){
	return clientSocket;
}