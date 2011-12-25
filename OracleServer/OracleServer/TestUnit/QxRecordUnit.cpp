#include "QxRecordUnit.h"
#include "TcfDecoder/QxCheckPoint.h"
#include "TcfDecoder/QxInputPoint.h"
#include "TcfDecoder/QxTestCase.h"

#include <QtCore/QDateTime>
#include <QtCore/QEventLoop>

QxRecordUnit::QxRecordUnit( QObject * parent ):
QxAbstractUnit( parent ),
lastTick_( 0 ),
ready_( false ) {
}

void QxRecordUnit::doOpen() {
	this->testCase().backupRemoteDatabase();
}

void QxRecordUnit::doTest() {
	this->lastTick_ = QDateTime::currentMSecsSinceEpoch();
	this->ready_ = true;

	QEventLoop wait;
	wait.connect( this, SIGNAL( canceled() ), SLOT( quit() ) );
	wait.exec();
}

void QxRecordUnit::doClose() {
	this->ready_ = false;
	this->testCase().restoreRemoteDatabase();
}

void QxRecordUnit::doCheck( QxCheckPoint * /*point*/ ) {
	// no need to send check command
}

void QxRecordUnit::doInput( QxInputPoint * /*point*/ ) {
	// no need to send input command
}

QxPacket QxRecordUnit::onCheck( int id, const QString & label, const QString & value ) {
	if( !this->ready_ ) {
		return QxPacket( "<Success>", QVariant() );
	}

	QxCheckPoint point( id, label, value, -1 );
	this->testCase().writeLine( point.toString() );
	return QxPacket( "<Success>", QVariant() );
}

QxPacket QxRecordUnit::onInput( int id, const QString & label, const QString & script, qint64 timestamp ) {
	if( !this->ready_ ) {
		return QxPacket( "<Success>", QVariant() );
	}

	qint64 waitTime = ( timestamp < this->lastTick_ ) ? 0 : ( timestamp - this->lastTick_ );
	this->lastTick_ = timestamp + waitTime;
	QxInputPoint input( id, label, script, waitTime, -1 );
	this->testCase().writeLine( input.toString() );
	return QxPacket( "<Success>", QVariant() );
}
