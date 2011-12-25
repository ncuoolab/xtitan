#include "QxSimpleServerPrivate.h"
#include "QxSimpleSocket.h"

QxSimpleServer::QxSimpleServer( QObject * parent ):
QObject( parent ),
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( newConnection() ), SIGNAL( newConnection() ) );
}

void QxSimpleServer::close() {
	this->p_->server->close();
}

QString QxSimpleServer::errorString() const {
	return this->p_->server->errorString();
}

QString QxSimpleServer::fullServerName() const {
	return this->p_->server->fullServerName();
}

bool QxSimpleServer::hasPendingConnections() const {
	return !this->p_->queue.isEmpty();
}

bool QxSimpleServer::isListening() const {
	return this->p_->server->isListening();
}

bool QxSimpleServer::listen( const QString & name ) {
	return this->p_->server->listen( name );
}

QxSimpleSocket * QxSimpleServer::nextPendingConntion() {
	if( this->hasPendingConnections() ) {
		return this->p_->queue.dequeue();
	}
	return NULL;
}

QAbstractSocket::SocketError QxSimpleServer::serverError() const {
	return this->p_->server->serverError();
}

QString QxSimpleServer::serverName() const {
	return this->p_->server->serverName();
}

// private part

QxSimpleServer::Private::Private( QObject * parent ):
QObject( parent ),
server( new QLocalServer( this ) ),
queue() {
	this->connect( this->server, SIGNAL( newConnection() ), SLOT( onNewConnection() ) );
}

void QxSimpleServer::Private::onNewConnection() {
	while( this->server->hasPendingConnections() ) {
		QxSimpleSocket * socket = new QxSimpleSocket( this->server->nextPendingConnection(), this );
		this->queue.enqueue( socket );
	}
	emit this->newConnection();
}
