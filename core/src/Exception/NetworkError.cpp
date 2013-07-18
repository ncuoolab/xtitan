/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
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
