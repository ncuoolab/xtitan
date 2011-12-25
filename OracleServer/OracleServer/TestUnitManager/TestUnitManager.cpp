#include "TestUnitManager/TestUnitManager.h"
#include "OracleServerView.h"
#include "TestUnit/QxTestUnit.h"
#include "Setting/Setting.h"
#include <QtNetwork>
#include <QMessageBox>
#include "OutputLogger/QxLogger.h"
#include "TestUnit/QxReplayUnit.h"
#include "TcfDecoder/QxTestCase.h"
#include "TestUnit/QxRecordUnit.h"
#include "TestUnit/QxUpdateUnit.h"

#include <QtGui/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QEventLoop>
#include <QtCore/QThreadPool>

#include <iostream>

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

//constructor
TestUnitManager::TestUnitManager( QObject * parent ):
QObject( parent ),
server_( new QxSimpleServer( this ) ),
currentUnit_( NULL ),
testing_( false ),
oneShot_( false ),
currentTestCount_( -1 ),
testList_(),
errorList_() {
	this->connect( this->server_, SIGNAL( newConnection() ), this, SLOT( onNewConnection_() ) );
//	QObject::connect( this->server_, SIGNAL( newConnection() ), this, SLOT( onNewConnection_() ) );
	if( !this->server_->listen( "OracleServer" ) ) {
//		QMessageBox::warning( view, "Oracle Server", "Unable to start the server: " + this->server_->errorString() );
		this->server_->close();
		return;
	}
}

//destructor
TestUnitManager::~TestUnitManager(){
	this->server_->close();
}

