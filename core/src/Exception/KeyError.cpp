/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "xTitan/Exception/KeyError.hpp"


using xtitan::KeyError;


/**
 * @ingroup exceptions
 * @class xtitan::KeyError
 * @brief Hash key error class
 *
 * This exception throws when a key does not exists in a hash.
 */

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8.
 */
KeyError::KeyError( const char * message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
KeyError::KeyError( const wchar_t * message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8
 */
KeyError::KeyError( const std::string & message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
KeyError::KeyError( const std::wstring & message ): BasicError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
KeyError::KeyError( const QString & message ): BasicError( message ) {
}
