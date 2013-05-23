#ifndef XTITAN_CLIENT_XTITAN_HPP
#define XTITAN_CLIENT_XTITAN_HPP

#include <QtCore/QObject>

#include "xTitan/Utility/SpyInput.hpp"
#include "xTitan/Utility/SpyCheck.hpp"
#include "xTitan/Utility/SpyAsyncCheck.hpp"


#define spyInput xtitan::detail::SpyInput(this->objectName().toUtf8(),__FUNCTION__)
#define spyCheck xtitan::detail::SpyCheck(__FILE__,__LINE__)
#define spyAsyncCheck xtitan::detail::SpyAsyncCheck(__FILE__,__LINE__)

XTITAN_SPY_DLL void xRegisterObject( QObject * object, const QString & id = QString() );

XTITAN_SPY_DLL QString xGetToken( QObject * object );
XTITAN_SPY_DLL QObject * xGetObject( const QString & token );

XTITAN_SPY_DLL void xTryTestAutomation();

XTITAN_SPY_DLL bool xIsTesting();

XTITAN_SPY_DLL QString xMakeSafeName( const QString & objName );

#endif
