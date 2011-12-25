#include "QxTesting/TCPConnection/QxSpySocket.h"
#include <QtGui>
#include <QtNetwork>
#include "QxTesting/SpyModule/Controller/QxSpy.h"

QxSpySocket::QxSpySocket( QString ip, int port, QObject *parent)
: _ip(ip), _port(port)
{
	blockSize = 0;
	_isConnect = false;
	connection = new QTcpSocket(this);
	readStream.setDevice(connection);
	connect(connection, SIGNAL(connected()),this, SLOT(slot_connectOK() ));
	connect(connection, SIGNAL(readyRead()),this, SLOT(slot_readMsg()), Qt::QueuedConnection);
	connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(slot_displayError(QAbstractSocket::SocketError)));
	connect(connection, SIGNAL(disconnected()),	connection, SLOT(deleteLater()));	
}

QxSpySocket::QxSpySocket(QTcpSocket* sck)
{
	blockSize = 0;
	_isConnect = false;
	connection = sck;
	readStream.setDevice(connection);
	connect(connection, SIGNAL(connected()),this, SLOT(slot_connectOK() ));
	connect(connection, SIGNAL(readyRead()),this, SLOT(slot_readMsg()), Qt::QueuedConnection);
	connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(slot_displayError(QAbstractSocket::SocketError)));
	connect(connection, SIGNAL(disconnected()),	connection, SLOT(deleteLater()));	
}

QxSpySocket::~QxSpySocket(){
	if (_isConnect){
		connection->disconnectFromHost();
		emit sig_ConnectionUpdate("Disconnected");
	}
}

void QxSpySocket::SetConnect(QString ip, int port ){
	_ip = ip;
	_port = port;
}

void QxSpySocket::Connect(){
	connection->connectToHost(_ip,_port);
	connection->waitForConnected();
}


void QxSpySocket::sendMsg(QString msg){
	if(_isConnect){
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16)0;
		out << msg;
		out.device()->seek(0);
		out << (quint16)(block.size() - sizeof(quint16));
		
		while (!connection->isWritable()){}

		connection->write(block);
		connection->waitForBytesWritten();		
	}
}

void QxSpySocket::slot_readMsg(){

	while(connection->bytesAvailable() ){
		if (blockSize == 0) {
			if (connection->bytesAvailable() < (int)sizeof(quint16))
				return;
			readStream >> blockSize;
		}
		if (connection->bytesAvailable() < blockSize)
			return;

		QString msg = "";
		readStream >> msg;
		blockSize = 0;
		emit sig_getMsg(msg);
	}

}

void QxSpySocket::slot_connectOK(){
	_isConnect = true;
	emit sig_ConnectionUpdate("Connected");
}

void QxSpySocket::slot_displayError(QAbstractSocket::SocketError socketError){
	switch (socketError) {
	 case QAbstractSocket::RemoteHostClosedError:
		 break;
	 case QAbstractSocket::HostNotFoundError:
		 QMessageBox::information(0, tr("SpySocket"),
			 tr("The host was not found. Please check the "
			 "host name and port settings."));
		 break;
	 case QAbstractSocket::ConnectionRefusedError:
		 QMessageBox::information( 0, tr("SpySocket"),
			 tr("The connection was refused by the peer. "
			 "Make sure the oracle server is running, "
			 "and check that the host name and port "
			 "settings are correct."));
		 break;
	 default:
		 QMessageBox::information(0, tr("SpySocket"),
			 tr("The following error occurred: %1.")
			 .arg(connection->errorString()));
	}
	emit sig_ConnectionUpdate("Error");
}