#include "AbstractPointPrivate.hpp"

using namespace xtitan::testcase;

AbstractPoint::Private::Private( int id, const QString & label, int line ):
id( id ),
label( label ),
line( line ) {
}

AbstractPoint::AbstractPoint( int id, const QString & label, int line ):
p_( new Private( id, label, line ) ) {
}

AbstractPoint::~AbstractPoint() {
}

int AbstractPoint::getID() const {
	return this->p_->id;
}

const QString & AbstractPoint::getLabel() const {
	return this->p_->label;
}

int AbstractPoint::getLine() const {
	return this->p_->line;
}
