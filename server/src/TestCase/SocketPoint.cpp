#include "SocketPointPrivate.hpp"
#include "TestUnit/AbstractUnit.hpp"

using namespace xtitan::testcase;
using xtitan::testunit::AbstractUnit;

SocketPoint::Private::Private( const QString & message ):
message( message ) {
}

SocketPoint::SocketPoint( int id, const QString & message, int line ):
AbstractPoint( id, QString(), line ),
p_( new Private( message ) ) {
}

const QString & SocketPoint::getMessage() const {
	return this->p_->message;
}

QString SocketPoint::toString() const {
	return QString( "%1#<Socket>#%2" ).arg( this->getID() ).arg( this->p_->message );
}

void SocketPoint::execute( AbstractUnit * unit ) {
	unit->socket( this );
}
