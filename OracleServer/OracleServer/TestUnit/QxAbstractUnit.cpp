#include "QxAbstractUnitPrivate.h"
#include "TcfDecoder/QxAbstractPoint.h"
#include "TCP_Connection/TCPMessage.h"
#include "Setting/Setting.h"

#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QEventLoop>

QxAbstractUnit::Private::Private( QxAbstractUnit * parent ):
QObject( parent ),
parent_( parent ),
sockets(),
children_(),
testCase(),
commands(),
canceled( false ) {
//	this->commands.insert( std::make_pair( TCPMessage::ReqJoinUnit, std::tr1::bind( &QxAbstractUnit::Private::onJoin, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2 ) ) );
	this->commands.insert( std::make_pair( TCPMessage::Input, std::tr1::bind( &QxAbstractUnit::Private::onInput, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2 ) ) );
	this->commands.insert( std::make_pair( TCPMessage::Check, std::tr1::bind( &QxAbstractUnit::Private::onCheck, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2 ) ) );
}

void QxAbstractUnit::Private::addClient( QxSimpleSocket * socket ) {
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

void QxAbstractUnit::Private::spawnChiwawa() {
	// spawn chiwawa
	QFileInfo msnShell( QxSetting::Instance().get( "MsnShellProgram" ) );
	QDir testCase( QxSetting::Instance().get( "TestCasePath" ) );

	for( int i = 0; i < this->testCase.getClients(); ++i ) {
		QStringList args;
		QString logPath( QString( "log/tmp/Chiwawa.%1" ).arg( i ) );
		testCase.mkpath( QString( "%1/log" ).arg( logPath ) );

		args.append( "-nf" );
		args.append( "-t" );
		// TODO: only one test unit at a time
		//		args.append( this->getName() );
		args.append( "-if" );
		args.append( QxSetting::Instance().get( "ConfigurationPath" ) );
		args.append( "-of" );
		args.append( testCase.filePath( logPath ) );
		QProcess * shell = new QProcess;
		shell->setWorkingDirectory( msnShell.path() );
		qDebug() << QString( "%1 %2" ).arg( msnShell.filePath() ).arg( args.join( " " ) );
		shell->start( msnShell.filePath(), args );
		this->children_.append( shell );
	}
}

void QxAbstractUnit::Private::restartServer() {
	qDebug() << QObject::tr( "restart server" );
	QFileInfo portal( QxSetting::Instance().get( "PortalServer" ) );
	QProcess p;
	p.setWorkingDirectory( portal.path() );
	p.start( portal.filePath() );
	p.waitForFinished( -1 );
	qDebug() << QObject::tr( "restart server completed" );
	_sleep( 1000 );
}

void QxAbstractUnit::Private::onSocketReadyRead() {
	QxSimpleSocket * socket = static_cast< QxSimpleSocket * >( this->sender() );
	assert( socket || !"socket is null" );
	while( !socket->atEnd() ) {
		QxPacket packet( socket->read() );
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

void QxAbstractUnit::Private::onSocketError( QLocalSocket::LocalSocketError socketError ) {
	QxSimpleSocket * socket = static_cast< QxSimpleSocket * >( this->sender() );
	this->canceled = true;
	emit this->log( QObject::tr( "Connection" ), socket->errorString() );
}

void QxAbstractUnit::Private::onSocketDisconnected() {
	emit this->log( QObject::tr( "Connection" ), QObject::tr( "Disconnected" ) );
}

QxPacket QxAbstractUnit::Private::onCheck( QxSimpleSocket * sender, const QVariant & data ) {
	int id = this->sockets.indexOf( sender );
	assert( ( id >= 0 && id < this->sockets.size() ) || !"invalid socket" );
	QVariantMap args( data.toMap() );
	QString label( args["label"].toString() );
	QString value( args["value"].toString() );

	return this->parent_->onCheck( id, label, value );
}

QxPacket QxAbstractUnit::Private::onInput( QxSimpleSocket * sender, const QVariant & data ) {
	int id = this->sockets.indexOf( sender );
	assert( ( id >= 0 && id < this->sockets.size() ) || !"invalid socket" );
	QVariantMap args( data.toMap() );
	QString label( args["label"].toString() );
	QString script( args["script"].toString() );
	int waitTime( args["waitTime"].toInt() );

	return this->parent_->onInput( id, label, script, waitTime );
}

QxPacket QxAbstractUnit::Private::onJoin( QxSimpleSocket * sender, const QVariant & data ) {
	return QxPacket();
}

QxAbstractUnit::QxAbstractUnit( QObject * parent ):
QObject( parent ),
p_( new QxAbstractUnit::Private( this ) ) {
	this->connect( this->p_, SIGNAL( error( const QString &, const QString & ) ), SIGNAL( error( const QString &, const QString & ) ) );
	this->connect( this->p_, SIGNAL( log( const QString &, const QString & ) ), SIGNAL( log( const QString &, const QString & ) ) );
}

QxAbstractUnit::~QxAbstractUnit() {
}

void QxAbstractUnit::addClient( QxSimpleSocket * socket ) {
	this->p_->addClient( socket );
}

const QString & QxAbstractUnit::getName() const {
	return this->p_->testCase.getName();
}

void QxAbstractUnit::setName( const QString & name ) {
	this->p_->testCase.setName( name );
}

bool QxAbstractUnit::isCancled() const {
	return this->p_->canceled;
}

void QxAbstractUnit::check( QxCheckPoint * point ) {
	this->doCheck( point );
}

void QxAbstractUnit::input( QxInputPoint * point ) {
	this->doInput( point );
}

void QxAbstractUnit::run() {
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
	foreach( QxSimpleSocket * socket, this->p_->sockets ) {
		qDebug() << QObject::tr( "Send quit command" );
		// NOTE sockets are on a diffirent thread
		QMetaObject::invokeMethod( socket, "write", Q_ARG( const QString &, TCPMessage::QuitApplicationTag ), Q_ARG( const QVariant &, QVariant() ) );
		QMetaObject::invokeMethod( socket, "disconnectFromServer" );
		socket->deleteLater();
	}
	// wait for process normal exit, or kill it
	foreach( QProcess * p, this->p_->children_ ) {
		qDebug() << QObject::tr( "wait process exit" );
		if( !p->waitForFinished( 3000 ) ) {
			// delete this object after the process been killed
			p->connect( p, SIGNAL( finished( int, QProcess::ExitStatus ) ), SLOT( deleteLater() ) );
			p->kill();
			qDebug() << QObject::tr( "killed process" );
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

QxTestCase & QxAbstractUnit::testCase() {
	return this->p_->testCase;
}

const QxTestCase & QxAbstractUnit::getTestCase() const {
	return this->p_->testCase;
}

void QxAbstractUnit::sendMessage( int id, const QString & command, const QVariant & data ) {
	QxSimpleSocket * socket = this->p_->sockets.at( id );
	// NOTE this hack is because socket is on diffirent thread
	QMetaObject::invokeMethod( socket, "write", Q_ARG( const QString &, command ), Q_ARG( const QVariant &, data ) );
}

void QxAbstractUnit::setCanceled( bool canceled ) {
	this->p_->canceled = canceled;
	if( canceled ) {
		emit this->canceled();
	}
}

int QxAbstractUnit::getClients() const {
	return this->p_->testCase.getClients();
}

void QxAbstractUnit::setClients( int nClients ) {
	this->p_->testCase.setClients( nClients );
}
