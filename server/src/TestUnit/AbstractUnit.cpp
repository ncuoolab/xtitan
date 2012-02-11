#include "AbstractUnitPrivate.hpp"
#include "TestCase/AbstractPoint.hpp"
#include "Network/TCPMessage.hpp"
#include "xTitan/Utility/Setting.hpp"

#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QEventLoop>

#include <cassert>

using xtitan::utilities::Setting;
using namespace xtitan::testunit;
using xtitan::network::SimpleSocket;
using xtitan::testcase::CheckPoint;
using xtitan::testcase::InputPoint;
using xtitan::testcase::TestCase;

AbstractUnit::Private::Private( AbstractUnit * host ):
QObject( host ),
host( host ),
sockets(),
children_(),
testCase(),
commands(),
canceled( false ) {
//	this->commands.insert( std::make_pair( TCPMessage::ReqJoinUnit, std::tr1::bind( &AbstractUnit::Private::onJoin, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2 ) ) );
	this->commands.insert( std::make_pair( TCPMessage::Input, std::bind( &AbstractUnit::Private::onInput, this, std::placeholders::_1, std::placeholders::_2 ) ) );
	this->commands.insert( std::make_pair( TCPMessage::Check, std::bind( &AbstractUnit::Private::onCheck, this, std::placeholders::_1, std::placeholders::_2 ) ) );
}

void AbstractUnit::Private::addClient( SimpleSocket * socket ) {
	if( this->sockets.size() >= this->testCase.getClients() ) {
		return;
	}

// 	QTimer * timer = new QTimer( socket );
// 	timer->setSingleShot( true );
// 	timer->setInterval( 60000 );
// 	timer->connect( socket, SIGNAL( readyRead() ), SLOT( start() ) );
// 	socket->connect( timer, SIGNAL( timeout() ), SLOT( abort() ) );
	this->connect( socket, SIGNAL( readyRead() ), SLOT( onSocketReadyRead() ) );
	this->connect( socket, SIGNAL( error( QLocalSocket::LocalSocketError ) ), SLOT( onSocketError( QLocalSocket::LocalSocketError ) ) );
	this->connect( socket, SIGNAL( disconnected() ), SLOT( onSocketDisconnected() ) );

	this->sockets.append( socket );
	if( this->sockets.size() == this->testCase.getClients() ) {
		emit this->readyForTest();
	}
}

void AbstractUnit::Private::spawnChiwawa() {
	// spawn chiwawa
	QFileInfo msnShell( Setting::getInstance().get( "MsnShellProgram" ) );
	QDir testCase( Setting::getInstance().get( "TestCasePath" ) );

	for( int i = 0; i < this->testCase.getClients(); ++i ) {
		QStringList args;
		QString logPath( QString( "log/tmp/Chiwawa.%1" ).arg( i ) );
		testCase.mkpath( QString( "%1/log" ).arg( logPath ) );

		args.append( "-nf" );
		args.append( "-t" );
		// TODO: only one test unit at a time
		//		args.append( this->getName() );
		args.append( "-if" );
		args.append( Setting::getInstance().get( "ConfigurationPath" ) );
		args.append( "-of" );
		args.append( testCase.filePath( logPath ) );
		QProcess * shell = new QProcess;
		shell->setWorkingDirectory( msnShell.path() );
		shell->start( msnShell.filePath(), args );
		this->children_.append( shell );
	}
}

void AbstractUnit::Private::restartServer() {
	QFileInfo portal( Setting::getInstance().get( "PortalServer" ) );
	QProcess p;
	p.setWorkingDirectory( portal.path() );
	p.start( portal.filePath() );
	p.waitForFinished( -1 );
}

void AbstractUnit::Private::onSocketReadyRead() {
	SimpleSocket * socket = static_cast< SimpleSocket * >( this->sender() );
	assert( socket || !"socket is null" );
	while( !socket->atEnd() ) {
		SimpleSocket::Packet packet( socket->read() );
		CommandMap::const_iterator it = this->commands.find( packet.first );
		if( it == this->commands.end() ) {
			// TODO error
			continue;
		} else {
			packet = it->second( socket, packet.second );
			socket->write( packet.first, packet.second );
		}
	}
}

void AbstractUnit::Private::onSocketError( QLocalSocket::LocalSocketError /*socketError*/ ) {
	SimpleSocket * socket = static_cast< SimpleSocket * >( this->sender() );
	this->canceled = true;
	emit this->log( QObject::tr( "Connection" ), socket->errorString() );
}

void AbstractUnit::Private::onSocketDisconnected() {
	emit this->log( QObject::tr( "Connection" ), QObject::tr( "Disconnected" ) );
}

