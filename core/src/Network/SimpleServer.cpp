#include "SimpleServerPrivate.hpp"
#include "xTitan/Network/SimpleSocket.hpp"

using namespace xtitan::network;

SimpleServer::SimpleServer( QObject * parent ):
QObject( parent ),
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( newConnection() ), SIGNAL( newConnection() ) );
}

void SimpleServer::close() {
	this->p_->server->close();
}

QString SimpleServer::errorString() const {
	return this->p_->server->errorString();
}

QString SimpleServer::fullServerName() const {
	return this->p_->server->fullServerName();
}

bool SimpleServer::hasPendingConnections() const {
	return !this->p_->queue.isEmpty();
}

bool SimpleServer::isListening() const {
	return this->p_->server->isListening();
}

bool SimpleServer::listen( const QString & name ) {
	return this->p_->server->listen( name );
}

SimpleSocket * SimpleServer::nextPendingConntion() {
	if( this->hasPendingConnections() ) {
		return this->p_->queue.dequeue();
	}
	return NULL;
}

QAbstractSocket::SocketError SimpleServer::serverError() const {
	return this->p_->server->serverError();
}

QString SimpleServer::serverName() const {
	return this->p_->server->serverName();
}

// private part

SimpleServer::Private::Private( QObject * parent ):
QObject( parent ),
server( new QLocalServer( this ) ),
queue() {
	this->connect( this->server, SIGNAL( newConnection() ), SLOT( onNewConnection() ) );
}

void SimpleServer::Private::onNewConnection() {
	while( this->server->hasPendingConnections() ) {
		SimpleSocket * socket = new SimpleSocket( this->server->nextPendingConnection(), this );
		this->queue.enqueue( socket );
	}
	emit this->newConnection();
}
