#include "Spy/SpyModel_p.hpp"

#include "xTitan/Exception/NetworkError.hpp"
#include "Network/TcpMessageComposer.hpp"
#include "Spy.hpp"


using xtitan::SpyModel;
using xtitan::SimpleSocket;
using xtitan::command::CommandParser;


SpyModel::Private::Private( SpyModel * host ):
QObject( host ),
socket( new SimpleSocket( this ) ),
commands() {
	this->connect( this->socket, SIGNAL( connected() ), SLOT( onConnected() ) );
	this->connect( this->socket, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
}

void SpyModel::Private::send( const QxPacket & pkt ) {
	if( this->socket->state() != QLocalSocket::ConnectedState ) {
		throw NetworkError( "disconnected from server, send failed" );
	}
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
	SimpleSocket::Packet msg = TcpMessageComposer::Check( label, value );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::input( const QString & object, const QString & method, const QStringList & args, qint64 timeStamp ) {
	if( this->p_->socket->state() == QLocalSocket::ConnectedState ) {
		this->p_->send( TcpMessageComposer::Input( object, method, args, timeStamp ) );
	}
}

void SpyModel::stop() {
	this->p_->socket->disconnectFromServer();
}
