#ifndef XTITAN_SPY_SPYINPUT_HPP_
#define XTITAN_SPY_SPYINPUT_HPP_

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "xTitan/Utility/SpyInput.hpp"


namespace xtitan {
namespace detail {

class SpyInput::Private {
public:
	Private( const char * objectName, const char * signature );

	QString objectName;
	QString methodName;
	QVariantList args;
};

}
}

#endif
