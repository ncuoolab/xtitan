#include "QxTestCase.h"
#include "Setting/Setting.h"
#include "TCP_Connection/QxRemoteController.h"

#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QVariantMap>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QBuffer>
#include <QtCore/QXmlStreamWriter>

#include <cassert>

QxTestCase::QxTestCase():
nClients_( 0 ),
name_(),
creator_(),
description_(),
points_(),
checkPoints_(),
remoteDatabase_(),
database_(),
databasePath_(),
recordedScript_() {
}

bool QxTestCase::save() {
	if( this->name_.isEmpty() ) {
		return false;
	}
	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	if( !root.cd( QString( "stories/%1" ).arg( this->name_ ) ) ) {
		if( !root.mkpath( QString( "stories/%1" ).arg( this->name_ ) ) ) {
			return false;
		}
		if( !root.cd( QString( "stories/%1" ).arg( this->name_ ) ) ) {
			return false;
		}
	}

	// save database
	QString databasePath( root.filePath( QString( "database.sql" ) ) );
	QFile fout( databasePath );
	if( !fout.open( QIODevice::WriteOnly ) ) {
		assert( !"Can not write database" );
	}
	fout.write( this->remoteDatabase_ );
	fout.close();

	// save script
	QString scriptPath( root.filePath( QString( "script.tcf" ) ) );
	fout.setFileName( scriptPath );
	if( !fout.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		assert( !"Can not write test script" );
	}
	QTextStream stream( &fout );
	stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	stream << this->nClients_ << endl;
	foreach( QString line, this->recordedScript_ ) {
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

	xmlWriter.writeTextElement( "creator", this->creator_ );

	xmlWriter.writeTextElement( "description", this->description_ );

	xmlWriter.writeTextElement( "ctime", QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) );

	xmlWriter.writeTextElement( "mtime", QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) );

	xmlWriter.writeEndElement();

	xmlWriter.writeEndDocument();

	fout.close();

	return true;
}

bool QxTestCase::load() {
	if( this->name_.isEmpty() ) {
		return false;
	}

	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	if( !root.cd( QString( "stories/%1" ).arg( this->name_ ) ) ) {
		// test case not exist
		assert( !"test case not exists" );
	}

	// load database
	this->databasePath_ = root.filePath( QString( "database.sql" ) );

	// load script
	QString scriptPath( root.filePath( QString( "script.tcf" ) ) );
	QFile fin( scriptPath );
	if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		assert( !"Can not open test script" );
	}

	QTextStream stream( &fin );
	stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	QString line( stream.readLine() );
	this->nClients_ = line.toInt();
	int lineCount = 0;
	while ( !stream.atEnd() ) {
		line = stream.readLine();	// line of text excluding '\n'
		QStringList tokens( line.split( "#" ) );
		if( tokens.at( 1 ) == "<Input>" ) {
			// input action
			this->points_.append( QxPoint( new QxInputPoint( tokens.at( 0 ).toInt(), tokens.at( 2 ), tokens.at( 3 ), tokens.at( 4 ).toInt(), lineCount ) ) );
		} else if( tokens.at( 1 ) == "<Check>" ) {
			QxCheckPoint * checkPoint = new QxCheckPoint( tokens.at( 0 ).toInt(), tokens.at( 2 ), tokens.at( 3 ), lineCount );

			this->points_.append( QxPoint( checkPoint ) );

			QPair< int, QString > key( checkPoint->getID(), checkPoint->getLabel() );
			CheckMap::iterator it = this->checkPoints_.find( key );
			if( it == this->checkPoints_.end() ) {
				QList< QxCheckPoint * > tmp;
				tmp.append( checkPoint );
				this->checkPoints_.insert( std::make_pair( key, tmp ) );
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

void QxTestCase::setClients( int nClients ) {
	this->nClients_ = nClients;
}

int QxTestCase::getClients() const {
	return this->nClients_;
}

const QxTestCase::CheckMap & QxTestCase::getCheckPoints() const {
	return this->checkPoints_;
}

const QList< QxPoint > & QxTestCase::getPoints() const {
	return this->points_;
}

void QxTestCase::backupRemoteDatabase() {
	this->remoteDatabase_.clear();
	QBuffer buffer( &this->remoteDatabase_ );
	while( !QxRemoteController::exportDatabase( &buffer ) );
}

void QxTestCase::restoreRemoteDatabase() {
	QBuffer buffer( &this->remoteDatabase_ );
	while( !QxRemoteController::importDatabase( &buffer ) );
}

void QxTestCase::importDatabase() {
	QFile fout( this->databasePath_ );
	while( !QxRemoteController::importDatabase( &fout ) );
}

void QxTestCase::writeLine( const QString & line ) {
	this->recordedScript_.append( line );
}

void QxTestCase::setDescription( const QString & description ) {
	this->description_ = description;
}

void QxTestCase::setCreator( const QString & creator ) {
	this->creator_ = creator;
}

void QxTestCase::setName( const QString & name ) {
	this->name_ = name;
}

const QString & QxTestCase::getName() const {
	return this->name_;
}
