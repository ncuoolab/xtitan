/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SPY_SPYASYNCCHECK_HPP_
#define XTITAN_SPY_SPYASYNCCHECK_HPP_

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "xTitan/Utility/SpyAsyncCheck.hpp"


namespace xtitan {
namespace detail {

class SpyAsyncCheck::Private {
public:
	Private( const char * file, int line );

	QString file;
	int line;
	QString id;
	QString pre;
	QVariantList args;
};

}
}

#endif
