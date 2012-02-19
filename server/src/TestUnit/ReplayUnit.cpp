#include "ReplayUnitPrivate.hpp"
#include "Network/TCPMessage.hpp"
#include "TestCase/TestCase.hpp"
#include "TestCase/InputPoint.hpp"

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>

#include <cassert>

using namespace xtitan::testunit;
using xtitan::testcase::TestCase;
using xtitan::testcase::Point;
using xtitan::testcase::CheckPoint;
using xtitan::testcase::InputPoint;
using xtitan::testcase::SocketPoint;
using xtitan::network::SimpleSocket;

ReplayUnit::Private::Private( ReplayUnit * host ):
QObject( host ),
waitingPoint( NULL ),
errorMessage() {
}

void ReplayUnit::Private::onError( const QString & errorString ) {
	this->errorMessage.append( qMakePair( QObject::tr( "Unknown" ), errorString ) );
	emit this->releaseWaiting();
}

ReplayUnit::ReplayUnit( QObject * parent ):
AbstractUnit( parent ),
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( releaseWaiting() ), SIGNAL( releaseWaiting() ) );
}

void ReplayUnit::doOpen() {
	this->getTestCase()->load();
	this->getTestCase()->backupRemoteDatabase();
	this->getTestCase()->importDatabase();
}

void ReplayUnit::doTest() {
	// execute points
	foreach( Point point, this->getTestCase()->getPoints() ) {
		if( this->isCancled() ) {
			break;
		}
		point->execute( this );
	}
}

void ReplayUnit::doClose() {
	this->getTestCase()->restoreRemoteDatabase();

	const TestCase::CheckMap & checkPoints( this->getTestCase()->getCheckPoints() );
	for( TestCase::CheckMap::const_iterator it = checkPoints.begin(); it != checkPoints.end(); ++it ) {
		foreach( CheckPoint * check, it->second ) {
			if( !check->isChecked() ) {
				this->p_->errorMessage.append( qMakePair( QObject::tr( "Missed" ), check->toString() ) );
			}
		}
	}

	if( !this->p_->errorMessage.isEmpty() ) {
		// error
		for( QList< QPair< QString, QString > >::const_iterator it = this->p_->errorMessage.begin(); it != this->p_->errorMessage.end(); ++it ) {
			emit this->error( it->first, it->second );
		}
	}
}

SimpleSocket::Packet ReplayUnit::onCheck( int id, const QString & label, const QString & value ) {
	TestCase::CheckKey key( id, label );

	const TestCase::CheckMap & checkPoints( this->getTestCase()->getCheckPoints() );
	TestCase::CheckMap::const_iterator it = checkPoints.find( key );
	if( it == checkPoints.end() ) {
		// TODO invalid id, maybe new check points
		return SimpleSocket::Packet( "<Success>", QVariant() );
	}

	// find if matched
	CheckPoint * matched = NULL;
	foreach( CheckPoint * check, it->second ) {
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
		if( matched == this->p_->waitingPoint ) {
			emit this->releaseWaiting();
		}
	} else {	// value is wrong
		QString values;
		foreach( CheckPoint * point, it->second ) {
			values.append( ( point->isChecked() ? "[v] " : "[x] " ) + point->getValue() + "\n" );
		}

		this->p_->errorMessage.append( qMakePair( QObject::tr( "Failure" ), QObject::tr(
			"Check value not found:\n"
			"id: %1\n"
			"label: %2\n"
			"[?] %3\n"
			"%4\n" )
			.arg( id ).arg( label ).arg( value )
			.arg( values ) ) );
		return SimpleSocket::Packet( "<Failure>", QVariant() );
	}
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

SimpleSocket::Packet ReplayUnit::onInput( int /*id*/, const QString & /*label*/, const QString & /*script*/, qint64 /*waitTime*/ ) {
	// NOTE nop
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

SimpleSocket::Packet ReplayUnit::onSocket( int /*id*/, const QString & /*message*/ ) {
	// NOTE nop
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

void ReplayUnit::doInput( InputPoint * input ) {
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

void ReplayUnit::doCheck( CheckPoint * check ) {
	// if checked, do nothing
	if( check->isChecked() ) {
		return;
	}
	this->p_->waitingPoint = check;

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
		this->p_->errorMessage.append( qMakePair( QObject::tr( "Timeout" ), check->toString() ) );
	}
	this->p_->waitingPoint = NULL;
}

void ReplayUnit::doSocket( SocketPoint * /*socket*/ ) {
	// TODO send socket message
}
