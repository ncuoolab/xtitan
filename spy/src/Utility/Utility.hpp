#ifndef XTITAN_SPY_UTILITY_HPP
#define XTITAN_SPY_UTILITY_HPP

#include <QtCore/QVariant>

namespace xtitan {

QString toJSON( bool b );
QString toJSON( int i );
QString toJSON( double d );
QString toJSON( const char * s );
QString toJSON( const std::string & s );
QString toJSON( const wchar_t * ws );
QString toJSON( const std::wstring & ws );
QString toJSON( const QString & us );

QVariant toVariant( bool b );
QVariant toVariant( int i );
QVariant toVariant( double d );
QVariant toVariant( const char * s );
QVariant toVariant( const std::string & s );
QVariant toVariant( const wchar_t * ws );
QVariant toVariant( const std::wstring & ws );
QVariant toVariant( const QString & us );

}

#endif
