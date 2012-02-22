#include "TestUnitManagerPrivate.hpp"
#include "RecordUnit.hpp"
#include "UpdateUnit.hpp"
#include "ReplayUnit.hpp"
#include "Widget/MainWindow.hpp"
#include "xTitan/Utility/Setting.hpp"
#include "xTitan/Utility/Logger.hpp"
#include "TestCase/TestCase.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QThreadPool>
#include <QtGui/QApplication>

#include <iostream>

using namespace xtitan::testunit;
using xtitan::utilities::Setting;
using xtitan::network::SimpleServer;
using xtitan::testcase::TestCase;

namespace {
	QStringList readFile( const QString & filePath ) {
		QFile fin( filePath );
		fin.open( QIODevice::ReadOnly | QIODevice::Text );
		QTextStream sin( &fin );
		QStringList result;
		while( !sin.atEnd() ) {
			result.append( sin.readLine() );
		}
		fin.close();
		return result;
	}
}

TestUnitManager::Private::Private():
server( new SimpleServer( this ) ),
currentUnit( NULL ),
testing( false ),
oneShot( false ),
currentTestCount( -1 ),
testList(),
errorList() {
	this->connect( this->server, SIGNAL( newConnection() ), SLOT( onNewConnection() ) );
	if( !this->server->listen( "OracleServer" ) ) {
		this->server->close();
		return;
	}
}

//destructor
TestUnitManager::Private::~Private(){
	this->server->close();
}

void TestUnitManager::Private::onRecordFinished() {
	this->currentUnit->deleteLater();
	this->currentUnit = NULL;
	this->errorList.clear();
	emit this->finished();
}

void TestUnitManager::Private::onUpdateFinished() {
	emit this->log( this->currentUnit->getName(), QObject::tr( "Finished" ) );
	emit this->finished( this->currentUnit->getTestCase() );
	this->currentUnit->deleteLater();
	this->currentUnit = NULL;
	this->errorList.clear();
}

void TestUnitManager::Private::onNewConnection() {
	while( this->server->hasPendingConnections() ) {
		this->currentUnit->addClient( this->server->nextPendingConntion() );
	}
}

void TestUnitManager::Private::onUnitError( const QString & type, const QString & message ) {
	this->errorList.append( QString( "<%1> %2" ).arg( type ).arg( message ) );
	emit this->error( type, message );
}

void TestUnitManager::Private::setupReplay( bool /*killing*/ ) {
	this->currentTestCount = 0;
	this->testing = true;

	std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	std::cout << "<testsuite>" << std::endl;

	this->nextReplay();
}

