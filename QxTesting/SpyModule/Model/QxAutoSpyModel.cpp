#include "QxTesting/SpyModule/Model/QxAutoSpyModel.h"
#include "QxTesting/SpyModule/Controller/QxSpy.h"
#include "QxTesting/QxTestDefinition.h"
#include "QxTesting/CommandParser/QxCommandParser.h"
#include <cassert>

QxAutoSpyModel::QxAutoSpyModel( QScriptEngine* eng ):
QxSpyModel( eng ),
socket_( new QxSimpleSocket( this ) ) {
	this->connect( this->socket_, SIGNAL( connected() ), SLOT( onConnected_() ) );
	this->connect( this->socket_, SIGNAL( readyRead() ), SLOT( onReadyRead_() ) );
}

QxAutoSpyModel::~QxAutoSpyModel(){
	this->socket_->disconnectFromServer();
}

void QxAutoSpyModel::connectToHost( const QString & name ) {
	this->socket_->connectToServer( name );
}

void QxAutoSpyModel::onConnected_() {
//	QxPacket msg = QxTcpMessageComposer::JoinUnit( this->fileName_ );
//	this->socket_->sendMessage( msg.first, msg.second );

	emit this->ready();
}

void QxAutoSpyModel::onReadyRead_() {
	while( !this->socket_->atEnd() ) {
		QxPacket packet( this->socket_->read() );
		QxCommandParser::getInstance()->parse( packet.first, packet.second );
	}
}

void QxAutoSpyModel::check( const QString & label, const QString & value ) {
	QString sValue( value );
	sValue.remove( '#' );
	sValue.remove( '\r' );
	sValue.remove( '\n' );
	QxPacket msg( QxTcpMessageComposer::Check( label, sValue ) );
	this->socket_->write( msg.first, msg.second );
}

void QxAutoSpyModel::input( const QString & label, const QString & script, qint64 waitTime ) {
	QString sScript( script );
	sScript.remove( '#' );
	sScript.remove( '\r' );
	sScript.remove( '\n' );
	if( this->socket_->state() == QAbstractSocket::ConnectedState ) {
		this->send_( QxTcpMessageComposer::Input( label, sScript, waitTime ) );
	}
}

void QxAutoSpyModel::Stop() {
	this->socket_->disconnectFromServer();
}

void QxAutoSpyModel::send_( const QxPacket & pkt ) {
	assert( this->socket_->state() == QAbstractSocket::ConnectedState );
	this->socket_->write( pkt.first, pkt.second );
}
