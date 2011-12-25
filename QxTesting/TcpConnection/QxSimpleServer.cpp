#include "QxSimpleServerPrivate.h"
#include "QxSimpleSocket.h"

QxSimpleServer::QxSimpleServer( QObject * parent ):
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( newConnection( QxSimpleSocket * ) ), SIGNAL( newConnection( QxSimpleSocket * ) ) );
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

bool QxSimpleServer::isListening() const {
	return this->p_->server->isListening();
}

bool QxSimpleServer::listen( const QString & name ) {
	return this->p_->server->listen( name );
}

QAbstractSocket::SocketError QxSimpleServer::serverError() const {
	return this->p_->server->serverError();
}

QString QxSimpleServer::serverName() const {
	return this->p_->server->serverName();
}

// private part

QxSimpleServer::Private::Private( QObject * parent ):
server( new QLocalServer( this ) ) {
	this->connect( this->server, SIGNAL( newConnection() ), SLOT( onNewConnection() ) );
}

void QxSimpleServer::Private::onNewConnection() {
	while( this->server->hasPendingConnections() ) {
		QxSimpleSocket * socket = new QxSimpleSocket( this->server->nextPendingConnection(), this );
		qDebug() << "connected" << socket->fullServerName();
		emit this->newConnection( socket );
	}
}
