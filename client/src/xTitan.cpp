#include "xTitanClient/xTitan.hpp"

void xRegisterObject( QObject * object, const QString & id /* = QString */ ) {
	using namespace xtitan::spy;
	if( object->objectName().isEmpty() && id.isEmpty() ) {
		return;
	}

//	MASSERT( !object->objectName().isEmpty() || !id.isEmpty() || !"OBJECT NAME IS EMPTY" );
	object->setObjectName( object->objectName() + id );
	Spy::getInstance().registerObject( object );
}

QObject * xGetObject( const QString & token ) {
	using namespace xtitan::spy;
	return Spy::getInstance().getObject( token );
}

void xTryTestAutomation() {
	using namespace xtitan::spy;
	Spy::getInstance().activate();
}

bool xIsTesting() {
	using namespace xtitan::spy;
	return Spy::getInstance().isTesting();
}

QString xMakeSafeName( const QString & objName ) {
	QString sName( objName );
	return sName.replace( QRegExp( "[^a-zA-Z0-9_]" ), "" );
}
