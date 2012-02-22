#include "TestCase/TestCasePrivate.hpp"
#include "xTitan/Utility/Setting.hpp"
#include "Network/RemoteController.hpp"

#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QVariantMap>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QBuffer>
#include <QtCore/QXmlStreamWriter>

#include <cassert>

using xtitan::utilities::Setting;
using namespace xtitan::testcase;
using xtitan::network::RemoteController;

TestCase::Private::Private():
nClients( 0 ),
name(),
creator(),
description(),
points(),
checkPoints(),
remoteDatabase(),
database(),
databasePath(),
recordedScript() {
}

TestCase::TestCase():
p_( new Private ) {
}

bool TestCase::save() {
	if( this->p_->name.isEmpty() ) {
		return false;
	}
	QDir root( Setting::getInstance().get( "TestCasePath" ) );
	if( !root.cd( QString( "stories/%1" ).arg( this->p_->name ) ) ) {
		if( !root.mkpath( QString( "stories/%1" ).arg( this->p_->name ) ) ) {
			return false;
		}
		if( !root.cd( QString( "stories/%1" ).arg( this->p_->name ) ) ) {
			return false;
		}
	}

	// save database
	QString databasePath( root.filePath( QString( "database.sql" ) ) );
	QFile fout( databasePath );
	if( !fout.open( QIODevice::WriteOnly ) ) {
		assert( !"Can not write database" );
	}
	fout.write( this->p_->remoteDatabase );
	fout.close();

	// save script
	QString scriptPath( root.filePath( QString( "script.tcf" ) ) );
	fout.setFileName( scriptPath );
	if( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		assert( !"Can not write test script" );
	}
	QTextStream stream( &fout );
	stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	stream << this->p_->nClients << endl;
	foreach( QString line, this->p_->recordedScript ) {
		stream << line << endl;
	}
	fout.close();

	// save metadata
	fout.setFileName( root.filePath( QString( "metadata.xml" ) ) );
	if( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		assert( !"Can not write metadata" );
	}
	QXmlStreamWriter xmlWriter( &fout );
	xmlWriter.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	xmlWriter.setAutoFormatting( true );
	xmlWriter.writeStartDocument( "teststory" );

	xmlWriter.writeStartElement( "story" );

	xmlWriter.writeTextElement( "creator", this->p_->creator );

	xmlWriter.writeTextElement( "description", this->p_->description );

	xmlWriter.writeTextElement( "ctime", QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) );

	xmlWriter.writeTextElement( "mtime", QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) );

	xmlWriter.writeEndElement();

	xmlWriter.writeEndDocument();

	fout.close();

	return true;
}

bool TestCase::load() {
	if( this->p_->name.isEmpty() ) {
		return false;
	}

	QDir root( Setting::getInstance().get( "TestCasePath" ) );
	if( !root.cd( QString( "stories/%1" ).arg( this->p_->name ) ) ) {
		// test case not exist
		assert( !"test case not exists" );
	}

	// load database
	this->p_->databasePath = root.filePath( QString( "database.sql" ) );

	// load script
	QString scriptPath( root.filePath( QString( "script.tcf" ) ) );
	QFile fin( scriptPath );
	if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		assert( !"Can not open test script" );
	}

	QTextStream stream( &fin );
	stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	QString line( stream.readLine() );
	this->p_->nClients = line.toInt();
	int lineCount = 0;
	while ( !stream.atEnd() ) {
		line = stream.readLine();	// line of text excluding '\n'
		QStringList tokens( line.split( "#" ) );
		if( tokens.at( 1 ) == "<Input>" ) {
			// input action
			this->p_->points.append( Point( new InputPoint( tokens.at( 0 ).toInt(), tokens.at( 2 ), tokens.at( 3 ), tokens.at( 4 ).toInt(), lineCount ) ) );
		} else if( tokens.at( 1 ) == "<Check>" ) {
			CheckPoint * checkPoint = new CheckPoint( tokens.at( 0 ).toInt(), tokens.at( 2 ), tokens.at( 3 ), lineCount );

			this->p_->points.append( Point( checkPoint ) );

			QPair< int, QString > key( checkPoint->getID(), checkPoint->getLabel() );
			CheckMap::iterator it = this->p_->checkPoints.find( key );
			if( it == this->p_->checkPoints.end() ) {
				QList< CheckPoint * > tmp;
				tmp.append( checkPoint );
				this->p_->checkPoints.insert( std::make_pair( key, tmp ) );
			} else {
				it->second.append( checkPoint );
			}
		} else {
			// invalid
			assert( !"INVALID COMMAND" );
		}
		++lineCount;
	}
	fin.close();

	// TODO load metadata

	return true;
}

void TestCase::setClients( int nClients ) {
	this->p_->nClients = nClients;
}

int TestCase::getClients() const {
	return this->p_->nClients;
}

const TestCase::CheckMap & TestCase::getCheckPoints() const {
	return this->p_->checkPoints;
}

const QList< Point > & TestCase::getPoints() const {
	return this->p_->points;
}

void TestCase::backupRemoteDatabase() {
	this->p_->remoteDatabase.clear();
	QBuffer buffer( &this->p_->remoteDatabase );
	while( !RemoteController::exportDatabase( &buffer ) );
}

void TestCase::restoreRemoteDatabase() {
	QBuffer buffer( &this->p_->remoteDatabase );
	while( !RemoteController::importDatabase( &buffer ) );
}

void TestCase::importDatabase() {
	QFile fout( this->p_->databasePath );
	while( !RemoteController::importDatabase( &fout ) );
}

void TestCase::writeLine( const QString & line ) {
	this->p_->recordedScript.append( line );
}

void TestCase::setDescription( const QString & description ) {
	this->p_->description = description;
}

void TestCase::setCreator( const QString & creator ) {
	this->p_->creator = creator;
}

void TestCase::setName( const QString & name ) {
	this->p_->name = name;
}

const QString & TestCase::getName() const {
	return this->p_->name;
}
