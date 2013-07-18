/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_IOERROR_HPP
#define XTITAN_IOERROR_HPP

#include "xTitan/Exception/BasicError.hpp"


namespace xtitan {

class XTITAN_CORE_DLL IOError: public BasicError {
public:
	explicit IOError( const char * message );
	explicit IOError( const wchar_t * message );
	explicit IOError( const std::string & message );
	explicit IOError( const std::wstring & message );
	explicit IOError( const QString & message );
};

}

#endif
