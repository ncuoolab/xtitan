#include "QxUpdateUnit.h"
#include "TcfDecoder/QxCheckPoint.h"
#include "TcfDecoder/QxInputPoint.h"
#include "TcfDecoder/QxTestCase.h"
#include "TCP_Connection/TCPMessage.h"

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

QxUpdateUnit::QxUpdateUnit( QObject * parent ):
QxAbstractUnit( parent ),
inputCase_() {
}

void QxUpdateUnit::doOpen() {
	this->testCase().load();
	this->testCase().backupRemoteDatabase();
	this->inputCase_ = this->testCase();
	this->inputCase_.importDatabase();
}

void QxUpdateUnit::doTest() {
	foreach( QxPoint point, this->inputCase_.getPoints() ) {
		if( this->isCancled() ) {
			break;
		}
		point->execute( this );
	}
}

void QxUpdateUnit::doClose() {
	this->inputCase_.restoreRemoteDatabase();
}

void QxUpdateUnit::doCheck( QxCheckPoint * /*point*/ ) {
	// FIXME we dont know when will the client be ready
	// so I forced wait 10 seconds
	QTimer timer;
	timer.setSingleShot( true );
	timer.setInterval( 10000 );
	QEventLoop wait;
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	timer.start();
	wait.exec();
	timer.stop();
}

void QxUpdateUnit::doInput( QxInputPoint * point ) {
	// wait a second
	QTimer timer;
	timer.setSingleShot( true );
	timer.setInterval( ( point->getWaitTime() >= 1000 ) ? point->getWaitTime() : point->getWaitTime() * 10 );
	QEventLoop wait;
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	// FIXME should stop while error occurs
	timer.start();
	wait.exec();
	timer.stop();

	// send input control
	this->sendMessage( point->getID(), TCPMessage::Input, point->getScript() );
	// record original input point
	this->testCase().writeLine( point->toString() );
}

QxPacket QxUpdateUnit::onCheck( int id, const QString & label, const QString & value ) {
	QxCheckPoint point( id, label, value, -1 );
	this->testCase().writeLine( point.toString() );
	return QxPacket( "<Success>", QVariant() );
}

QxPacket QxUpdateUnit::onInput( int id, const QString & label, const QString & script, qint64 waitTime ) {
	// NOTE nop
	return QxPacket( "<Success>", QVariant() );
}
