#include "QxTest.h"
#include <QtCore/QtDebug>

void xRegisterObject( QObject * object, const QString & id /* = QString */ ) {
	if( object->objectName().isEmpty() && id.isEmpty() ) {
		return;
	}

//	MASSERT( !object->objectName().isEmpty() || !id.isEmpty() || !"OBJECT NAME IS EMPTY" );
	object->setObjectName( object->objectName() + id );
	QxSpy::getInstance()->registerObject( object );
}

QObject * xGetObject( const QString & token ) {
	return QxSpy::getInstance()->getObject( token );
}

void xTryTestAutomation(){
	QxSpy::getInstance()->tryAutomation();
}

bool xIsTesting(){
	return QxSpy::getInstance()->isTesting();
}

QString xMakeSafeName(QString objName){
	objName = objName.replace(QRegExp("[^a-zA-Z0-9_]"),"");
	return objName;
}
