#include "TestUnit/QxTestUnit.h"
#include "TCP_Connection/TCPMessageComposer.h"
#include <cassert>
#include "TestUnitManager/TestUnitManager.h"
#include "Setting/Setting.h"
#include <QDatetime>
#include "View/QxTestUnitView.h"
#include "TCP_Connection/QxRemoteController.h"
#include "TcfDecoder/QxCheckPoint.h"
#include "TcfDecoder/QxInputPoint.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QDir>
#include <QtCore/QTextCodec>
#include <QtCore/QEventLoop>

#include <iostream>

QxTestUnit::QxTestUnit( const QString & name ):
QObject(),
link_( new QxRemoteController( this ) ),
view( new QxTestUnitView ),
timer_( new QTimer( this ) ),
timeout_( new QTimer( this ) ),
name_( name ),
currentInput_( 0 ),
points_(),
checkPoints_(),
waitingCheck_( NULL ),
clients_(),
nClients_( 0 ),
killing_( true ),
currentState_( INIT ),
checkPointUnordered_( false ),
lastTick_( 0 ) {
	this->Setup();
	this->timer_->setSingleShot( true );
	this->connect( this->timer_, SIGNAL( timeout() ), SLOT( executeNextInput() ) );
	this->timeout_->setSingleShot( true );
	this->timeout_->setInterval( 60000 );
	this->connect( this->timeout_, SIGNAL( timeout() ), SLOT( onTimeout_() ) );
}

QxTestUnit::~QxTestUnit() {
	this->view->deleteLater();
}

QWidget * QxTestUnit::getView() const {
	return this->view;
}

const QString & QxTestUnit::getName() const {
	return this->name_;
}

int QxTestUnit::getClientCount() const {
	return this->clients_.size();
}

QxTestUnit::UnitStates QxTestUnit::getState() const {
	return this->currentState_;
}

void QxTestUnit::setState( QxTestUnit::UnitStates state ) {
	this->currentState_ = state;
}

void QxTestUnit::addClient( TestClient * client ) {
	client->setID( this->clients_.size() );
	this->clients_.insert( client->getID(), client );

	this->view->appendLog( QString( "Client %1 connected" ).arg( client->getID() ) );
	client->setTestUnit( this );
	this->connect( client, SIGNAL( error( const QString & ) ), SLOT( onClientError_( const QString & ) ) );
	this->connect( client, SIGNAL( disconnected() ), SLOT( onClientDisconnected_() ) );
	this->view->appendLog("Client " + QString::number( client->getID() ) + " is connected from "+client->getIP() + ".");

	if( this->nClients_ == this->clients_.size() ) {
		this->lastTick_ = QDateTime::currentMSecsSinceEpoch();
		emit this->ready();
	}
}

TestClient* QxTestUnit::getClient( int id ) const {
	QMap< int, TestClient * >::const_iterator it = this->clients_.find( id );
	if(it != this->clients_.end()){
		return it.value();
	}else{
		return NULL;
	}
}

QList<TestClient*> QxTestUnit::getClientList() const {
	QList<TestClient*> list;
	QMap< int, TestClient * >::iterator it = this->clients_.begin();
	while (it != this->clients_.end()){
		list.append( it.value() );
		it++;
	}
	return list;
}

void QxTestUnit::wait( int msec ) {
	this->timer_->start( msec );
}

void QxTestUnit::wait( QxCheckPoint * point ) {
	this->waitingCheck_ = point;
	this->timeout_->start();
}

void QxTestUnit::executeNextInput() {
	if( this->currentInput_ >= this->points_.size() ) {
		// reach end
		this->finish();
		return;
	}
	std::tr1::shared_ptr< QxAbstractPoint > point( this->points_.at( this->currentInput_ ) );
	this->view->appendLog( point->toString() );
//	point->execute( this );
	++this->currentInput_;
}