SimpleSocket::Packet AbstractUnit::Private::onCheck( SimpleSocket * sender, const QVariant & data ) {
	int id = this->sockets.indexOf( sender );
	assert( ( id >= 0 && id < this->sockets.size() ) || !"invalid socket" );
	QVariantMap args( data.toMap() );
	QString label( args["label"].toString() );
	QString value( args["value"].toString() );

	return this->host->onCheck( id, label, value );
}

SimpleSocket::Packet AbstractUnit::Private::onInput( SimpleSocket * sender, const QVariant & data ) {
	int id = this->sockets.indexOf( sender );
	assert( ( id >= 0 && id < this->sockets.size() ) || !"invalid socket" );
	QVariantMap args( data.toMap() );
	QString label( args["label"].toString() );
	QString script( args["script"].toString() );
	int waitTime( args["waitTime"].toInt() );

	return this->host->onInput( id, label, script, waitTime );
}

SimpleSocket::Packet AbstractUnit::Private::onJoin( SimpleSocket * /*sender*/, const QVariant & /*data*/ ) {
	return SimpleSocket::Packet();
}

AbstractUnit::AbstractUnit( QObject * parent ):
QObject( parent ),
p_( new AbstractUnit::Private( this ) ) {
	this->connect( this->p_, SIGNAL( error( const QString &, const QString & ) ), SIGNAL( error( const QString &, const QString & ) ) );
	this->connect( this->p_, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
}

AbstractUnit::~AbstractUnit() {
}

void AbstractUnit::addClient( SimpleSocket * socket ) {
	this->p_->addClient( socket );
}

const QString & AbstractUnit::getName() const {
	return this->p_->testCase.getName();
}

void AbstractUnit::setName( const QString & name ) {
	this->p_->testCase.setName( name );
}

bool AbstractUnit::isCancled() const {
	return this->p_->canceled;
}

void AbstractUnit::check( CheckPoint * point ) {
	this->doCheck( point );
}

void AbstractUnit::input( InputPoint * point ) {
	this->doInput( point );
}

void AbstractUnit::run() {
	emit this->log( QObject::tr( "Information" ), QObject::tr( "Setup started" ) );

	// open test case
	this->doOpen();
	// restart server to reload cache
	this->p_->restartServer();
	// spawn clients
	this->p_->spawnChiwawa();

	// wait for all clients connected, timeout in 10 seconds
	QTimer timer;
	timer.setSingleShot( true );
	timer.setInterval( 10000 );
	QEventLoop wait;
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	wait.connect( this->p_, SIGNAL( readyForTest() ), SLOT( quit() ) );
	timer.start();
	wait.exec();
	timer.stop();

	emit this->log( QObject::tr( "Information" ), QObject::tr( "Setup completed" ) );

	if( this->p_->sockets.size() == this->p_->testCase.getClients() ) {
		// means really ready for replay
		this->doTest();
	} else {
		emit this->error( QObject::tr( "Client" ), QObject::tr( "Chiwawa client not started" ) );
	}

	emit this->log( QObject::tr( "Information" ), QObject::tr( "Clean up started" ) );

	// send quit command
	// TODO check socket state, some of them may disconnected
	foreach( SimpleSocket * socket, this->p_->sockets ) {
		// NOTE sockets are on a diffirent thread
		QMetaObject::invokeMethod( socket, "write", Q_ARG( const QString &, TCPMessage::QuitApplicationTag ), Q_ARG( const QVariant &, QVariant() ) );
		QMetaObject::invokeMethod( socket, "disconnectFromServer" );
		socket->deleteLater();
	}
	// wait for process normal exit, or kill it
	foreach( QProcess * p, this->p_->children_ ) {
		if( !p->waitForFinished( 3000 ) ) {
			// delete this object after the process been killed
			p->connect( p, SIGNAL( finished( int, QProcess::ExitStatus ) ), SLOT( deleteLater() ) );
			p->kill();
		}
	}

	this->doClose();
	// restart server to reload cache
	this->p_->restartServer();

	emit this->log( QObject::tr( "Information" ), QObject::tr( "Clean up completed" ) );

	// TODO make a result report
	// HINT error messages

	emit this->finished();
}

TestCase & AbstractUnit::testCase() {
	return this->p_->testCase;
}

const TestCase & AbstractUnit::getTestCase() const {
	return this->p_->testCase;
}

void AbstractUnit::sendMessage( int id, const QString & command, const QVariant & data ) {
	SimpleSocket * socket = this->p_->sockets.at( id );
	// NOTE this hack is because socket is on diffirent thread
	QMetaObject::invokeMethod( socket, "write", Q_ARG( const QString &, command ), Q_ARG( const QVariant &, data ) );
}

void AbstractUnit::setCanceled( bool canceled ) {
	this->p_->canceled = canceled;
	if( canceled ) {
		emit this->canceled();
	}
}

int AbstractUnit::getClients() const {
	return this->p_->testCase.getClients();
}

void AbstractUnit::setClients( int nClients ) {
	this->p_->testCase.setClients( nClients );
}
