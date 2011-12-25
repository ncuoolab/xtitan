#include "QxReplayUnit.h"
#include "TCP_Connection/TCPMessage.h"
#include "TcfDecoder/QxTestCase.h"

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>

#include <cassert>

QxReplayUnit::QxReplayUnit( QObject * parent ):
QxAbstractUnit( parent ),
waitingPoint_( NULL ),
errorMessage_() {
}

void QxReplayUnit::doOpen() {
	this->testCase().load();
	this->testCase().backupRemoteDatabase();
	this->testCase().importDatabase();
}

void QxReplayUnit::doTest() {
	// execute points
	foreach( QxPoint point, this->testCase().getPoints() ) {
		if( this->isCancled() ) {
			break;
		}
		point->execute( this );
	}
}

void QxReplayUnit::doClose() {
	this->testCase().restoreRemoteDatabase();

	const QxTestCase::CheckMap & checkPoints( this->testCase().getCheckPoints() );
	for( QxTestCase::CheckMap::const_iterator it = checkPoints.begin(); it != checkPoints.end(); ++it ) {
		foreach( QxCheckPoint * check, it->second ) {
			if( !check->isChecked() ) {
				this->errorMessage_.append( qMakePair( QObject::tr( "Missed" ), check->toString() ) );
			}
		}
	}

	if( !this->errorMessage_.isEmpty() ) {
		// error
		for( QList< QPair< QString, QString > >::const_iterator it = this->errorMessage_.begin(); it != this->errorMessage_.end(); ++it ) {
			emit this->error( it->first, it->second );
		}
	}
}

QxPacket QxReplayUnit::onCheck( int id, const QString & label, const QString & value ) {
	QxTestCase::CheckKey key( id, label );

	const QxTestCase::CheckMap & checkPoints( this->testCase().getCheckPoints() );
	QxTestCase::CheckMap::const_iterator it = checkPoints.find( key );
	if( it == checkPoints.end() ) {
		// TODO invalid id, maybe new check points
		return QxPacket( "<Success>", QVariant() );
	}

	// find if matched
	QxCheckPoint * matched = NULL;
	foreach( QxCheckPoint * check, it->second ) {
		if( check->isChecked() ) {
			continue;
		}
		if( value == check->getValue() ) {
			matched = check;
			matched->setChecked( true );
			// break waiting
			break;
		}
	}

	if( matched ) {	// passed
		// TODO log message
		// if matched check point is the locking one, release the lock
		if( matched == this->waitingPoint_ ) {
			emit this->releaseWaiting();
		}
	} else {	// value is wrong
		QString values;
		foreach( QxCheckPoint * point, it->second ) {
			values.append( ( point->isChecked() ? "[v] " : "[x] " ) + point->getValue() + "\n" );
		}

		this->errorMessage_.append( qMakePair( QObject::tr( "Failure" ), QObject::tr(
			"Check value not found:\n"
			"id: %1\n"
			"label: %2\n"
			"[?] %3\n"
			"%4\n" )
			.arg( id ).arg( label ).arg( value )
			.arg( values ) ) );
		return QxPacket( "<Failure>", QVariant() );
	}
	return QxPacket( "<Success>", QVariant() );
}

QxPacket QxReplayUnit::onInput( int /*id*/, const QString & /*label*/, const QString & /*script*/, qint64 /*waitTime*/ ) {
	// NOTE nop
	return QxPacket( "<Success>", QVariant() );
}

void QxReplayUnit::doInput( QxInputPoint * input ) {
	// wait a time
	QTimer timer;
	timer.setSingleShot( true );
	timer.setInterval( input->getWaitTime() );
	QEventLoop wait;
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	// FIXME should stop while error occurs
	timer.start();
	wait.exec();

	// send input control
	this->sendMessage( input->getID(), TCPMessage::Input, input->getScript() );
}

void QxReplayUnit::doCheck( QxCheckPoint * check ) {
	// if checked, do nothing
	if( check->isChecked() ) {
		return;
	}
	this->waitingPoint_ = check;

	// wait for checkpoint arrive, and error if timeout
	// FIXME should stop while error occurs
	QEventLoop wait;
	wait.connect( this, SIGNAL( releaseWaiting() ), SLOT( quit() ) );
	QTimer timer;
	timer.setSingleShot( true );
	timer.setInterval( 60000 );	// wait 60 secs
	timer.connect( this, SIGNAL( releaseWaiting() ), SLOT( stop() ) );
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	timer.start();
	wait.exec();

	// if still not checked, this is timeout event
	if( !check->isChecked() ) {
		this->setCanceled( true );
		this->errorMessage_.append( qMakePair( QObject::tr( "Timeout" ), check->toString() ) );
	}
	this->waitingPoint_ = NULL;
}

void QxReplayUnit::onError_( const QString & errorString ) {
	this->errorMessage_.append( qMakePair( QObject::tr( "Unknown" ), errorString ) );
	emit this->releaseWaiting();
}
