/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "SimpleServer_p.hpp"

#include "xTitan/Network/SimpleSocket.hpp"


/**
 * @ingroup core
 * @class xtitan::SimpleServer
 * @brief A simple network server
 */


using xtitan::SimpleServer;
using xtitan::SimpleSocket;

/**
 * @brief Constructor
 * @param parent parent of this QObject
 */
SimpleServer::SimpleServer( QObject * parent ):
QObject( parent ),
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( newConnection() ), SIGNAL( newConnection() ) );
}

/**
 * @brief stop listening client
 * @sa listen()
 */
void SimpleServer::close() {
	this->p_->server->close();
}

/**
 * @brief get last error message
 * @return error message
 */
QString SimpleServer::errorString() const {
	return this->p_->server->errorString();
}

/**
 * @brief get pipe name
 * @return full pipe name
 */
QString SimpleServer::fullServerName() const {
	return this->p_->server->fullServerName();
}

/**
 * @brief check for more connections
 * @return true if there is any pending connection
 */
bool SimpleServer::hasPendingConnections() const {
	return !this->p_->queue.isEmpty();
}

/**
 * @brief check listening state
 * @return true if listening
 */
bool SimpleServer::isListening() const {
	return this->p_->server->isListening();
}

/**
 * @brief listen to a local server
 * @param [in] name local server name
 * @param [in] replace replace server if exists
 * @return true if listen successful
 *
 * This function listen on a local server, i.e. a named pipe.
 *
 * Under UNIX-like system, they won't remove local server automatically, so you
 * @p replace flag to force replace existing server.
 */
bool SimpleServer::listen( const QString & name, bool replace /*= false*/ ) {
	bool ok = this->p_->server->listen( name );
	if( replace && !ok && this->p_->server->serverError() == QAbstractSocket::AddressInUseError ) {
		replace = QLocalServer::removeServer( name );
		if( !replace ) {
			return false;
		}
		ok = this->p_->server->listen( name );
	}
	return ok;
}

/**
 * @brief get next pending connection
 * @return pending connection, null if no pending connection
 */
SimpleSocket * SimpleServer::nextPendingConnection() {
	if( this->hasPendingConnections() ) {
		return this->p_->queue.dequeue();
	}
	return NULL;
}

/**
 * @brief get last server error code
 * @return error code
 */
QAbstractSocket::SocketError SimpleServer::serverError() const {
	return this->p_->server->serverError();
}

/**
 * @brief get short pipe name
 * @return short pipe name
 */
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
