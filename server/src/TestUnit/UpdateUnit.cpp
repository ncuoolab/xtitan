#include "UpdateUnitPrivate.hpp"
#include "TestCase/CheckPoint.hpp"
#include "TestCase/InputPoint.hpp"
#include "TestCase/SocketPoint.hpp"
#include "TestCase/TestCase.hpp"
#include "Network/TCPMessage.hpp"

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

using namespace xtitan::testunit;
using xtitan::testcase::Point;
using xtitan::testcase::CheckPoint;
using xtitan::testcase::InputPoint;
using xtitan::testcase::SocketPoint;
using xtitan::network::SimpleSocket;

UpdateUnit::Private::Private():
inputCase() {
}

UpdateUnit::UpdateUnit( QObject * parent ):
AbstractUnit( parent ),
p_( new Private ) {
}

void UpdateUnit::doOpen() {
	this->getTestCase()->load();
	this->getTestCase()->backupRemoteDatabase();
	this->p_->inputCase = this->getTestCase();
	this->p_->inputCase->importDatabase();
}

void UpdateUnit::doTest() {
	foreach( Point point, this->p_->inputCase->getPoints() ) {
		if( this->isCancled() ) {
			break;
		}
		point->execute( this );
	}
}

void UpdateUnit::doClose() {
	this->p_->inputCase->restoreRemoteDatabase();
}

void UpdateUnit::doCheck( CheckPoint * /*point*/ ) {
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

void UpdateUnit::doInput( InputPoint * point ) {
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
	this->getTestCase()->writeLine( point->toString() );
}

void UpdateUnit::doSocket( SocketPoint * /*point*/ ) {
	// NOT nop
}

SimpleSocket::Packet UpdateUnit::onCheck( int id, const QString & label, const QString & value ) {
	CheckPoint point( id, label, value, -1 );
	this->getTestCase()->writeLine( point.toString() );
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

SimpleSocket::Packet UpdateUnit::onInput( int /*id*/, const QString & /*label*/, const QString & /*script*/, qint64 /*waitTime*/ ) {
	// NOTE nop
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

SimpleSocket::Packet UpdateUnit::onSocket( int id, const QString & message ) {
	SocketPoint point( id, message, -1 );
	this->getTestCase()->writeLine( point.toString() );
	return SimpleSocket::Packet( "<Success>", QVariant() );
}
