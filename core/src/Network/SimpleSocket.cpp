#include "SimpleSocketPrivate.hpp"

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>
#include <cassert>

using namespace xtitan::network;

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

void SimpleSocket::abort() {
	this->p_->socket->abort();
}

bool SimpleSocket::atEnd() const {
	return this->p_->queue.isEmpty();
}

void SimpleSocket::close() {
	this->p_->socket->close();
}

void SimpleSocket::connectToServer( const QString & name, QIODevice::OpenMode openMode /* = QIODevice::ReadWrite */ ) {
	this->p_->socket->connectToServer( name, openMode );
}

void SimpleSocket::disconnectFromServer() {
	this->p_->socket->disconnectFromServer();
}

QLocalSocket::LocalSocketError SimpleSocket::error() const {
	return this->p_->socket->error();
}

QString SimpleSocket::errorString() const {
	return this->p_->socket->errorString();
}

bool SimpleSocket::flush() {
	return this->p_->socket->flush();
}

QString SimpleSocket::fullServerName() const {
	return this->p_->socket->fullServerName();
}

bool SimpleSocket::isValid() const {
	return this->p_->socket->isValid();
}

QString SimpleSocket::serverName() const {
	return this->p_->socket->serverName();
}

QLocalSocket::LocalSocketState SimpleSocket::state() const {
	return this->p_->socket->state();
}

SimpleSocket::Packet SimpleSocket::read() {
	assert( !this->p_->queue.isEmpty() || !"QUEUE IS EMPTY" );
	return this->p_->queue.dequeue();
}

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
