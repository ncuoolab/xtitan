#ifndef XTITAN_CLIENT_XTITAN_HPP
#define XTITAN_CLIENT_XTITAN_HPP

#include <QtCore/QObject>

#include "xTitan/Utility/SpyShell.hpp"


#define spyInput xtitan::SpyInput(this->objectName().toUtf8(),__FUNCTION__)
#define spyCheck(value) xtitan::encodeCheck(__FUNCTION__,"spyCheck",value)
#define spyAsyncCheck(value) xtitan::encodeCheck(__FUNCTION__,"spyAsyncCheck",value);

XTITAN_SPY_DLL void xRegisterObject( QObject * object, const QString & id = QString() );

XTITAN_SPY_DLL QString xGetToken( QObject * object );
XTITAN_SPY_DLL QObject * xGetObject( const QString & token );

XTITAN_SPY_DLL void xTryTestAutomation();

XTITAN_SPY_DLL bool xIsTesting();

XTITAN_SPY_DLL QString xMakeSafeName( const QString & objName );

#endif
