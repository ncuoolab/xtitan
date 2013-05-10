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
#include "BasicError_p.hpp"


using xtitan::BasicError;


/**
 * @class krapture::error::BasicError
 * @brief Basic error class
 * @note This class is an immutable class, the copy behavior is shallow copy.
 */

BasicError::Private::Private( const QString & m ): message( m ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8.
 */
BasicError::BasicError( const char * message ): std::exception(), p_( new Private( QString::fromUtf8( message ) ) ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
BasicError::BasicError( const wchar_t * message ): std::exception(), p_( new Private( QString::fromWCharArray( message ) ) ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8
 */
BasicError::BasicError( const std::string & message ): std::exception(), p_( new Private( QString::fromStdString( message ) ) ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
BasicError::BasicError( const std::wstring & message ): std::exception(), p_( new Private( QString::fromStdWString( message ) ) ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
BasicError::BasicError( const QString & message ): std::exception(), p_( new Private( message ) ) {
}

/**
 * @brief Destructor
 */
BasicError::~BasicError() throw() {
}

/**
 * @brief Get error message
 * @return error message
 * @sa what()
 *
 * This version returns Unicode error message.
 */
const QString & BasicError::getMessage() const {
	return this->p_->message;
}

/**
 * @brief Get error message
 * @return error message
 * @sa getMessage()
 *
 * This version is provided for compability. The error message is encoded to
 * system locale.
 */
const char * BasicError::what() const throw() {
	return this->p_->message.toLocal8Bit();
}
