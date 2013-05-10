/*
Copyright (c) 2011 Wei Cheng Pan <legnaleurc@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef XTITAN_ERROR_BASICERROR_HPP
#define XTITAN_ERROR_BASICERROR_HPP

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
