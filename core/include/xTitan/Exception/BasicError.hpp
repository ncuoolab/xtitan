/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_BASICERROR_HPP
#define XTITAN_BASICERROR_HPP

#include <exception>
#include <memory>

#include <QtCore/QString>

#include "xTitan/Utility/CoreConfig.hpp"


namespace xtitan {

class XTITAN_CORE_DLL BasicError: public std::exception {
public:
	explicit BasicError( const char * message );
	explicit BasicError( const wchar_t * message );
	explicit BasicError( const std::string & message );
	explicit BasicError( const std::wstring & message );
	explicit BasicError( const QString & message );
	virtual ~BasicError() throw();

	const QString & getMessage() const;
	virtual const char * what() const throw();

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif
