#ifndef XTITAN_CLIENT_XTITAN_HPP
#define XTITAN_CLIENT_XTITAN_HPP

#include "xTitanClient/Spy/Spy.hpp"

#define xCheck(...)						xtitan::spy::encodeCheck(QString::fromUtf8(__FUNCTION__),__VA_ARGS__);
#define xInput(...)						xtitan::spy::recordInput(this->objectName(),__FUNCTION__,__VA_ARGS__);
#define xCustomInput(object,method,...)	xtitan::spy::recordInput(object,__FUNCTION__,method,__VA_ARGS__);
XTITAN_CLIENT_DLL void xRawCommand( const QString & tag, const QVariant & data );

XTITAN_CLIENT_DLL void xRegisterObject( QObject * object, const QString & id = QString() );

XTITAN_CLIENT_DLL QString xGetToken( QObject * object );
XTITAN_CLIENT_DLL QObject * xGetObject( const QString & token );

XTITAN_CLIENT_DLL void xTryTestAutomation();

XTITAN_CLIENT_DLL bool xIsTesting();

XTITAN_CLIENT_DLL QString xMakeSafeName( const QString & objName );

#endif
