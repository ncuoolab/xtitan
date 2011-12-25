#include "TCP_Connection/TestClient.h"
#include "TestUnit/QxTestUnit.h"
#include "TestUnitManager/TestUnitManager.h"
#include "Setting/Setting.h"
#include "QxSimpleSocket.h"
#include "TCPMessage.h"
#include "TcfDecoder/QxInputPoint.h"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QVariant>

TestClient::TestClient( QxSimpleSocket * socket ):
unit_( NULL ),
socket_( socket ),
closed_( false ) {
	this->connect( this->socket_, SIGNAL( messageReceived( const QString &, const QVariant & ) ), SLOT( onMessageReceived_( const QString &, const QVariant & ) ) );
	this->connect( this->socket_, SIGNAL( error( QLocalSocket::LocalSocketError ) ), SLOT( onSocketError_( QLocalSocket::LocalSocketError ) ) );
	this->connect( this->socket_, SIGNAL( disconnected() ), SIGNAL( disconnected() ) );

	//request
	this->commands_.insert( TCPMessage::ReqJoinUnit, std::tr1::bind( &TestClient::doReqJoinUnit_, this, std::tr1::placeholders::_1 ) );

	//operation
	this->commands_.insert( TCPMessage::Check, std::tr1::bind( &TestClient::doCheckCmd_, this, std::tr1::placeholders::_1 ) );
	this->commands_.insert( TCPMessage::Input, std::tr1::bind( &TestClient::doInputCmd_, this, std::tr1::placeholders::_1 ) );
}

TestClient::~TestClient(){
}

void TestClient::setTestUnit( QxTestUnit * u ){
	this->unit_ = u;
}

void TestClient::setID( int id ) {
	this->id_ = id;
}

int TestClient::getID() const {
	return this->id_;
}

QString TestClient::getIP() const {
	return this->socket_->fullServerName();
}

void TestClient::AckNextInput( const QxInputPoint & input ) const {
	this->socket_->write( TCPMessage::Input, input.getScript() );
}

void TestClient::AckQuit() {
	this->socket_->write( TCPMessage::QuitApplicationTag, QVariant() );
	this->closed_ = true;
}

void TestClient::disconnectFromHost() {
	this->socket_->disconnectFromServer();
}

void TestClient::onMessageReceived_( const QString & header, const QVariant & data ) {
	CommandTable::const_iterator it = this->commands_.find( header );
	if( it == this->commands_.end() ) {
		assert( !"UNKNOWN COMMAND" );
	} else {
		it.value()( data );
	}
}

void TestClient::doCheckCmd_( const QVariant & data ) {
	if( this->closed_ ) {
		return;
	}

	QVariantMap args( data.toMap() );
	QString label( args["label"].toString() );
	QString value( args["value"].toString() );
	this->unit_->check( this->getID(), label, value );
}

void TestClient::doInputCmd_( const QVariant & data ) {
	if( this->unit_->getState() & QxTestUnit::PLAY || this->closed_ ) {
		return;
	}
	QVariantMap args( data.toMap() );
	this->unit_->record( this->getID(), args["label"].toString(), args["script"].toString(), args["waitTime"].toLongLong() );
}

void TestClient::doReqJoinUnit_( const QVariant & data ) {
	QString joinName = data.toString();
//	QxTestUnit * unit = TestUnitManager::getInstance()->getTestUnit();
	this->socket_->write( TCPMessage::Success, QVariant() );

//	unit->addClient( this );
}

void TestClient::onSocketError_( QLocalSocket::LocalSocketError /*code*/ ) {
	emit this->error( this->socket_->errorString() );
}
