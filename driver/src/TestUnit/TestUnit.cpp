#include "TestUnit_p.hpp"

#include <QtCore/QStringList>


using xtitan::TestUnit;
using xtitan::TestUnitServer;
using xtitan::SimpleSocket;


TestUnit::Private::Private( int id, SimpleSocket * socket, TestUnitServer * server ):
QObject(),
id( id ),
socket( socket ),
server( server ),
commands(),
lastTimestamp( -1LL ),
sutCheckPoints(),
oracleCheckPoints() {
	// input handler
	this->commands.insert( std::make_pair( "<Input>", [this]( const QVariant & data )->void {
		if( !this->server->isRecording() ) {
			return;
		}

		auto kwargs = data.toMap();

		auto object = kwargs.value( "object" ).toString();
		auto method = kwargs.value( "method" ).toString();
		auto args = kwargs.value( "args" ).toStringList();
		auto timestamp = kwargs.value( "timestamp" ).toLongLong();
		int delay = ( this->lastTimestamp < 0LL ) ? 0 : timestamp - this->lastTimestamp;
		this->lastTimestamp = timestamp;
		if( delay < 0LL ) {
			// prevent event flush
			return;
		}

		emit this->inputReceived( this->id, delay, object, method, args );
	} ) );
	// check handler
	this->commands.insert( std::make_pair( "<Check>", [this]( const QVariant & data )->void {
		auto kwargs = data.toMap();

		auto label = kwargs.value( "label" ).toString();
		auto value = kwargs.value( "value" ).toString();

		if( this->server->isRecording() ) {
			emit this->checkReceived( this->id, label, value );
		} else {
			// TODO thread lock?
			this->sutCheckPoints.push_back( value );
		}
	} ) );

	this->connect( socket, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
	server->connect( this, SIGNAL( checkReceived( int, const QString &, const QString & ) ), SIGNAL( checkReceived( int, const QString &, const QString & ) ) );
	server->connect( this, SIGNAL( inputReceived( int, int, const QString &, const QString &, const QStringList & ) ), SIGNAL( inputReceived( int, int, const QString &, const QString &, const QStringList & ) ) );
}

TestUnit::Private::~Private() {
	if( this->socket ) {
		if( this->socket->isValid() ) {
			this->socket->disconnectFromServer();
		}
		this->socket->deleteLater();
	}
}

void TestUnit::Private::onReadyRead() {
	while( !this->socket->atEnd() ) {
		auto packet = this->socket->read();
		auto it = this->commands.find( packet.first );
		if( it == this->commands.end() ) {
			// TODO report error
			continue;
		}
		it->second( packet.second );
	}
}

TestUnit::TestUnit( int id, SimpleSocket * socket, TestUnitServer * server, QObject * parent ):
QObject( parent ),
p_( new Private( id, socket, server ) ) {
	this->connect( socket, SIGNAL( disconnected() ), SIGNAL( disconnected() ) );
}

bool TestUnit::check() const {
	return this->p_->oracleCheckPoints == this->p_->sutCheckPoints;
}

void TestUnit::recordOracle( const QString & label, const QString & value ) {
	this->p_->oracleCheckPoints.push_back( value );
}

void TestUnit::sendInput( const QString & object, const QString & method, const QVariantList & args ) {
	QVariantMap kwargs;
	kwargs.insert( "object", object );
	kwargs.insert( "method", method );
	kwargs.insert( "args", args );
	this->p_->socket->write( "<Input>", kwargs );
}