void QxTestUnit::check( int id, const QString & label, const QString & value ) {
	if( this->getState() & REC ) {
		this->record( id, label, value );
		return;
	}
	view->appendLog( "Check : label=" + label + ", value=" + value );

	QPair< int, QString > key( id, label );
	QMap< QPair< int, QString >, QList< QxCheckPoint * > >::const_iterator it = this->checkPoints_.find( key );
	if( it == this->checkPoints_.end() ) {
		// FIXME: log to view
		qDebug() << "ignored: new checkpoint";
		return;
	}
	QxCheckPoint * matched = NULL;
	foreach( QxCheckPoint * point, it.value() ) {
		if( point->isChecked() ) {
			continue;
		}
		if( point->getValue() == value ) {
			point->setChecked( true );
			matched = point;
			break;
		}
	}
	if( matched ) {
		this->view->appendLog( QString( "Passed check point: { id: %1, label: \"%2\", value: \"%3\" }\n" ).arg( matched->getID() ).arg( matched->getLabel() ).arg( matched->getValue() ) );
		if( this->waitingCheck_ && this->waitingCheck_->isChecked() ) {
			this->waitingCheck_ = NULL;
			this->timer_->start( 0 );
		}
	} else {
		// failed
		this->view->appendLog( "Check point failed" );

		QString values;
		foreach( QxCheckPoint * point, it.value() ) {
			values.append( ( point->isChecked() ? "[v] " : "[x] " ) + point->getValue() + "\n" );
		}

		QString msg( QString(
			"Check value not found:\n"
			"id: %1\n"
			"label: %2\n"
			"[?] %3\n"
			"%4\n" )
			.arg( id ).arg( label ).arg( value )
			.arg( values ) );
		this->onClientError_( msg );
	}
}

void QxTestUnit::startRecord( int nClients ){
	this->currentState_ = REC;
	this->recordedScirpt_.clear();
	this->nClients_ = nClients;

	this->spawnChiwawa_();
}

bool QxTestUnit::startReplay( const QString & testName ) {
	this->currentState_ = PLAY;

	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	this->importDatabase_( root.filePath( QString( "stories/%1/database.sql" ).arg( testName ) ) );
	this->loadScript_( root.filePath( QString( "stories/%1/script.tcf" ).arg( testName ) ) );

//	this->view->setInputPoints( this->inputPoints_ );
//	this->view->setCheckPoints( this->checkPoints_ );

	this->connect( this, SIGNAL( ready() ), SLOT( executeNextInput() ) );

	this->spawnChiwawa_();

	return true;
}

bool QxTestUnit::startUpdate( const QString & testName ) {
	this->currentState_ = REC | PLAY;

	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	QString srcSQL( root.filePath( QString( "stories/%1/database.sql" ).arg( testName ) ) );
	this->importDatabase_( srcSQL );
	QFile::copy( srcSQL, root.filePath( QString( "stories/%1/database.sql" ).arg( this->getName() ) ) );
	this->loadScript_( root.filePath( QString( "stories/%1/script.tcf" ).arg( testName ) ) );

//	this->view->setInputPoints( this->inputPoints_ );
	this->recordedScirpt_.clear();

	this->connect( this, SIGNAL( ready() ), SLOT( executeNextInput() ) );

	this->spawnChiwawa_();

	return true;
}

bool QxTestUnit::saveRecord( const QString & name, const QString & creator, const QString & description ) {
	if( !this->saveScript( name ) ) {
		return false;
	}

	QDir dir( QxSetting::Instance().get( "TestCasePath" ) );
	this->databasePath_ = dir.filePath( QString( "stories/%1/database.sql" ).arg( name ) );
	if( !( this->getState() & PLAY ) ) {
		QFile::rename( dir.filePath( QString( "stories/%1/%2.sql" ).arg( name ).arg( this->getName() ) ), this->databasePath_ );
	}

// 	QDomDocument doc( "teststory" );
// 	QDomElement root( doc.createElement( "story" ) );
// 	doc.appendChild( root );
// 	
// 	QDomElement node = doc.createElement( "creator" );
// 	root.appendChild( node );
// 	QDomText text = doc.createTextNode( creator );
// 	node.appendChild( text );
// 
// 	node = doc.createElement( "description" );
// 	root.appendChild( node );
// 	text = doc.createTextNode( description );
// 	node.appendChild( text );
// 
// 	node = doc.createElement( "ctime" );
// 	root.appendChild( node );
// 	text = doc.createTextNode( QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) );
// 	node.appendChild( text );
// 
// 	node = doc.createElement( "mtime" );
// 	root.appendChild( node );
// 	text = doc.createTextNode( QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) );
// 	node.appendChild( text );
// 
// 	QFile fout( dir.filePath( QString( "stories/%1/metadata.xml" ).arg( name ) ) );
// 	fout.open( QIODevice::WriteOnly | QIODevice::Text );
// 	fout.write( doc.toString().toUtf8() );
// 	fout.close();

	emit this->requestDestroy();
	return true;
}