void TestUnitManager::startRecord( int nClients ) {
	this->currentUnit_ = new QxRecordUnit( this );
	this->currentUnit_->setAutoDelete( false );
	this->connect( this->currentUnit_, SIGNAL( error( const QString &, const QString & ) ), SLOT( onUnitError_( const QString &, const QString & ) ) );
	this->connect( this->currentUnit_, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
	this->connect( this->currentUnit_, SIGNAL( finished() ), SLOT( onRecordFinished_() ) );
	this->currentUnit_->setClients( nClients );

	emit this->log( QObject::tr( "OracleServer" ), QObject::tr( "Start recoding" ) );

	QThreadPool::globalInstance()->start( this->currentUnit_ );
}

QxTestCase TestUnitManager::stopRecord() {
	if( this->currentUnit_ ) {
		QxTestCase tc( this->currentUnit_->getTestCase() );
		this->currentUnit_->setCanceled( true );
		// NOTE even the replay unit has been set to canceled
		// there is still some thing to clean up.
		// please lock the UI until finished emited.
		emit this->log( QObject::tr( "OracleServer" ), QObject::tr( "Stop recoding" ) );
		return tc;
	}
	return QxTestCase();
}

void TestUnitManager::onRecordFinished_() {
	this->currentUnit_->deleteLater();
	this->currentUnit_ = NULL;
	this->errorList_.clear();
	emit this->finished();
}

void TestUnitManager::startUpdate( const QString & name ) {
	this->currentUnit_ = new QxUpdateUnit( this );
	this->currentUnit_->setAutoDelete( false );
	this->currentUnit_->setName( name );
	this->connect( this->currentUnit_, SIGNAL( error( const QString &, const QString & ) ), SLOT( onUnitError_( const QString &, const QString & ) ) );
	this->connect( this->currentUnit_, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
	this->connect( this->currentUnit_, SIGNAL( finished() ), SLOT( onUpdateFinished_() ) );

	emit this->log( name, QObject::tr( "Started" ) );

	QThreadPool::globalInstance()->start( this->currentUnit_ );
}

void TestUnitManager::onUpdateFinished_() {
	emit this->log( this->currentUnit_->getName(), QObject::tr( "Finished" ) );
	emit this->finished( this->currentUnit_->getTestCase() );
	this->currentUnit_->deleteLater();
	this->currentUnit_ = NULL;
	this->errorList_.clear();
}

bool TestUnitManager::isOneShot() const {
	return this->oneShot_;
}

void TestUnitManager::setOneShot( bool oneShot ) {
	this->oneShot_ = oneShot;
}

//slots 

void TestUnitManager::onNewConnection_() {
	while( this->server_->hasPendingConnections() ) {
		this->currentUnit_->addClient( this->server_->nextPendingConntion() );
	}
}

void TestUnitManager::onUnitError_( const QString & type, const QString & message ) {
	this->errorList_.append( QString( "<%1> %2" ).arg( type ).arg( message ) );
	emit this->error( type, message );
}

void TestUnitManager::RegressionTest() {
	QDir tcDir( QxSetting::Instance().get( "TestCasePath" ) );
	this->testList_.clear();
	this->testList_ << readFile( tcDir.filePath( "level0.txt" ) );
	this->testList_ << readFile( tcDir.filePath( "level1.txt" ) );
	this->testList_ << readFile( tcDir.filePath( "level2.txt" ) );

	this->createLogDir_();
	//start a client
	this->setupReplay_( true );
}

void TestUnitManager::SmokeTest() {
	QDir tcDir( QxSetting::Instance().get( "TestCasePath" ) );
	this->testList_.clear();
	this->testList_ << readFile( tcDir.filePath( "level0.txt" ) );

	this->createLogDir_();
	//start a client
	this->setupReplay_( true );
}

void TestUnitManager::specificTest( const QStringList & tests, bool killing ) {
	this->testList_ = tests;

	this->createLogDir_();
	this->setupReplay_( killing );
}

void TestUnitManager::setupReplay_( bool /*killing*/ ) {
	this->currentTestCount_ = 0;
	this->testing_ = true;

	std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	std::cout << "<testsuite>" << std::endl;

	this->nextReplay_();
}

void TestUnitManager::nextReplay_() {
	// if current test count to the end, or user cancel replaying, stop
	if( this->currentTestCount_ == this->testList_.size() || !this->testing_ ) {
		// end log
		emit this->log( "OracleServer", QObject::tr( "Finished" ) );
		std::cout << "</testsuite>" << std::endl;

		// move log files
		QDir root( QxSetting::Instance().get( "TestCasePath" ) );
		root.cd( "log" );
		root.rename( "tmp", QDateTime::currentDateTime().toString( "yyyy_MM_dd_hhmmss" ) );
		emit this->finished();

		if( this->oneShot_ ) {
			QMetaObject::invokeMethod( QCoreApplication::instance(), "quit" );
		}

		return;
	}

	std::cout << "<testcase classname=\"Chiwawa\" name=\"" << this->testList_.at( this->currentTestCount_ ).toUtf8().constData() << "\">" << std::endl;

	this->currentUnit_ = new QxReplayUnit( this );
	this->currentUnit_->setAutoDelete( false );
	this->currentUnit_->setName( this->testList_.at( this->currentTestCount_ ) );
	this->connect( this->currentUnit_, SIGNAL( error( const QString &, const QString & ) ), SLOT( onUnitError_( const QString &, const QString & ) ) );
	this->connect( this->currentUnit_, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
	this->connect( this->currentUnit_, SIGNAL( finished() ), SLOT( onReplayFinished_() ) );

	emit this->log( this->currentUnit_->getName(), QObject::tr( "Begin" ) );

	QThreadPool::globalInstance()->start( this->currentUnit_ );
}

void TestUnitManager::onReplayFinished_() {
	emit this->log( this->currentUnit_->getName(), QObject::tr( "End" ) );

	this->currentUnit_->deleteLater();
	this->currentUnit_ = NULL;

	if( !this->errorList_.isEmpty() ) {
		std::cout << "<failure type=\"Failure\"><![CDATA[" << this->errorList_.join( "\n" ).toUtf8().constData() << "]]></failure>" << std::endl;
	}
	std::cout << "</testcase>" << std::endl;

	this->errorList_.clear();
	++this->currentTestCount_;

	this->nextReplay_();
}

void TestUnitManager::stopTest() {
	if( this->currentUnit_ ) {
		// for stop nextTest
		this->testing_ = false;
		this->currentUnit_->setCanceled( true );
	}
}

void TestUnitManager::createLogDir_() {
	QDir path( QxSetting::Instance().get( "TestCasePath" ) );
	path.mkpath( "log/tmp" );
	path.cd( "log/tmp" );
	path.mkdir( "Pass" );
	path.mkdir( "Fail" );
	path.mkdir( "Warning" );
}
