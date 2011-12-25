#include "TestUnit/TestUnitManager.h"
#include "OracleServerView.h"
#include "TestUnit/TestUnit.h"
#include <QtNetwork>
#include <QMessageBox>

TestUnitManager* TestUnitManager::singleton = NULL ; 

TestUnitManager* TestUnitManager::getInstance(){
	if (singleton == NULL)
		singleton = new TestUnitManager();
	return singleton;
}

//constructor
TestUnitManager::TestUnitManager(){
	view = new OracleServerView();
	view->show();
	this->createTcpConnection(8888);
}

void TestUnitManager::createTcpConnection(int port){

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

	tcpServer = new QTcpServer();
	if (!tcpServer->listen(QHostAddress::Any,port)) {
		/*
		QMessageBox::critical(this, tr("Oracle Server"),
			tr("Unable to start the server: %1.")
			.arg(tcpServer->errorString()));
		*/
		tcpServer->close();
		return;
	}
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slot_acceptConnection()));
}

void TestUnitManager::destroyTcpConnection(){
	tcpServer->close();
}

void TestUnitManager::createTestUnit(QTcpSocket* sck){
	TestUnit* unit = new TestUnit(sck);
	unitList.append(unit);
}

void TestUnitManager::destroyTestUnit(){

}

void TestUnitManager::appendLog(QString msg){
	view->appendLog(msg);
}

//slots 
void TestUnitManager::slot_acceptConnection(){
	QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
	this->createTestUnit(clientConnection);
}
