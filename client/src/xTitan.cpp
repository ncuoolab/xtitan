#include "xTitanClient/xTitan.hpp"

using namespace xtitan::spy;

void xRegisterObject( QObject * object, const QString & id /* = QString */ ) {
	if( object->objectName().isEmpty() && id.isEmpty() ) {
		return;
	}

//	MASSERT( !object->objectName().isEmpty() || !id.isEmpty() || !"OBJECT NAME IS EMPTY" );
	object->setObjectName( object->objectName() + id );
	Spy::getInstance().registerObject( object );
}

QObject * xGetObject( const QString & token ) {
	return Spy::getInstance().getObject( token );
}

void xTryTestAutomation(){
	Spy::getInstance().activate();
}

bool xIsTesting(){
	return Spy::getInstance().isTesting();
}

QString xMakeSafeName(QString objName){
	objName = objName.replace( QRegExp( "[^a-zA-Z0-9_]" ), "" );
	return objName;
}