void TestUnitManager::Private::nextReplay() {
	// if current test count to the end, or user cancel replaying, stop
	if( this->currentTestCount == this->testList.size() || !this->testing ) {
		// end log
		emit this->log( "OracleServer", QObject::tr( "Finished" ) );
		std::cout << "</testsuite>" << std::endl;

		// move log files
		QDir root( Setting::getInstance().get( "TestCasePath" ) );
		root.cd( "log" );
		root.rename( "tmp", QDateTime::currentDateTime().toString( "yyyy_MM_dd_hhmmss" ) );
		emit this->finished();

		if( this->oneShot ) {
			QMetaObject::invokeMethod( QCoreApplication::instance(), "quit" );
		}

		return;
	}

	std::cout << "<testcase classname=\"Chiwawa\" name=\"" << this->testList.at( this->currentTestCount ).toUtf8().constData() << "\">" << std::endl;

	this->currentUnit = new ReplayUnit( this );
	this->currentUnit->setAutoDelete( false );
	this->currentUnit->setName( this->testList.at( this->currentTestCount ) );
	this->connect( this->currentUnit, SIGNAL( error( const QString &, const QString & ) ), SLOT( onUnitError( const QString &, const QString & ) ) );
	this->connect( this->currentUnit, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
	this->connect( this->currentUnit, SIGNAL( finished() ), SLOT( onReplayFinished() ) );

	emit this->log( this->currentUnit->getName(), QObject::tr( "Begin" ) );

	QThreadPool::globalInstance()->start( this->currentUnit );
}

void TestUnitManager::Private::onReplayFinished() {
	emit this->log( this->currentUnit->getName(), QObject::tr( "End" ) );

	this->currentUnit->deleteLater();
	this->currentUnit = NULL;

	if( !this->errorList.isEmpty() ) {
		std::cout << "<failure type=\"Failure\"><![CDATA[" << this->errorList.join( "\n" ).toUtf8().constData() << "]]></failure>" << std::endl;
	}
	std::cout << "</testcase>" << std::endl;

	this->errorList.clear();
	++this->currentTestCount;

	this->nextReplay();
}

void TestUnitManager::Private::createLogDir() {
	QDir path( Setting::getInstance().get( "TestCasePath" ) );
	path.mkpath( "log/tmp" );
	path.cd( "log/tmp" );
	path.mkdir( "Pass" );
	path.mkdir( "Fail" );
	path.mkdir( "Warning" );
}

//constructor
TestUnitManager::TestUnitManager( QObject * parent ):
QObject( parent ),
p_( new Private ) {
	this->connect( this->p_.get(), SIGNAL( error( const QString &, const QString & ) ), SIGNAL( error( const QString &, const QString & ) ) );
	this->connect( this->p_.get(), SIGNAL( finished() ), SIGNAL( finished() ) );
	this->connect( this->p_.get(), SIGNAL( finished( const xtitan::testcase::TestCase & ) ), SIGNAL( finished( const xtitan::testcase::TestCase & ) ) );
	this->connect( this->p_.get(), SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
}

void TestUnitManager::startRecord( int nClients ) {
	this->p_->currentUnit = new RecordUnit( this );
	this->p_->currentUnit->setAutoDelete( false );
	this->p_->connect( this->p_->currentUnit, SIGNAL( error( const QString &, const QString & ) ), SLOT( onUnitError( const QString &, const QString & ) ) );
	this->connect( this->p_->currentUnit, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
	this->p_->connect( this->p_->currentUnit, SIGNAL( finished() ), SLOT( onRecordFinished() ) );
	this->p_->currentUnit->setClients( nClients );

	emit this->log( QObject::tr( "OracleServer" ), QObject::tr( "Start recoding" ) );

	QThreadPool::globalInstance()->start( this->p_->currentUnit );
}

TestCase TestUnitManager::stopRecord() {
	if( this->p_->currentUnit ) {
		TestCase tc( this->p_->currentUnit->getTestCase() );
		this->p_->currentUnit->setCanceled( true );
		// NOTE even the replay unit has been set to canceled
		// there is still some thing to clean up.
		// please lock the UI until finished emited.
		emit this->log( QObject::tr( "OracleServer" ), QObject::tr( "Stop recoding" ) );
		return tc;
	}
	return TestCase();
}

void TestUnitManager::startUpdate( const QString & name ) {
	this->p_->currentUnit = new UpdateUnit( this );
	this->p_->currentUnit->setAutoDelete( false );
	this->p_->currentUnit->setName( name );
	this->p_->connect( this->p_->currentUnit, SIGNAL( error( const QString &, const QString & ) ), SLOT( onUnitError( const QString &, const QString & ) ) );
	this->connect( this->p_->currentUnit, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
	this->p_->connect( this->p_->currentUnit, SIGNAL( finished() ), SLOT( onUpdateFinished() ) );

	emit this->log( name, QObject::tr( "Started" ) );

	QThreadPool::globalInstance()->start( this->p_->currentUnit );
}

bool TestUnitManager::isOneShot() const {
	return this->p_->oneShot;
}

void TestUnitManager::setOneShot( bool oneShot ) {
	this->p_->oneShot = oneShot;
}

void TestUnitManager::regressionTest() {
	QDir tcDir( Setting::getInstance().get( "TestCasePath" ) );
	this->p_->testList.clear();
	this->p_->testList << readFile( tcDir.filePath( "level0.txt" ) );
	this->p_->testList << readFile( tcDir.filePath( "level1.txt" ) );
	this->p_->testList << readFile( tcDir.filePath( "level2.txt" ) );

	this->p_->createLogDir();
	// start a client
	this->p_->setupReplay( true );
}

void TestUnitManager::smokeTest() {
	QDir tcDir( Setting::getInstance().get( "TestCasePath" ) );
	this->p_->testList.clear();
	this->p_->testList << readFile( tcDir.filePath( "level0.txt" ) );

	this->p_->createLogDir();
	//start a client
	this->p_->setupReplay( true );
}

void TestUnitManager::specificTest( const QStringList & tests, bool killing ) {
	this->p_->testList = tests;

	this->p_->createLogDir();
	this->p_->setupReplay( killing );
}

void TestUnitManager::stopTest() {
	if( this->p_->currentUnit ) {
		// for stop nextTest
		this->p_->testing = false;
		this->p_->currentUnit->setCanceled( true );
	}
}
