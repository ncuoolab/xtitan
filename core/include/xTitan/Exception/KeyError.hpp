/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_KEYERROR_HPP
#define XTITAN_KEYERROR_HPP

#include "xTitan/Exception/BasicError.hpp"


namespace xtitan {

class XTITAN_CORE_DLL KeyError: public BasicError {
public:
	explicit KeyError( const char * message );
	explicit KeyError( const wchar_t * message );
	explicit KeyError( const std::string & message );
	explicit KeyError( const std::wstring & message );
	explicit KeyError( const QString & message );
};

}

#endif
