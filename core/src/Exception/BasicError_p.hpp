/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_BASICERROR_HPP_
#define XTITAN_BASICERROR_HPP_

#include "xTitan/Exception/BasicError.hpp"


namespace xtitan {

class BasicError::Private {
public:
	explicit Private( const QString & );

	QString message;
};

}

#endif
