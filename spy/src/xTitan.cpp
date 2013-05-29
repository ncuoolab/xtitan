#include "xTitan/xTitan.hpp"

#include "Spy/Spy.hpp"


using xtitan::Spy;


void xRegisterObject( QObject * object, const QString & id ) {
	if( id.isEmpty() ) {
		return;
	}

	QMetaObject::invokeMethod( &Spy::instance(), "registerObject", Q_ARG( QObject *, object ), Q_ARG( const QString &, id ) );
}

QString xGetToken( QObject * object ) {
	return Spy::instance().getToken( object );
}

QObject * xGetObject( const QString & token ) {
	return Spy::instance().getObject( token );
}

void xTryTestAutomation() {
	Spy::instance().activate();
}

bool xIsTesting() {
	return Spy::instance().isTesting();
}

QString xMakeSafeName( const QString & objName ) {
	QString sName( objName );
	return sName.replace( QRegExp( "[^a-zA-Z0-9_]" ), "" );
}
