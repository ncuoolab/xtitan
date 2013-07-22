/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "xTitan/Exception/NetworkError.hpp"


using xtitan::NetworkError;


/**
 * @ingroup exceptions
 * @class xtitan::NetworkError
 * @brief Network error class
 *
 * This exception throws when a network error occurs.
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
