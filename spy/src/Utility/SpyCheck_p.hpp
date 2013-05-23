#ifndef XTITAN_SPY_SPYCHECK_HPP_
#define XTITAN_SPY_SPYCHECK_HPP_

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "xTitan/Utility/SpyCheck.hpp"


namespace xtitan {
namespace detail {

class SpyCheck::Private {
public:
	Private( const char * file, int line );

	QString file;
	int line;
	QString id;
	QStringList args;
};

}
}

#endif
