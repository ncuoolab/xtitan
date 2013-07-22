/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "SimpleSocket_p.hpp"

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include "xTitan/Exception/NetworkError.hpp"


/**
 * @ingroup core
 * @class xtitan::SimpleSocket
 * @brief A simple network client
 */


using xtitan::SimpleSocket;


/**
 * @brief constructor
 * @param [in] parent parent of this QObject
 */
SimpleSocket::SimpleSocket( QObject * parent ):
QObject( parent ),
p_( new Private( new QLocalSocket( this ), this ) ) {
	this->connect( this->p_->socket, SIGNAL( connected() ), SIGNAL( connected() ) );
	this->connect( this->p_->socket, SIGNAL( disconnected() ), SIGNAL( disconnected() ) );
	this->connect( this->p_->socket, SIGNAL( error( QLocalSocket::LocalSocketError ) ), SIGNAL( error( QLocalSocket::LocalSocketError ) ) );
	this->connect( this->p_, SIGNAL( readyRead() ), SIGNAL( readyRead() ) );
}

SimpleSocket::SimpleSocket( QLocalSocket * socket, QObject * parent ):
QObject( parent ),
p_( new Private( socket, this ) ) {
	this->connect( this->p_->socket, SIGNAL( connected() ), SIGNAL( connected() ) );
	this->connect( this->p_->socket, SIGNAL( disconnected() ), SIGNAL( disconnected() ) );
	this->connect( this->p_->socket, SIGNAL( error( QLocalSocket::LocalSocketError ) ), SIGNAL( error( QLocalSocket::LocalSocketError ) ) );
	this->connect( this->p_, SIGNAL( readyRead() ), SIGNAL( readyRead() ) );

//	socket->connect( socket, SIGNAL( disconnected() ), SLOT( deleteLater() ) );
}

/**
 * @brief abort connection
 * @sa close()
 */
void SimpleSocket::abort() {
	this->p_->socket->abort();
}

/**
 * @brief check if no pending data
 * @return true if no pending data
 */
bool SimpleSocket::atEnd() const {
	return this->p_->queue.isEmpty();
}

/**
 * @brief close connection
 * @sa abort()
 */
void SimpleSocket::close() {
	this->p_->socket->close();
}

/**
 * @brief connect to a server
 * @param [in] name server name
 * @param [in] openMode read or write
 */
void SimpleSocket::connectToServer( const QString & name, QIODevice::OpenMode openMode /* = QIODevice::ReadWrite */ ) {
	this->p_->socket->connectToServer( name, openMode );
}

/**
 * @brief disconnect from server
 */
void SimpleSocket::disconnectFromServer() {
	this->p_->socket->disconnectFromServer();
}

/**
 * @brief get last error code
 * @return last error code
 */
QLocalSocket::LocalSocketError SimpleSocket::error() const {
	return this->p_->socket->error();
}

/**
 * @brief get last error message
 * @return last error message
 */
QString SimpleSocket::errorString() const {
	return this->p_->socket->errorString();
}

/**
 * @brief flush pending socket
 * @return true if data flushed
 */
bool SimpleSocket::flush() {
	return this->p_->socket->flush();
}

/**
 * @brief get full server name
 * @return full server name
 */
QString SimpleSocket::fullServerName() const {
	return this->p_->socket->fullServerName();
}

/**
 * @brief check socket state
 * @return true if socket is still connecting
 */
bool SimpleSocket::isValid() const {
	return this->p_->socket->isValid();
}

/**
 * @brief get short server name
 * @return short server name
 */
QString SimpleSocket::serverName() const {
	return this->p_->socket->serverName();
}

/**
 * @brief get socket state
 * @return current socket state
 */
QLocalSocket::LocalSocketState SimpleSocket::state() const {
	return this->p_->socket->state();
}

/**
 * @brief read one data
 * @return a packet
 */
SimpleSocket::Packet SimpleSocket::read() {
	if( this->p_->queue.isEmpty() ) {
		throw NetworkError( "no message to read" );
	}
	return this->p_->queue.dequeue();
}

/**
 * @brief block until read something
 * @return true if read something and not timeout
 */
bool SimpleSocket::waitForReadyRead( int msecs /* = 30000 */ ) {
	QTimer * timer = new QTimer( this );
	timer->setSingleShot( true );
	timer->setInterval( msecs );
	QEventLoop * wait = new QEventLoop( this );
	wait->connect( this, SIGNAL( readyRead() ), SLOT( quit() ) );
	wait->connect( timer, SIGNAL( timeout() ), SLOT( quit() ) );
	timer->start( msecs );
	wait->exec();
	timer->deleteLater();
	wait->deleteLater();

	return !this->atEnd();
}

/**
 * @brief send packet
 * @param [in] header packet header
 * @param [in] data packet data
 */
void SimpleSocket::write( const QString & header, const QVariant & data ) const {
	QByteArray block;
	QDataStream dout( &block, QIODevice::WriteOnly );
	dout << static_cast< qint64 >( 0 ) << header << data;
	dout.device()->seek( 0 );
	dout << static_cast< qint64 >( block.size() - static_cast< int >( sizeof( qint64 ) ) );

	this->p_->socket->write( block );
	// NOTE: flush well randomly send twice
//	this->p_->socket->flush();
}

SimpleSocket::Private::Private( QLocalSocket * socket, QObject * parent ):
QObject( parent ),
socket( socket ),
buffer(),
blockSize( 0 ),
din( &this->buffer, QIODevice::ReadOnly ),
queue() {
	this->connect( this->socket, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
}

bool SimpleSocket::Private::readBlockSize() {
	if( this->blockSize <= 0 && this->buffer.size() >= static_cast< int >( sizeof( this->blockSize ) ) ) {
		this->din >> this->blockSize;
		this->din.device()->seek( 0 );
		this->buffer.remove( 0, sizeof( this->blockSize ) );
	}
	return this->blockSize > 0 && this->buffer.size() >= this->blockSize;
}

void SimpleSocket::Private::onReadyRead() {
	this->buffer.append( this->socket->readAll() );

	while( this->readBlockSize() ) {
		QString command;
		QVariant data;
		this->din >> command >> data;
		this->din.device()->seek( 0 );
		this->buffer.remove( 0, this->blockSize );
		this->blockSize = 0;
		this->queue.enqueue( qMakePair( command, data ) );
	}
	emit this->readyRead();
}
