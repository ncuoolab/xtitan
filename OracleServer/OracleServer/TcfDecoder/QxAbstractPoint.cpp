#include "QxAbstractPoint.h"

QxAbstractPoint::QxAbstractPoint( int id, const QString & label, int line ):
id_( id ),
label_( label ),
line_( line ) {
}

int QxAbstractPoint::getID() const {
	return this->id_;
}

const QString & QxAbstractPoint::getLabel() const {
	return this->label_;
}

int QxAbstractPoint::getLine() const {
	return this->line_;
}
