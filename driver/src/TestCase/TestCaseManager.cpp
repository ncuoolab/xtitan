#include "TestCaseManager_p.hpp"

#include <algorithm>

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "xTitan/Utility/Setting.hpp"
#include "xTitan/Exception/InternalError.hpp"
#include "Exception/SqlError.hpp"
#include "TestCase/TestCase.hpp"


namespace {
	const char * const CONNECTION_NAME = "default";
}


using xtitan::TestCase;
using xtitan::TestCaseManager;
using xtitan::Setting;
using xtitan::IOError;
using xtitan::SqlError;
using xtitan::InternalError;

std::shared_ptr< TestCaseManager > TestCaseManager::Private::self;

void TestCaseManager::Private::destroy( TestCaseManager * self ) {
	delete self;
}

TestCaseManager::Private::Private():
QObject() {
}

/// This method fixes database schema from old to new
void TestCaseManager::Private::fixup() {
	QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
	QSqlQuery query( database );

	// new column since 0.1.6
	bool prepared = query.prepare( "SELECT program_options FROM testcases" );
	if( prepared ) {
		return;
	}
	prepared = query.prepare( "ALTER TABLE testcases ADD program_options TEXT" );
	if( !prepared ) {
		throw SqlError( query.lastError().text() );
	}
	if( !database.transaction() ) {
		throw SqlError( query.lastError().text() );
	}
	if( !query.exec() ) {
		throw SqlError( query.lastError().text() );
	}
	if( !database.commit() ) {
		throw SqlError( query.lastError().text() );
	}
}

void TestCaseManager::Private::refresh() {
	self->setQuery( "SELECT name,level,active,creator,description,mtime,program_options FROM testcases ORDER BY name", QSqlDatabase::database( CONNECTION_NAME, false ) );
}


void TestCaseManager::Private::close() {
	{
		// if any QSqlDatabase instance exists, it can not be really removable
		QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
		if( !database.isOpen() ) {
			return;
		}
		database.close();
	}
	QSqlDatabase::removeDatabase( CONNECTION_NAME );
}

void TestCaseManager::initialize() {
	if( Private::self ) {
		return;
	}
	Private::self.reset( new TestCaseManager, Private::destroy );
}

TestCaseManager & TestCaseManager::instance() {
	if( !Private::self ) {
		throw InternalError( "TestCaseManager has not initialized yet" );
	}
	return *Private::self;
}

TestCaseManager::TestCaseManager():
QSqlQueryModel(),
p_( new Private ) {
	this->p_->connect( QCoreApplication::instance(), SIGNAL( aboutToQuit() ), SLOT( close() ) );
}

TestCaseManager::~TestCaseManager() {
}

void TestCaseManager::close() {
	this->p_->close();
}

void TestCaseManager::create() {
	if( this->isOpen() ) {
		this->close();
	}

	QDir tcDir( Setting::instance().get( "test_cases_path" ).toString() );
	if( !tcDir.exists() ) {
		throw IOError( QObject::tr( "%1 does not exists" ).arg( tcDir.absolutePath() ) );
	}

	QFile dbFile( tcDir.filePath( "database.sqlite" ) );
	if( dbFile.exists() ) {
		dbFile.remove();
	}

	QSqlDatabase database( QSqlDatabase::addDatabase( "QSQLITE", CONNECTION_NAME ) );
	database.setDatabaseName( dbFile.fileName() );
	if( !database.open() ) {
		QSqlDatabase::removeDatabase( CONNECTION_NAME );
		throw SqlError( database.lastError().text() );
	}

	QSqlQuery query( database );
	bool prepared = query.prepare( "CREATE TABLE testcases (id INTEGER NOT NULL PRIMARY KEY, name TEXT NOT NULL, level INTEGER NOT NULL, active BOOLEAN NOT NULL, creator TEXT NOT NULL, description TEXT, mtime DATETIME NOT NULL, program_options TEXT);" );
	if( !prepared ) {
		database.close();
		QSqlDatabase::removeDatabase( CONNECTION_NAME );
		throw SqlError( query.lastError().text() );
	}
	if( !query.exec() ) {
		database.close();
		QSqlDatabase::removeDatabase( CONNECTION_NAME );
		throw SqlError( query.lastError().text() );
	}
	database.close();
	QSqlDatabase::removeDatabase( CONNECTION_NAME );
}

bool TestCaseManager::isOpen() const {
	QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
	return database.isOpen();
}

