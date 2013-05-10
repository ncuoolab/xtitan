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
#include "xTitan/Exception/NetworkError.hpp"


using xtitan::NetworkError;


/**
 * @class krapture::error::IOError
 * @brief I/O error class
 *
 * This exception throws when an input or output operation has failed.
 * e.g. File not found, write error ... etc.
 */

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8.
 */
NetworkError::NetworkError( const char * message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
NetworkError::NetworkError( const wchar_t * message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8
 */
NetworkError::NetworkError( const std::string & message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
NetworkError::NetworkError( const std::wstring & message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
NetworkError::NetworkError( const QString & message ): IOError( message ) {
}
