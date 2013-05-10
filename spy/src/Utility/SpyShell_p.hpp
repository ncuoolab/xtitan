#ifndef XTITAN_CLIENT_SPY_SPYINPUT_PRIVATE_HPP
#define XTITAN_CLIENT_SPY_SPYINPUT_PRIVATE_HPP

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "xTitan/Utility/SpyShell.hpp"


namespace xtitan {

class SpyInput::Private {
public:
	explicit Private( const char * objectName, const char * signature );

	QString objectName;
	QString methodName;
	QStringList args;
};

}

#endif