void TestCaseManager::open() {
	if( this->isOpen() ) {
		this->close();
	}

	QDir tcDir( Setting::instance().get( "test_cases_path" ).toString() );
	QString dbPath( tcDir.filePath( "database.sqlite" ) );
	if( !QFile::exists( dbPath ) ) {
		throw IOError( QObject::tr( "%1 does not exists" ).arg( dbPath ) );
	}

	QSqlDatabase database( QSqlDatabase::addDatabase( "QSQLITE", CONNECTION_NAME ) );
	database.setDatabaseName( dbPath );
	if( !database.open() ) {
		QSqlDatabase::removeDatabase( CONNECTION_NAME );
		throw SqlError( database.lastError().text() );
	}

	this->p_->fixup();

	this->p_->refresh();
	this->setHeaderData( 0, Qt::Horizontal, QObject::tr( "Name" ) );
	this->setHeaderData( 1, Qt::Horizontal, QObject::tr( "Level" ) );
	this->setHeaderData( 2, Qt::Horizontal, QObject::tr( "Active" ) );
	this->setHeaderData( 3, Qt::Horizontal, QObject::tr( "Creator" ) );
	this->setHeaderData( 4, Qt::Horizontal, QObject::tr( "Description" ) );
	this->setHeaderData( 5, Qt::Horizontal, QObject::tr( "Last Modified" ) );
	this->setHeaderData( 6, Qt::Horizontal, QObject::tr( "Program Options" ) );
}

void TestCaseManager::deleteByIndex( const QModelIndex & index ) {
	if( !index.isValid() ) {
		return;
	}

	QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
	if( !database.transaction() ) {
		throw SqlError( database.lastError().text() );
	}
	QSqlQuery query( database );
	bool prepared = query.prepare( "DELETE FROM testcases WHERE name=:name" );
	if( !prepared ) {
		throw SqlError( query.lastError().text() );
	}
	QString name( this->getName( index ) );
	query.bindValue( ":name", name );
	if( !query.exec() ) {
		throw SqlError( query.lastError().text() );
	}
	if( !database.commit() ) {
		throw SqlError( query.lastError().text() );
	}

	this->p_->refresh();
}

std::vector< std::tuple< QString, QString > > TestCaseManager::getActivated() const {
	QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
	QSqlQuery query( database );
	bool prepared = query.prepare( "SELECT name,program_options FROM testcases WHERE active=1" );
	if( !prepared || !query.exec() ) {
		throw SqlError( query.lastError().text() );
	}
	std::vector< std::tuple< QString, QString > > result;
	while( query.next() ) {
		result.push_back( std::make_tuple( query.value( 0 ).toString(), query.value( 1 ).toString() ) );
	}
	return move( result );
}

QString TestCaseManager::getName( const QModelIndex & index ) const {
	QModelIndex nameIndex( this->index( index.row(), 0 ) );
	return this->data( nameIndex ).toString();
}

std::shared_ptr< TestCase > TestCaseManager::load( const QString & name ) const {
	QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
	QSqlQuery query( database );

	bool prepared = query.prepare( "SELECT name,level,active,creator,description,mtime,program_options FROM testcases WHERE name=:name" );
	if( !prepared ) {
		throw SqlError( query.lastError().text() );
	}
	query.bindValue( ":name", name );
	if( !query.exec() || !query.next() ) {
		throw SqlError( QObject::tr( "%1 does not exist in database" ).arg( name ) );
	}

	QDir tcDir( Setting::instance().get( "test_cases_path" ).toString() );
	if( !tcDir.cd( name + ".sikuli" ) ) {
		throw IOError( QObject::tr( "%1.sikuli does not exist" ).arg( name ) );
	}

	std::shared_ptr< TestCase > testCase( new TestCase );
	testCase->setName( query.value( 0 ).toString() );
	testCase->setLevel( query.value( 1 ).toInt() );
	testCase->setActivated( query.value( 2 ).toBool() );
	testCase->setCreator( query.value( 3 ).toString() );
	testCase->setDescription( query.value( 4 ).toString() );
	testCase->setModifyTime( query.value( 5 ).toDateTime() );
	testCase->setProgramOptions( query.value( 6 ).toString() );

	QFile fin( tcDir.filePath( name + ".html" ) );
	if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		throw IOError( QObject::tr( "%1: %2" ).arg( fin.errorString() ).arg( fin.fileName() ) );
	}
	QTextStream sin( &fin );
	sin.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	testCase->setHTML( sin.readAll() );
	fin.close();

	fin.setFileName( tcDir.filePath( name + ".py" ) );
	if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		throw IOError( QObject::tr( "%1: %2" ).arg( fin.errorString() ).arg( fin.fileName() ) );
	}
	sin.setDevice( &fin );
	sin.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	testCase->setScript( sin.readAll() );
	fin.close();

	QStringList filter;
	std::map< QString, QUrl > mapping;
	// FIXME not only png files
	filter.append( "*.png" );
	filter = tcDir.entryList( filter );
	std::for_each( filter.begin(), filter.end(), [&tcDir, &mapping]( const QString & name )->void {
		mapping.insert( std::make_pair( name, QUrl::fromLocalFile( tcDir.filePath( name ) ) ) );
	} );
	testCase->setImageMapping( mapping );

	return testCase;
}

