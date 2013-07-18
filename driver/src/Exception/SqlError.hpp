/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SQLERROR_HPP
#define XTITAN_SQLERROR_HPP

#include "xTitan/Exception/IOError.hpp"

namespace xtitan {

class SqlError : public IOError {
public:
	explicit SqlError( const char * message );
	explicit SqlError( const wchar_t * message );
	explicit SqlError( const std::string & message );
	explicit SqlError( const std::wstring & message );
	explicit SqlError( const QString & message );
};

}

#endif
