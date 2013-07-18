/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_NETWORKERROR_HPP
#define XTITAN_NETWORKERROR_HPP

#include "xTitan/Exception/IOError.hpp"


namespace xtitan {

class XTITAN_CORE_DLL NetworkError: public IOError {
public:
	explicit NetworkError( const char * message );
	explicit NetworkError( const wchar_t * message );
	explicit NetworkError( const std::string & message );
	explicit NetworkError( const std::wstring & message );
	explicit NetworkError( const QString & message );
};

}

#endif