void TestCaseManager::save( std::shared_ptr< TestCase > testcase ) {
	QDir tcDir( Setting::instance().get( "test_cases_path" ).toString() );
	if( !tcDir.exists() ) {
		// if not exists, create it
		if( !tcDir.mkpath( "." ) ) {
			throw IOError( QObject::tr( "`%1\' not exists" ).arg( tcDir.absolutePath() ) );
		}
	}
	if( !tcDir.cd( testcase->getName() + ".sikuli" ) ) {
		// if not exists, create it
		if( !tcDir.mkdir( testcase->getName() + ".sikuli" ) ) {
			throw IOError( QObject::tr( "Can not create `%1\'" ).arg( tcDir.filePath( testcase->getName() + ".sikuli" ) ) );
		}
		tcDir.cd( testcase->getName() + ".sikuli" );
	}

	std::map< QString, QUrl > mapping( testcase->getImageMapping() );
	std::for_each( mapping.begin(), mapping.end(), [&tcDir]( std::map< QString, QUrl >::value_type & p )->void {
		QString sourcePath( p.second.toLocalFile() );
		QString targetPath( tcDir.filePath( p.first ) );
		if( sourcePath == targetPath ) {
			// skip copy if in same place
			return;
		}
		// TODO report errors here
		if( QFile::exists( targetPath ) ) {
			QFile::remove( targetPath );
		}
		QFile::copy( sourcePath, targetPath );
		p.second = QUrl::fromLocalFile( targetPath );
	} );
	testcase->setImageMapping( mapping );

	QFile fout( tcDir.filePath( testcase->getName() + ".html" ) );
	if( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		throw IOError( QObject::tr( "%1: %2" ).arg( fout.fileName() ).arg( fout.errorString() ) );
	}
	QTextStream sout( &fout );
	sout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	sout << testcase->getHTML();
	fout.close();

	fout.setFileName( tcDir.filePath( testcase->getName() + ".py" ) );
	if( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		throw IOError( QObject::tr( "%1: %2" ).arg( fout.fileName() ).arg( fout.errorString() ) );
	}
	sout.setDevice( &fout );
	sout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	sout << testcase->getScript();
	fout.close();

	QSqlDatabase database( QSqlDatabase::database( CONNECTION_NAME, false ) );
	if( !database.transaction() ) {
		throw SqlError( database.lastError().text() );
	}
	QSqlQuery query( database );
	bool prepared = query.prepare( "SELECT id FROM testcases WHERE name=:name" );
	if( !prepared ) {
		throw SqlError( query.lastError().text() );
	}
	query.bindValue( ":name", testcase->getName() );
	if( query.exec() && query.next() ) {
		// already exists, update new value
		prepared = query.prepare( "UPDATE testcases SET level=:level,active=:active,creator=:creator,description=:description,mtime=:mtime,program_options=:program_options WHERE name=:name" );
		if( !prepared ) {
			throw SqlError( query.lastError().text() );
		}
	} else {
		prepared = query.prepare( "INSERT INTO testcases (name,level,active,creator,description,mtime,program_options) VALUES (:name,:level,:active,:creator,:description,:mtime,:program_options)" );
		if( !prepared ) {
			throw SqlError( query.lastError().text() );
		}
	}
	query.bindValue( ":name", testcase->getName() );
	query.bindValue( ":level", testcase->getLevel() );
	query.bindValue( ":active", static_cast< int >( testcase->isActivated() ) );
	query.bindValue( ":creator", testcase->getCreator() );
	query.bindValue( ":description", testcase->getDescription() );
	query.bindValue( ":mtime", QDateTime::currentDateTime() );
	query.bindValue( ":program_options", testcase->getProgramOptions() );
	if( !query.exec() ) {
		throw SqlError( query.lastError().text() );
	}
	if( !database.commit() ) {
		throw SqlError( query.lastError().text() );
	}

	this->p_->refresh();
}
