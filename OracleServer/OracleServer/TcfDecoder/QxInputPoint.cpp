#include "QxInputPoint.h"
#include "TestUnit/QxTestUnit.h"
#include "TCP_Connection/TestClient.h"
#include "TestUnit/QxAbstractUnit.h"

#include <QtCore/QEventLoop>

#include <cassert>

QxInputPoint::QxInputPoint( int id, const QString & label, const QString & script, qint64 waitTime, int line ):
QxAbstractPoint( id, label, line ),
script_( script ),
timer_(),
waitTime_( waitTime ) {
	this->timer_.setSingleShot( true );
}

const QString & QxInputPoint::getScript() const {
	return this->script_;
}

qint64 QxInputPoint::getWaitTime() const {
	return this->waitTime_;
}

QString QxInputPoint::toString() const {
	return QString( "%1#<Input>#%2#%3#%4" ).arg( this->getID() ).arg( this->getLabel() ).arg( this->getScript() ).arg( this->getWaitTime() );
}

void QxInputPoint::execute( QxAbstractUnit * unit ) {
	qDebug() << this->toString();
	unit->input( this );
// 	// if in update mode
// 	if( unit->getState() & QxTestUnit::REC ) {
// 		unit->record( this->getID(), this->getLabel(), this->getScript(), this->getWaitTime() );
// 	}
// 
// 	QEventLoop wait;
// 	wait.connect( &this->timer_, SIGNAL( timeout() ), SLOT( quit() ) );
// 	if( unit->getState() & QxTestUnit::REC ) {
// 		// in update mode, emulate human speed
// 		this->timer_.start( this->waitTime_ * 10 );
// 	} else {
// 		this->timer_.start( this->waitTime_ );
// 	}
// 	wait.exec();
// 
// 	// ack to client by id
// 	TestClient * client = unit->getClient( this->getID() );
// 	if( client ) {
// 		client->AckNextInput( *this );
// 	} else {
// 		qDebug() << "Client disconnected, abort input";
// 	}
// 
// 	// FIXME: maybe we should use wait time of next input
// 	unit->wait( 0 );
}
