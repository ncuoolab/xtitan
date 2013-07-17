/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "xTitan/Exception/IOError.hpp"


using xtitan::IOError;


/**
 * @ingroup exceptions
 * @class xtitan::IOError
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
IOError::IOError( const char * message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
IOError::IOError( const wchar_t * message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8
 */
IOError::IOError( const std::string & message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
IOError::IOError( const std::wstring & message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
IOError::IOError( const QString & message ): BasicError( message ) {
}
