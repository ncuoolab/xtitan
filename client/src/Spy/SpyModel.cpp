#include "Spy/SpyModelPrivate.hpp"
#include "CommandParser/CommandParser.hpp"
#include "Network/TcpMessageComposer.hpp"
#include "xTitanClient/Spy/Spy.hpp"

#include <cassert>

using namespace xtitan::spy;
using xtitan::network::SimpleSocket;

SpyModel::Private::Private( SpyModel * host ):
QObject( host ),
socket( new SimpleSocket( this ) ) {
	this->connect( this->socket, SIGNAL( connected() ), SLOT( onConnected() ) );
	this->connect( this->socket, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
}

void SpyModel::Private::send( const QxPacket & pkt ) {
	assert( this->socket->state() == QLocalSocket::ConnectedState );
	this->socket->write( pkt.first, pkt.second );
}

void SpyModel::Private::onConnected() {
	emit this->ready();
}

void SpyModel::Private::onReadyRead() {
	while( !this->socket->atEnd() ) {
		SimpleSocket::Packet packet( this->socket->read() );
		// FIXME
//		QxCommandParser::getInstance()->parse( packet.first, packet.second );
	}
}

SpyModel::SpyModel():
QObject(),
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( ready() ), SIGNAL( ready() ) );
}

SpyModel::~SpyModel(){
	this->p_->socket->disconnectFromServer();
}

void SpyModel::connectToHost( const QString & name ) {
	this->p_->socket->connectToServer( name );
}

void SpyModel::check( const QString & label, const QString & value ) {
	QString sValue( value );
	sValue.remove( '#' );
	sValue.remove( '\r' );
	sValue.remove( '\n' );
	SimpleSocket::Packet msg( TcpMessageComposer::Check( label, sValue ) );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::input( const QString & label, const QString & script, qint64 waitTime ) {
	QString sScript( script );
	sScript.remove( '#' );
	sScript.remove( '\r' );
	sScript.remove( '\n' );
	if( this->p_->socket->state() == QLocalSocket::ConnectedState ) {
		this->p_->send( TcpMessageComposer::Input( label, sScript, waitTime ) );
	}
}

void SpyModel::stop() {
	this->p_->socket->disconnectFromServer();
}
