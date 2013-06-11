#include "xTitan/xTitan.hpp"

#include <QtCore/QRegExp>

#include "Spy/Spy.hpp"


using xtitan::Spy;


void spyRegisterObject( QObject * object, const QString & id ) {
	if( id.isEmpty() ) {
		return;
	}

	QMetaObject::invokeMethod( &Spy::instance(), "registerObject", Q_ARG( QObject *, object ), Q_ARG( const QString &, id ) );
}

QString spyGetToken( QObject * object ) {
	return Spy::instance().getToken( object );
}

QObject * spyGetObject( const QString & token ) {
	return Spy::instance().getObject( token );
}

void spyTryTestAutomation() {
	Spy::instance().activate();
}

bool spyIsTesting() {
	return Spy::instance().isTesting();
}

QString spyMakeSafeName( const QString & objName ) {
	QString sName( objName );
	return sName.replace( QRegExp( "[^a-zA-Z0-9_]" ), "" );
}
