#ifndef XTITAN_CLIENT_XTITAN_HPP
#define XTITAN_CLIENT_XTITAN_HPP

#include "xTitan/SpyModule/Controller/QxSpy.hpp"

//#define xCheck(id,x)					QxSpy::getInstance()->encodeCheck(QString::fromUtf8(__FUNCTION__),id, x );
//#define xCheck(x)						QxSpy::getInstance()->encodeCheck(QString::fromUtf8(__FUNCTION__),QString::fromUtf8(__FUNCTION__),x);
#define xCheck(...)						QxSpy::getInstance()->encodeCheck(QString::fromUtf8(__FUNCTION__),__VA_ARGS__);
#define xInput(...)						QxSpy::getInstance()->recordInput(this->objectName(),__FUNCTION__,__VA_ARGS__);
#define xCustomInput(object,method,...)	QxSpy::getInstance()->recordInput(object,__FUNCTION__,method,__VA_ARGS__);

void xRegisterObject( QObject * object, const QString & id = QString() );

QString xGetToken( QObject * object );
QObject * xGetObject( const QString & token );

void xTryTestAutomation();

bool xIsTesting();

QString xMakeSafeName(QString objName);

#endif
