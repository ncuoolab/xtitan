#include "Utility.hpp"

namespace xtitan {

QString toQString( bool b ) {
	return b ? "true" : "false";
}

QString toQString( int i ) {
	return QString::number( i );
}

QString toQString( double d ) {
	return QString::number( d );
}

QString toQString( const char * s ) {
	return QString( "\'%1\'" ).arg( QString::fromAscii( s ) );
}

QString toQString( const std::string & s ) {
	return QString( "\'%1\'" ).arg( QString::fromStdString( s ) );
}

QString toQString( const wchar_t * ws ) {
	return QString( "\'%1\'" ).arg( QString::fromWCharArray( ws ) );
}

QString toQString( const std::wstring & ws ) {
	return QString( "\'%1\'" ).arg( QString::fromStdWString( ws ) );
}

}
