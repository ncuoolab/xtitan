#include "TestUnit_p.hpp"

#include <cassert>

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
sutAsyncCheckPoints(),
oracleCheckPoints(),
oracleAsyncCheckPoints() {
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

		//auto label = kwargs.value( "label" ).toString();
		auto feature = kwargs.value( "feature" ).toString();
		auto value = kwargs.value( "value" ).toString();

		if( this->server->isRecording() ) {
			emit this->checkReceived( this->id, feature, value );
		} else {
			// TODO thread lock?
			if( feature == "spyCheck" ) {
				this->sutCheckPoints.push_back( value );
			} else if( feature == "spyAsyncCheck" ) {
				this->sutAsyncCheckPoints.push_back( value );
			} else {
				assert( !"invalid check feature" );
			}
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
	bool syncPassed = this->p_->oracleCheckPoints == this->p_->sutCheckPoints;
	for( auto it = this->p_->sutAsyncCheckPoints.begin(); it != this->p_->sutAsyncCheckPoints.end(); ++it ) {
		auto it2 = std::find( this->p_->oracleAsyncCheckPoints.begin(), this->p_->oracleAsyncCheckPoints.end(), *it );
		if( it2 == this->p_->sutAsyncCheckPoints.end() ) {
			// not found
			return false;
		}
		this->p_->oracleAsyncCheckPoints.erase( it2 );
	}
	bool asyncPassed = this->p_->oracleAsyncCheckPoints.empty();
	return syncPassed && asyncPassed;
}

void TestUnit::recordOracle( const QString & value ) {
	this->p_->oracleCheckPoints.push_back( value );
}

void TestUnit::recordAsyncOracle( const QString & value ) {
	this->p_->oracleAsyncCheckPoints.push_back( value );
}

void TestUnit::sendInput( const QString & object, const QString & method, const QVariantList & args ) {
	QVariantMap kwargs;
	kwargs.insert( "object", object );
	kwargs.insert( "method", method );
	kwargs.insert( "args", args );
	this->p_->socket->write( "<Input>", kwargs );
}
