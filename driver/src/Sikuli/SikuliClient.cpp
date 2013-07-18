/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "SikuliClient_p.hpp"

#include <cassert>
#include <limits>

#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>
#include <QtGui/QCursor>

#include "xTitan/Exception/IOError.hpp"
#include "xTitan/Utility/Setting.hpp"

#include "TestUnit/CheckPoint.hpp"


namespace {

// make complete sikuli bundle path from settings
QString getBundlePath( const QString & name ) {
	QDir tcd( xtitan::Setting::instance().get( "test_cases_path" ).toString() );
	return tcd.filePath( name ) + ".sikuli";
}

}


using xtitan::SikuliClient;
using xtitan::IOError;


SikuliClient::Private::Private():
doFailure(),
doSuccess(),
opening( false ),
server( new QProcess( this ) ),
client( new QProcess( this ) ),
engine( new QScriptEngine( this ) ),
sio( this->server ),
clientPath(),
clientWD(),
bundles(),
currentBundle( std::numeric_limits< BundleList::size_type >::max() ) {
	this->connect( this->server, SIGNAL( readyReadStandardError() ), SLOT( onReadyReadStandardError() ) );
	this->connect( this->server, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
	this->connect( this->server, SIGNAL( finished( int, QProcess::ExitStatus ) ), SLOT( onFinished( int, QProcess::ExitStatus ) ) );

	this->sio.setCodec( QTextCodec::codecForName( "UTF-8" ) );
}

void SikuliClient::Private::onActionTimeout( const QString & message ) {
	if( this->client->state() == QProcess::Running ) {
		this->client->kill();
	}
	this->doSuccess = nullptr;
	this->doFailure = nullptr;
	emit this->error( message );
}

void SikuliClient::Private::onActivationTimeout() {
	this->opening = false;
	if( this->server->state() != QProcess::NotRunning ) {
		this->server->kill();
		this->server->waitForFinished();
	}
	emit this->error( QObject::tr( "cant not start SikuliServer." ) );
}

void SikuliClient::Private::readBundle() {
	this->client->kill();
	// TODO asynchronous if need
	this->client->waitForFinished();
	this->doSuccess = nullptr;
	this->doFailure = nullptr;

	QString line;
	while( !this->sio.atEnd() ) {
		line.append( this->sio.readLine() );
	}
	QVariantMap result( this->decode( line ) );

	emit this->taskCompleted( std::get< 0 >( this->bundles.at( this->currentBundle ) ), result.value( "success" ).toBool(), result.value( "message" ).toString() );

	this->nextBundle();
}

void SikuliClient::Private::readCapture() {
	this->doSuccess = nullptr;
	this->doFailure = nullptr;

	QString line;
	while( !this->sio.atEnd() ) {
		line.append( this->sio.readLine() );
	}
	QVariantMap result( this->decode( line ) );

	if( result.value( "success" ).toBool() ) {
		emit this->captured( result.value( "path" ).toString() );
	} else {
		emit this->error( result.value( "message" ).toString() );
	}
}

void SikuliClient::Private::readExecute() {
	while( this->server->canReadLine() ) {
		QString line = this->sio.readLine();
		QVariantMap data = this->decode( line );

		auto result = data.value( "result" ).toString();
		if( result == "succeed" ) {
			this->client->kill();
			this->doSuccess = nullptr;
			this->doFailure = nullptr;

			emit this->executed( true, "" );
			return;
		} else if( result == "failed" ) {
			this->client->kill();
			this->doSuccess = nullptr;
			this->doFailure = nullptr;

			emit this->executed( false, data.value( "message" ).toString() );
			return;
		} else if( result == "input" ) {
			auto id = data.value( "id" ).toInt();
			InputPoint tmp;
			tmp.object = data.value( "object" ).toString();
			tmp.method = data.value( "method" ).toString();
			tmp.args = data.value( "args" ).toList();
			emit this->inputRequired( id, tmp );
		} else if( result == "check" ) {
			auto id = data.value( "id" ).toInt();
			auto cp = data.value( "cp" ).toMap();
			CheckPoint tmp;
			tmp.file = cp.value( "file" ).toString();
			tmp.line = cp.value( "line" ).toInt();
			tmp.id = cp.value( "id" ).toString();
			tmp.args = cp.value( "args" ).toList();
			emit this->checkRequired( id, tmp );
		} else if( result == "async_check" ) {
			auto id = data.value( "id" ).toInt();
			auto acp = data.value( "acp" ).toMap();
			AsyncCheckPoint tmp;
			tmp.file = acp.value( "file" ).toString();
			tmp.line = acp.value( "line" ).toInt();
			tmp.id = acp.value( "id" ).toString();
			tmp.pre = acp.value( "pre" ).toString();
			tmp.args = acp.value( "args" ).toList();
			emit this->asyncCheckRequired( id, tmp );
		} else {
			assert( !"invalid value" );
		}
	}
}

void SikuliClient::Private::readReadyToken() {
	bool ready = false;
	while( !this->sio.atEnd() ) {
		QString line( this->sio.readLine() );
		if( line == "ready" ) {
			ready = true;
			break;
		}
	}
	if( ready ) {
		this->opening = false;
		this->doSuccess = nullptr;
		this->doFailure = nullptr;
		emit this->ready();
		return;
	}
}

void SikuliClient::Private::nextBundle() {
	if( this->server->state() != QProcess::Running ) {
		emit this->error( QObject::tr( "SikuliServer is not connected" ) );
		emit this->bundlesExecuted();
		return;
	}
	if( this->doSuccess || this->doFailure ) {
		// there is another action
		return;
	}

	++this->currentBundle;
	if( this->currentBundle >= this->bundles.size() ) {
		// TODO finished
		this->bundles.clear();
		this->currentBundle = std::numeric_limits< BundleList::size_type >::max();
		emit this->bundlesExecuted();
		return;
	}

	auto bundle( this->bundles.at( this->currentBundle ) );

	// move cursor out to prevent some hover event
	QCursor::setPos( 0, 0 );
	this->client->setWorkingDirectory( this->clientWD );
	this->client->start( QString( "%1 %2" ).arg( this->clientPath ).arg( std::get< 1 >( bundle ) ) );
	// TODO asynchronous if need
	this->client->waitForStarted();
	// wait window manager animation finished
	QEventLoop wait;
	QTimer::singleShot( Setting::instance().get( "test_case_interval" ).toInt(), &wait, SLOT( quit() ) );
	wait.exec();

	this->doSuccess = std::bind( &SikuliClient::Private::readBundle, this );
	this->doFailure = std::bind( &SikuliClient::Private::onActionTimeout, this, QObject::tr( "file command not responding" ) );

	QScriptValue args( this->engine->newObject() );
	args.setProperty( "command", "bundle" );
	args.setProperty( "path", getBundlePath( std::get< 0 >( bundle ) ) );

	this->sio << this->encode( args ) << endl;
}

QVariantMap SikuliClient::Private::decode( const QString & base64 ) {
	QString json( QString::fromUtf8( QByteArray::fromBase64( base64.toUtf8() ) ) );
	this->engine->globalObject().setProperty( "tmp", json );
	QScriptValue v( this->engine->evaluate( "JSON.parse( tmp )" ) );
	this->engine->globalObject().setProperty( "tmp", this->engine->nullValue() );
	return v.toVariant().toMap();
}

QString SikuliClient::Private::encode( const QScriptValue & object ) {
	this->engine->globalObject().setProperty( "tmp", object );
	QString json( this->engine->evaluate( "JSON.stringify( tmp );" ).toString() );
	this->engine->globalObject().setProperty( "tmp", this->engine->nullValue() );
	return QString::fromUtf8( json.toUtf8().toBase64() );
}

void SikuliClient::Private::onFinished( int exitCode, QProcess::ExitStatus exitStatus ) {
	if( exitStatus == QProcess::CrashExit ) {
		emit this->error( QObject::tr( "Sikuli Server crashed with code %1" ).arg( exitCode ) );
	}
}

void SikuliClient::Private::onReadyRead() {
	if( this->doSuccess ) {
		this->doSuccess();
	}
}

void SikuliClient::Private::onReadyReadStandardError() {
	QString msg( QString::fromUtf8( this->server->readAllStandardError() ) );
	emit this->log( msg );
}

void SikuliClient::Private::onTimeout() {
	if( this->doFailure ) {
		this->doFailure();
	}
}

SikuliClient::SikuliClient():
p_( new Private ) {
	this->connect( this->p_.get(), SIGNAL( bundlesExecuted() ), SIGNAL( bundlesExecuted() ) );
	this->connect( this->p_.get(), SIGNAL( captured( const QString & ) ), SIGNAL( captured( const QString & ) ) );
	this->connect( this->p_.get(), SIGNAL( error( const QString & ) ), SIGNAL( error( const QString & ) ) );
	this->connect( this->p_.get(), SIGNAL( executed( bool, const QString & ) ), SIGNAL( executed( bool, const QString & ) ) );
	this->connect( this->p_.get(), SIGNAL( checkRequired( int, const xtitan::CheckPoint & ) ), SIGNAL( checkRequired( int, const xtitan::CheckPoint & ) ) );
	this->connect( this->p_.get(), SIGNAL( asyncCheckRequired( int, const xtitan::AsyncCheckPoint & ) ), SIGNAL( asyncCheckRequired( int, const xtitan::AsyncCheckPoint & ) ) );
	this->connect( this->p_.get(), SIGNAL( inputRequired( int, const xtitan::InputPoint & ) ), SIGNAL( inputRequired( int, const xtitan::InputPoint & ) ) );
	this->connect( this->p_.get(), SIGNAL( log( const QString & ) ), SIGNAL( log( const QString & ) ) );
	this->connect( this->p_.get(), SIGNAL( ready() ), SIGNAL( ready() ) );
	this->connect( this->p_.get(), SIGNAL( taskCompleted( const QString &, bool, const QString & ) ), SIGNAL( taskCompleted( const QString &, bool, const QString & ) ) );
}

SikuliClient::~SikuliClient() {
	this->close();
}

void SikuliClient::close() {
	if( !this->isOpen() ) {
		return;
	}

	QScriptValue args( this->p_->engine->newObject() );
	args.setProperty( "command", "exit" );
	this->p_->sio << this->p_->encode( args ) << endl;

	if( !this->p_->server->waitForFinished() ) {
		// TODO error handling
		this->p_->server->kill();
	}
}

bool SikuliClient::isOpen() const {
	return this->p_->server->state() == QProcess::Running;
}

void SikuliClient::open() {
	if( this->p_->opening || this->isOpen() ) {
		return;
	}

	// TODO command line args support
	// TODO working directory support
	QString path = Setting::instance().get( "testing_program_path" ).toString();
	QFileInfo info( path );
	if( !info.exists() ) {
		throw IOError( QObject::tr( "The program to be testing is not found. (%1)" ).arg( path ) );
	}
	this->p_->clientPath = path;
	this->p_->clientWD = info.path();

	path = Setting::instance().get( "sikuliserver_path" ).toString();
	info = path;
	if( !info.exists() ) {
		throw IOError( QObject::tr( "The SikuliServer `%1\' is invalid." ).arg( path ) );
	}

	this->p_->doSuccess = std::bind( &SikuliClient::Private::readReadyToken, this->p_ );
	this->p_->doFailure = std::bind( &SikuliClient::Private::onActivationTimeout, this->p_ );

	QStringList args;
	args.append( "-jar" );
	args.append( path );
	args.append( "--quite" );
	this->p_->server->start( "java", args );
	this->p_->opening = true;
}

void SikuliClient::capture() {
	if( !this->isOpen() ) {
		throw IOError( QObject::tr( "SikuliServer is not connected" ) );
	}
	if( this->p_->doSuccess || this->p_->doFailure ) {
		// there is another action
		return;
	}

	this->p_->doSuccess = std::bind( &SikuliClient::Private::readCapture, this->p_ );
	this->p_->doFailure = std::bind( &SikuliClient::Private::onActionTimeout, this->p_, QObject::tr( "capture command not responding" ) );

	QScriptValue args( this->p_->engine->newObject() );
	args.setProperty( "command", "capture" );

	this->p_->sio << this->p_->encode( args ) << endl;
}

void SikuliClient::execute( const QString & script, const QString & programOptions ) {
	if( !this->isOpen() ) {
		throw IOError( QObject::tr( "SikuliServer is not connected" ) );
	}
	if( this->p_->doSuccess || this->p_->doFailure ) {
		// there is another action
		return;
	}

	// TODO spawn by python?
	this->p_->client->setWorkingDirectory( this->p_->clientWD );
	this->p_->client->start( QString( "%1 %2" ).arg( this->p_->clientPath ).arg( programOptions ) );
	// TODO asynchronous if need
	this->p_->client->waitForStarted();

	this->p_->doSuccess = std::bind( &SikuliClient::Private::readExecute, this->p_ );
	this->p_->doFailure = std::bind( &SikuliClient::Private::onActionTimeout, this->p_, QObject::tr( "execute command not responding" ) );

	QScriptValue args( this->p_->engine->newObject() );
	args.setProperty( "command", "execute" );
	args.setProperty( "script", script );

	this->p_->sio << this->p_->encode( args ) << endl;
}

void SikuliClient::executeBundles( const std::vector< std::tuple< QString, QString > > & bundles ) {
	this->p_->bundles = bundles;
	this->p_->currentBundle = std::numeric_limits< Private::BundleList::size_type >::max();
	this->p_->nextBundle();
}
