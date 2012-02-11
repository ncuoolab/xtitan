#include "InputPointPrivate.hpp"
#include "TestUnit/AbstractUnit.hpp"

#include <QtCore/QEventLoop>

#include <cassert>

using namespace xtitan::testcase;
using xtitan::testunit::AbstractUnit;

InputPoint::Private::Private( const QString & script, qint64 waitTime ):
script( script ),
timer(),
waitTime( waitTime ) {
	this->timer.setSingleShot( true );
}

InputPoint::InputPoint( int id, const QString & label, const QString & script, qint64 waitTime, int line ):
AbstractPoint( id, label, line ),
p_( new Private( script, waitTime ) ) {
}

const QString & InputPoint::getScript() const {
	return this->p_->script;
}

qint64 InputPoint::getWaitTime() const {
	return this->p_->waitTime;
}

QString InputPoint::toString() const {
	return QString( "%1#<Input>#%2#%3#%4" ).arg( this->getID() ).arg( this->getLabel() ).arg( this->getScript() ).arg( this->getWaitTime() );
}

void InputPoint::execute( AbstractUnit * unit ) {
	qDebug() << this->toString();
	unit->input( this );
}
