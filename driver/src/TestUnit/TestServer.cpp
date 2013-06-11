#include "TestServer_p.hpp"


using xtitan::AsyncCheckPoint;
using xtitan::CheckPoint;
using xtitan::InputPoint;
using xtitan::SimpleServer;
using xtitan::TestServer;


TestServer::Private::Private( TestServer * owner ):
QObject(),
owner( owner ),
server( new SimpleServer( this ) ),
clients(),
recording( false ) {
	this->connect( this->server, SIGNAL( newConnection() ), SLOT( onNewConnection() ) );
}

void TestServer::Private::onNewConnection() {
	while( this->server->hasPendingConnections() ) {
		auto socket = this->server->nextPendingConntion();
		auto testUnit = new TestClient( this->clients.size(), socket, this->owner, this );
		this->connect( testUnit, SIGNAL( disconnected() ), SLOT( onClientDisconnected() ) );
		this->clients.push_back( testUnit );
	}
}

void TestServer::Private::onClientDisconnected() {
	auto testUnit = qobject_cast< TestClient * >( this->sender() );
	auto it = std::remove( this->clients.begin(), this->clients.end(), testUnit );
	if( it == this->clients.end() ) {
		// why not found?
		return;
	}
	std::for_each( it, this->clients.end(), []( TestClient * tu )->void {
		tu->deleteLater();
	} );
	this->clients.erase( it, this->clients.end() );
}

TestServer::TestServer( QObject * parent ): QObject( parent ), p_( new Private( this ) ) {
}

bool TestServer::check() const {
	return std::all_of( this->p_->clients.begin(), this->p_->clients.end(), []( TestClient * tu )->bool {
		return tu->check();
	} );
}

QString TestServer::errorString() const {
	return this->p_->server->errorString();
}

bool TestServer::isRecording() const {
	return this->p_->recording;
}

bool TestServer::listen( const QString & name ) {
	return this->p_->server->listen( name, true );
}

void TestServer::recordOracle( int id, const CheckPoint & cp ) {
	// TODO catch exception
	auto testUnit = this->p_->clients.at( id );
	testUnit->recordOracle( cp );
}

void TestServer::recordAsyncOracle( int id, const AsyncCheckPoint & acp ) {
	// TODO catch exception
	auto testUnit = this->p_->clients.at( id );
	testUnit->recordAsyncOracle( acp );
}


void TestServer::sendInput( int id, const InputPoint & ip ) {
	// TODO catch exception
	auto testUnit = this->p_->clients.at( id );
	testUnit->sendInput( ip );
}

void TestServer::startRecording() {
	this->p_->recording = true;
}

void TestServer::stopRecording() {
	this->p_->recording = false;
}
