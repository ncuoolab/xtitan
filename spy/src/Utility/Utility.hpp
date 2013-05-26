#ifndef XTITAN_SPY_UTILITY_HPP
#define XTITAN_SPY_UTILITY_HPP

#include <QtCore/QString>

namespace xtitan {

QString toQString( bool b );
QString toQString( int i );
QString toQString( double d );
QString toQString( const char * s );
QString toQString( const std::string & s );
QString toQString( const wchar_t * ws );
QString toQString( const std::wstring & ws );
QString toQString( const QString & us );

}

#endif