void QxTestUnit::record( int id, const QString & label, const QString & script, qint64 timestamp ) {
	qint64 waitTime = ( timestamp < this->lastTick_ ) ? 0 : ( timestamp - this->lastTick_ );
	this->lastTick_ = timestamp + waitTime;
	QxInputPoint input( id, label, script, waitTime, 0 );
	this->recordedScirpt_.append( input.toString() );	// FIXME
	this->view->appendInput( input );
}

void QxTestUnit::record( int id, const QString & label, const QString & value ) {
	QxCheckPoint check( id, label, value, 0 );

	this->recordedScirpt_.append( check.toString() );	// FIXME
}

void QxTestUnit::appendLog( const QString & msg ) {
	view->appendLog(msg);
}

void QxTestUnit::setKill( bool killing ) {
	this->killing_ = killing;
}

void QxTestUnit::Setup() {
	qDebug() << "Setup" << this->getName();
	//backup current db
	qDebug() << "dumping database";
	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	if( !root.exists( "stories/" + this->getName() ) ) {
		root.mkpath( "stories/" + this->getName() );
	}
	this->databasePath_ = root.filePath( QString( "stories/%1/%2.sql" ).arg( this->getName() ).arg( QDateTime::currentDateTime().toString( "yyyyMMddhhmmss" ) ) );
	this->exportDatabase_( this->databasePath_ );
	if( !QFile::exists( this->databasePath_ ) ) {
		assert( !"database export failed" );
	}
	//restart server
	qDebug() << "restarting server";
	this->restartServer_();
}

void QxTestUnit::teardown(){
	qDebug() << "Teardone" << this->getName();
	this->timer_->stop();
	this->timeout_->stop();
	//recover the backup db
	qDebug() << "recovering database";
	this->importDatabase_( this->databasePath_ );
	if( this->currentState_ & PLAY ) {
		QFile f( this->databasePath_ );
		if( !f.remove() ) {
			qDebug() << f.errorString();
		} else {
			qDebug() << "Removed" << this->databasePath_;
		}
	}
	// restart server
	qDebug() << "restarting server";
	this->restartServer_();
}

void QxTestUnit::onClientDisconnected_() {
	TestClient * client = static_cast< TestClient * >( this->sender() );
	this->clients_.remove( client->getID() );
	if( this->clients_.isEmpty() ) {
		// kill all remaining process
		foreach( QProcess * p, this->childs_ ) {
			if( this->killing_ ) {
				p->kill();
			}
			p->deleteLater();
		}
		this->childs_.clear();
		this->teardown();
		if( !( this->getState() & REC ) ) {
			emit this->requestDestroy();
		}
	}
}

void QxTestUnit::onClientError_( const QString & msg ) {
	if( this->getState() & FIN ) {
		return;
	}
	this->setState( this->getState() | FIN );

	this->generateReport( msg );
	this->closeClients();
	QFile f( this->databasePath_ );
	if( !f.remove() ) {
		qDebug() << f.errorString();
	}
	emit this->error( this->getName() );
}

void QxTestUnit::onTimeout_() {
	if( this->getState() & REC ) {
		// in update mode, check point timeout means it should be drop
		this->timer_->start( 0 );
	} else {
		this->onClientError_( QObject::tr( "Client response timeout." ) );
	}
}

