/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "BasicError_p.hpp"


using xtitan::BasicError;

/**
 * @defgroup exceptions Exceptions
 */

/**
 * @ingroup exceptions
 * @class xtitan::BasicError
 * @brief Basic error class
 * @note This class is immutable, which means copy behavior is shallow copy.
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
