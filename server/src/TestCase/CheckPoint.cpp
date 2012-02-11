#include "CheckPointPrivate.hpp"
#include "TestUnit/AbstractUnit.hpp"

using namespace xtitan::testcase;
using xtitan::testunit::AbstractUnit;

CheckPoint::Private::Private( const QString & value ):
value( value ),
checked( false ) {
}

CheckPoint::CheckPoint( int id, const QString & label, const QString & value, int line ):
AbstractPoint( id, label, line ),
p_( new Private( value ) ) {
}

const QString & CheckPoint::getValue() const {
	return this->p_->value;
}

void CheckPoint::setChecked( bool checked ) {
	this->p_->checked = checked;
}

bool CheckPoint::isChecked() const {
	return this->p_->checked;
}

QString CheckPoint::toString() const {
	return QString( "%1#<Check>#%2#%3" ).arg( this->getID() ).arg( this->getLabel() ).arg( this->p_->value );
}

void CheckPoint::execute( AbstractUnit * unit ) {
	qDebug() << this->toString();
	unit->check( this );
}
