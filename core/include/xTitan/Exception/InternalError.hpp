/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_INTERNALERROR_HPP
#define XTITAN_INTERNALERROR_HPP

#include "xTitan/Exception/BasicError.hpp"


namespace xtitan {

class XTITAN_CORE_DLL InternalError: public BasicError {
public:
	explicit InternalError( const char * message );
	explicit InternalError( const wchar_t * message );
	explicit InternalError( const std::string & message );
	explicit InternalError( const std::wstring & message );
	explicit InternalError( const QString & message );
};

}

#endif
