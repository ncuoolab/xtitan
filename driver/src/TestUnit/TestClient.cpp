#include "TestClient_p.hpp"

#include <cassert>

#include <set>

#include <QtCore/QStringList>


using xtitan::AsyncCheckPoint;
using xtitan::CheckPoint;
using xtitan::InputPoint;
using xtitan::SimpleSocket;
using xtitan::TestClient;
using xtitan::TestServer;


TestClient::Private::Private( int id, SimpleSocket * socket, TestServer * server ):
QObject(),
id( id ),
socket( socket ),
server( server ),
commands(),
lastTimestamp( -1LL ),
sutCPs(),
sutACPs(),
oracleCPs(),
oracleACPs() {
	// input handler
	this->commands.insert( std::make_pair( "<Input>", [this]( const QVariant & data )->void {
		if( !this->server->isRecording() ) {
			return;
		}

		auto kwargs = data.toMap();

		InputPoint ip;
		ip.object = kwargs.value( "object" ).toString();
		ip.method = kwargs.value( "method" ).toString();
		ip.args = kwargs.value( "args" ).toList();
		ip.timestamp = kwargs.value( "timestamp" ).toLongLong();

		int delay = ( this->lastTimestamp < 0LL ) ? 0 : ip.timestamp - this->lastTimestamp;
		this->lastTimestamp = ip.timestamp;
		if( delay < 0LL ) {
			// prevent event flush
			return;
		}

		emit this->inputReceived( this->id, delay, ip );
	} ) );
	// check handler
	this->commands.insert( std::make_pair( "<Check>", [this]( const QVariant & data )->void {
		auto kwargs = data.toMap();

		CheckPoint cp;
		cp.file = kwargs.value( "file" ).toString();
		cp.line = kwargs.value( "line" ).toInt();
		cp.id = kwargs.value( "id" ).toString();
		cp.args = kwargs.value( "args" ).toList();

		if( this->server->isRecording() ) {
			emit this->checkReceived( this->id, cp );
		} else {
			// TODO thread lock?
			this->sutCPs.push_back( cp );
		}
	} ) );
	// async check handler
	this->commands.insert( std::make_pair( "<AsyncCheck>", [this]( const QVariant & data )->void {
		auto kwargs = data.toMap();

		AsyncCheckPoint acp;
		acp.file = kwargs.value( "file" ).toString();
		acp.line = kwargs.value( "line" ).toInt();
		acp.id = kwargs.value( "id" ).toString();
		acp.pre = kwargs.value( "pre" ).toString();
		acp.args = kwargs.value( "args" ).toList();

		if( this->server->isRecording() ) {
			emit this->asyncCheckReceived( this->id, acp );
		} else {
			// TODO thread lock?
			this->sutACPs.push_back( acp );
		}
	} ) );

	this->connect( socket, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
	server->connect( this, SIGNAL( asyncCheckReceived( int, const xtitan::AsyncCheckPoint & ) ), SIGNAL( asyncCheckReceived( int, const xtitan::AsyncCheckPoint & ) ) );
	server->connect( this, SIGNAL( checkReceived( int, const xtitan::CheckPoint & ) ), SIGNAL( checkReceived( int, const xtitan::CheckPoint & ) ) );
	server->connect( this, SIGNAL( inputReceived( int, int, const xtitan::InputPoint & ) ), SIGNAL( inputReceived( int, int, const xtitan::InputPoint & ) ) );
}

TestClient::Private::~Private() {
	if( this->socket ) {
		if( this->socket->isValid() ) {
			this->socket->disconnectFromServer();
		}
		this->socket->deleteLater();
	}
}

void TestClient::Private::onReadyRead() {
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

TestClient::TestClient( int id, SimpleSocket * socket, TestServer * server, QObject * parent ):
QObject( parent ),
p_( new Private( id, socket, server ) ) {
	this->connect( socket, SIGNAL( disconnected() ), SIGNAL( disconnected() ) );
}

bool TestClient::check() const {
	// synchronized check points must exactly same
	bool syncPassed = this->p_->oracleCPs == this->p_->sutCPs;
	// asynchronized check points
	// they could be unordered
	// but pre-cond must exists
	std::set< QString > ids;
	for( auto it = this->p_->sutACPs.begin(); it != this->p_->sutACPs.end(); ++it ) {
		auto it2 = std::find( this->p_->oracleACPs.begin(), this->p_->oracleACPs.end(), *it );
		if( it2 == this->p_->sutACPs.end() ) {
			// not found
			return false;
		}
		if( !it2->id.isEmpty() ) {
			ids.insert( it2->id );
		}
		if( !it2->pre.isEmpty() ) {
			auto it3 = ids.find( it2->pre );
			if( it3 == ids.end() ) {
				// pre-cond not found
				return false;
			}
		}
		this->p_->oracleACPs.erase( it2 );
	}
	bool asyncPassed = this->p_->oracleACPs.empty();
	return syncPassed && asyncPassed;
}

void TestClient::recordOracle( const CheckPoint & cp ) {
	this->p_->oracleCPs.push_back( cp );
}

void TestClient::recordAsyncOracle( const AsyncCheckPoint & acp ) {
	this->p_->oracleACPs.push_back( acp );
}

void TestClient::sendInput( const InputPoint & ip ) {
	QVariantMap kwargs;
	kwargs.insert( "object", ip.object );
	kwargs.insert( "method", ip.method );
	kwargs.insert( "args", ip.args );
	this->p_->socket->write( "<Input>", kwargs );
}