void QxTestUnit::stopReplay() {
	if( this->getState() & FIN ) {
		return;
	}
	this->setState( this->getState() | FIN );

	this->timer_->stop();
	this->closeClients();
	emit this->requestDestroy();
}

void QxTestUnit::stopRecord() {
	if( this->getState() & FIN ) {
		return;
	}
	this->setState( this->getState() | FIN );

	this->closeClients();
}

void QxTestUnit::finish() {
	if( this->getState() & FIN ) {
		return;
	}
	this->setState( this->getState() | FIN );

	this->generateReport( "" );
	if( this->getState() & REC ) {
		this->closeClients();
		emit this->requestSave( this->getName() );
	} else {
		this->closeClients();
	}
}

void QxTestUnit::closeClients() {
	foreach( TestClient * client, this->clients_ ) {
		client->AckQuit();
		client->disconnectFromHost();
	}
}

void QxTestUnit::generateReport( const QString & errorMessage ) {
	QString msg;

	// check if there are checkpoints unchecked
	QStringList tmp;
	foreach( QList< QxCheckPoint * > cl, this->checkPoints_ ) {
		foreach( QxCheckPoint * cp, cl ) {
			if( !cp->isChecked() ) {
				tmp.append( QString( "%1 (%2)" ).arg( cp->toString() ).arg( cp->getLine() + 2 ) );
			}
		}
	}
	if( tmp.empty() ) {
		if( this->checkPointUnordered_ ) {
			emit this->log( this->getName(), "[Warning] Check points shuffled." );
			msg = "Warning";
		} else {
			emit this->log( this->getName(), "[Information] All pass!" );
			msg = "Pass";
		}
	} else {
		if( this->currentInput_ < this->points_.size() ) {
			msg = QString( "[Error] %1\nFail on line %3: %2\n" ).arg( errorMessage ).arg( this->points_.at( this->currentInput_ - 1 )->toString() ).arg( this->points_.at( this->currentInput_ - 1 )->getLine() + 2 );
			emit this->log( this->getName(), msg );
		} else {
			msg = QString( "[Error] The fallowing checkpoint(s) are not checked:\n%1" ).arg( tmp.join( "\n" ) );
			emit this->log( this->getName(), msg );
		}
		std::cout << "<failure type=\"Check Point\"><![CDATA[" << msg.toUtf8().constData() << "]]></failure>" << std::endl;
	}

	this->saveResult( msg );
}

bool QxTestUnit::saveScript( const QString & testName ) {
	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	root.cd( "stories" );
	if( root.exists( testName ) || !root.rename( this->getName(), testName ) ) {
		return false;
	}

	QFile fout( root.filePath( QString( "%1/script.tcf" ).arg( testName ) ) );
	bool result = fout.open( QFile::WriteOnly | QFile::Text );
	assert( result );
	QTextStream s( &fout );
	s.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	s << this->nClients_ << endl;
	foreach( QString line, this->recordedScirpt_ ) {
		s << line << endl;
	}
	fout.close();

	return true;
}

void QxTestUnit::saveResult( const QString & msg ) {
	QDir resultDir( QxSetting::Instance().get( "TestCasePath" ) );
	QDateTime current_time = QDateTime::currentDateTime();

	QString result = "Test Name : "+ getName() + "\n";
	result += "Testing time : "+current_time.toString() + "\n";
	result += "Test Result : \n";

	result += msg;

	QString dir;
	if( msg == "Pass" ) {
		dir = msg;
	} else if( msg == "Warning" ) {
		dir = msg;
	} else {
		dir = "Fail";
	}
	QString filename( resultDir.filePath( QString( "log/tmp/%1/%2.txt" ).arg( dir ).arg( this->getName() ) ) );
	QFile fOut( filename );
	bool ret = fOut.open( QFile::WriteOnly | QFile::Text );
	if( !ret ) {
		// dont write log;
		return;
	}
	QTextStream s(&fOut);
	s << result;
	fOut.close();
}

