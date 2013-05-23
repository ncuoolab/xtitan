#include "TestUnitServer_p.hpp"

using xtitan::TestUnitServer;
using xtitan::SimpleServer;


TestUnitServer::Private::Private( TestUnitServer * owner ):
QObject(),
owner( owner ),
server( new SimpleServer( this ) ),
clients(),
recording( false ) {
	this->connect( this->server, SIGNAL( newConnection() ), SLOT( onNewConnection() ) );
}

void TestUnitServer::Private::onNewConnection() {
	while( this->server->hasPendingConnections() ) {
		auto socket = this->server->nextPendingConntion();
		auto testUnit = new TestUnit( this->clients.size(), socket, this->owner, this );
		this->connect( testUnit, SIGNAL( disconnected() ), SLOT( onClientDisconnected() ) );
		this->clients.push_back( testUnit );
	}
}

void TestUnitServer::Private::onClientDisconnected() {
	auto testUnit = qobject_cast< TestUnit * >( this->sender() );
	auto it = std::remove( this->clients.begin(), this->clients.end(), testUnit );
	if( it == this->clients.end() ) {
		// why not found?
		return;
	}
	std::for_each( it, this->clients.end(), []( TestUnit * tu )->void {
		tu->deleteLater();
	} );
	this->clients.erase( it, this->clients.end() );
}

TestUnitServer::TestUnitServer( QObject * parent ): QObject( parent ), p_( new Private( this ) ) {
}

bool TestUnitServer::check() const {
	return std::all_of( this->p_->clients.begin(), this->p_->clients.end(), []( TestUnit * tu )->bool {
		return tu->check();
	} );
}

QString TestUnitServer::errorString() const {
	return this->p_->server->errorString();
}

bool TestUnitServer::isRecording() const {
	return this->p_->recording;
}

bool TestUnitServer::listen( const QString & name ) {
	return this->p_->server->listen( name, true );
}

void TestUnitServer::recordOracle( int id, const CheckPoint & cp ) {
	auto testUnit = this->p_->clients.at( id );
	testUnit->recordOracle( cp );
}

void TestUnitServer::recordAsyncOracle( int id, const AsyncCheckPoint & acp ) {
	auto testUnit = this->p_->clients.at( id );
	testUnit->recordAsyncOracle( acp );
}


void TestUnitServer::sendInput( int id, const QString & object, const QString & method, const QVariantList & args ) {
	auto testUnit = this->p_->clients.at( id );
	testUnit->sendInput( object, method, args );
}

void TestUnitServer::startRecording() {
	this->p_->recording = true;
}

void TestUnitServer::stopRecording() {
	this->p_->recording = false;
}
