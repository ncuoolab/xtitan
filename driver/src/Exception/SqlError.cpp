/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "SqlError.hpp"


using xtitan::SqlError;


/**
 * @class krapture::error::SqlError
 * @brief SQL error class
 *
 * This exception throws when a SQL query has failed.
 */

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8.
 */
SqlError::SqlError( const char * message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
SqlError::SqlError( const wchar_t * message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 *
 * @p message should encode to UTF-8
 */
SqlError::SqlError( const std::string & message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
SqlError::SqlError( const std::wstring & message ): IOError( message ) {
}

/**
 * @brief Create exception with @p message
 * @param [in] message error message
 */
SqlError::SqlError( const QString & message ): IOError( message ) {
}
