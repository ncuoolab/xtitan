#include "RecordUnitPrivate.hpp"
#include "TestCase/CheckPoint.hpp"
#include "TestCase/InputPoint.hpp"
#include "TestCase/SocketPoint.hpp"
#include "TestCase/TestCase.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QEventLoop>

using namespace xtitan::testunit;
using xtitan::testcase::CheckPoint;
using xtitan::testcase::InputPoint;
using xtitan::testcase::SocketPoint;
using xtitan::network::SimpleSocket;

RecordUnit::Private::Private():
lastTick( 0 ),
ready( false ) {
}

RecordUnit::RecordUnit( QObject * parent ):
AbstractUnit( parent ),
p_( new Private ) {
}

void RecordUnit::doOpen() {
	this->getTestCase()->backupRemoteDatabase();
}

void RecordUnit::doTest() {
	this->p_->lastTick = QDateTime::currentMSecsSinceEpoch();
	this->p_->ready = true;

	QEventLoop wait;
	wait.connect( this, SIGNAL( canceled() ), SLOT( quit() ) );
	wait.exec();
}

void RecordUnit::doClose() {
	this->p_->ready = false;
	this->getTestCase()->restoreRemoteDatabase();
}

void RecordUnit::doCheck( CheckPoint * /*point*/ ) {
	// no need to send check command
}

void RecordUnit::doInput( InputPoint * /*point*/ ) {
	// no need to send input command
}

void RecordUnit::doSocket( SocketPoint * /*point*/ ) {
	// no need to send socket command
}

SimpleSocket::Packet RecordUnit::onCheck( int id, const QString & label, const QString & value ) {
	if( !this->p_->ready ) {
		return SimpleSocket::Packet( "<Success>", QVariant() );
	}

	CheckPoint point( id, label, value, -1 );
	this->getTestCase()->writeLine( point.toString() );
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

SimpleSocket::Packet RecordUnit::onInput( int id, const QString & label, const QString & script, qint64 timestamp ) {
	if( !this->p_->ready ) {
		return SimpleSocket::Packet( "<Success>", QVariant() );
	}

	qint64 waitTime = ( timestamp < this->p_->lastTick ) ? 0 : ( timestamp - this->p_->lastTick );
	this->p_->lastTick = timestamp + waitTime;
	InputPoint input( id, label, script, waitTime, -1 );
	this->getTestCase()->writeLine( input.toString() );
	return SimpleSocket::Packet( "<Success>", QVariant() );
}

SimpleSocket::Packet RecordUnit::onSocket( int id, const QString & message ) {
	if( !this->p_->ready ) {
		return SimpleSocket::Packet( "<Success>", QVariant() );
	}

	SocketPoint point( id, message, -1 );
	this->getTestCase()->writeLine( point.toString() );
	return SimpleSocket::Packet( "<Success>", QVariant() );
}
