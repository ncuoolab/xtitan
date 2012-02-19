#include "Spy/SpyModelPrivate.hpp"
#include "Network/TcpMessageComposer.hpp"
#include "xTitanClient/Spy/Spy.hpp"
#include "xTitanClient/Command/MessageEncoder.hpp"

#include <cassert>

using namespace xtitan::spy;
using xtitan::network::SimpleSocket;
using xtitan::command::CommandParser;
using xtitan::command::MessageEncoder;

SpyModel::Private::Private( SpyModel * host ):
QObject( host ),
socket( new SimpleSocket( this ) ),
commands() {
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
		this->commands.parse( packet.first, packet.second );
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
	if( this->p_->socket->state() != QLocalSocket::ConnectedState ) {
		return;
	}
	QVariantMap args;
	args.insert( "label", label );
	args.insert( "value", value );
	SimpleSocket::Packet msg( MessageEncoder::getInstance().encode( "check", args ) );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::input( const QString & label, const QString & script, qint64 waitTime ) {
	if( this->p_->socket->state() != QLocalSocket::ConnectedState ) {
		return;
	}
	QVariantMap args;
	args.insert( "label", label );
	args.insert( "script", script );
	args.insert( "waitTime", waitTime );
	SimpleSocket::Packet msg( MessageEncoder::getInstance().encode( "input", args ) );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::raw( const QString & tag, const QVariant & data ) {
	if( this->p_->socket->state() != QLocalSocket::ConnectedState ) {
		return;
	}
	SimpleSocket::Packet msg( MessageEncoder::getInstance().encode( tag, data ) );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::stop() {
	this->p_->socket->disconnectFromServer();
}
