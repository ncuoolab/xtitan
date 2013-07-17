/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "xTitan/Exception/InternalError.hpp"


using xtitan::InternalError;


/**
 * @ingroup exceptions
 * @class xtitan::InternalError
 * @brief Internal error class
 *
 * This exception throws when something really bad happened.
 */

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8.
 */
InternalError::InternalError( const char * message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
InternalError::InternalError( const wchar_t * message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8
 */
InternalError::InternalError( const std::string & message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
InternalError::InternalError( const std::wstring & message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
InternalError::InternalError( const QString & message ): BasicError( message ) {
}
