#ifndef XTITAN_CLIENT_XTITAN_HPP
#define XTITAN_CLIENT_XTITAN_HPP

#include <QtCore/QObject>

#include "xTitan/Utility/SpyInput.hpp"
#include "xTitan/Utility/SpyCheck.hpp"
#include "xTitan/Utility/SpyAsyncCheck.hpp"


#define spyInput xtitan::detail::SpyInput(xGetToken(this).toUtf8(),__FUNCTION__)
#define spyCheck xtitan::detail::SpyCheck(__FILE__,__LINE__)
#define spyAsyncCheck xtitan::detail::SpyAsyncCheck(__FILE__,__LINE__)

XTITAN_SPY_DLL void spyRegisterObject( QObject * object, const QString & id );

XTITAN_SPY_DLL QString spyGetToken( QObject * object );
XTITAN_SPY_DLL QObject * spyGetObject( const QString & token );

XTITAN_SPY_DLL void spyTryTestAutomation();

XTITAN_SPY_DLL bool spyIsTesting();

XTITAN_SPY_DLL QString spyMakeSafeName( const QString & objName );

#endif