void QxTestUnit::loadScript_( const QString & filePath ) {
	QFile file( filePath );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) { 
		QTextStream stream( &file );
		stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
		QString line( stream.readLine() );
		this->nClients_ = line.toInt();
		int lineCount = 0;
		while ( !stream.atEnd() ) {
			line = stream.readLine();	// line of text excluding '\n'
			QStringList tokens( line.split( "#" ) );
			if( tokens.at( 1 ) == "<Input>" ) {
				// input action
				this->points_.append( std::tr1::shared_ptr< QxAbstractPoint >( new QxInputPoint( tokens.at( 0 ).toInt(), tokens.at( 2 ), tokens.at( 3 ), tokens.at( 4 ).toInt(), lineCount ) ) );
			} else if( tokens.at( 1 ) == "<Check>" ) {
				QxCheckPoint * checkPoint = new QxCheckPoint( tokens.at( 0 ).toInt(), tokens.at( 2 ), tokens.at( 3 ), lineCount );
				this->points_.append( std::tr1::shared_ptr< QxAbstractPoint >( checkPoint ) );

				QPair< int, QString > key( checkPoint->getID(), checkPoint->getLabel() );
				QMap< QPair< int, QString >, QList< QxCheckPoint * > >::iterator it = this->checkPoints_.find( key );
				if( it == this->checkPoints_.end() ) {
					QList< QxCheckPoint * > tmp;
					tmp.append( checkPoint );
					this->checkPoints_.insert( key, tmp );
				} else {
					it.value().append( checkPoint );
				}
			} else {
				// invalid
				assert( !"INVALID COMMAND" );
			}
			++lineCount;
		}
		file.close();
	}
}

void QxTestUnit::spawnChiwawa_() {
	// spawn chiwawa
	QFileInfo msnShell( QxSetting::Instance().get( "MsnShellProgram" ) );
	QFileInfo msnRender( QxSetting::Instance().get( "MsnRenderProgram" ) );
	QDir testCase( QxSetting::Instance().get( "TestCasePath" ) );
	for( int i = 0; i < this->nClients_; ++i ) {
		QStringList args;
		QString logPath( QString( "log/tmp/Chiwawa.%1" ).arg( i ) );
		testCase.mkpath( QString( "%1/log" ).arg( logPath ) );

		args.append( "-pn" );
		args.append( "ChIwAwA" + QString::number( i ) );
		QStringList argr( args );
		argr.append( "-o" );
		argr.append( testCase.filePath( logPath ) );
		QProcess * render = new QProcess( this );
		render->setWorkingDirectory( msnRender.path() );
		render->start( msnRender.filePath(), argr );
		this->childs_.append( render );

		args.append( "-t" );
		args.append( this->getName() );
		args.append( "-if" );
		args.append( QxSetting::Instance().get( "ConfigurationPath" ) );
		args.append( "-of" );
		args.append( testCase.filePath( logPath ) );
		QProcess * shell = new QProcess( this );
		shell->setWorkingDirectory( msnShell.path() );
		shell->start( msnShell.filePath(), args );
		this->childs_.append( shell );
	}
}

void QxTestUnit::importDatabase_( const QString & filePath ) {
	this->link_->importDatabase( filePath );
	QEventLoop * wait = new QEventLoop;
	wait->connect( this->link_, SIGNAL( finished() ), SLOT( quit() ) );
	wait->exec();
	wait->deleteLater();
}

void QxTestUnit::exportDatabase_( const QString & filePath ) {
	this->link_->exportDatabase( filePath );
	QEventLoop * wait = new QEventLoop;
	wait->connect( this->link_, SIGNAL( finished() ), SLOT( quit() ) );
	wait->exec();
	wait->deleteLater();
}

void QxTestUnit::restartServer_() {
	QFileInfo portal( QxSetting::Instance().get( "PortalServer" ) );
	QProcess p;
	p.setWorkingDirectory( portal.path() );
	p.start( portal.filePath() );
	p.waitForFinished( -1 );
	_sleep( 1000 );
}
