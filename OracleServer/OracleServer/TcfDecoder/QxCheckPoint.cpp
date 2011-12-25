#include "QxCheckPoint.h"
#include "TestUnit/QxTestUnit.h"
#include "TestUnit/QxAbstractUnit.h"

QxCheckPoint::QxCheckPoint( int id, const QString & label, const QString & value, int line ):
QxAbstractPoint( id, label, line ),
value_( value ),
checked_( false ) {
}

const QString & QxCheckPoint::getValue() const {
	return this->value_;
}

void QxCheckPoint::setChecked( bool checked ) {
	this->checked_ = checked;
}

bool QxCheckPoint::isChecked() const {
	return this->checked_;
}

QString QxCheckPoint::toString() const {
	return QString( "%1#<Check>#%2#%3" ).arg( this->getID() ).arg( this->getLabel() ).arg( this->value_ );
}

void QxCheckPoint::execute( QxAbstractUnit * unit ) {
	qDebug() << this->toString();
	unit->check( this );
// 	if( this->checked_ || unit->getState() & QxTestUnit::REC ) {
// 		unit->wait( 0 );
// 	} else {
// 		unit->wait( this );
// 	}
}
