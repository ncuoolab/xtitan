#include "Utility.hpp"

namespace {

QString escape( QString && us ) {
	return us.replace( '\\', "\\\\" ).replace( '\'', "\\\'" );
}

}

namespace xtitan {

QString toJSON( bool b ) {
	return b ? "true" : "false";
}

QString toJSON( int i ) {
	return QString::number( i );
}

QString toJSON( double d ) {
	return QString::number( d );
}

QString toJSON( const char * s ) {
	return QString( "\'%1\'" ).arg( escape( QString::fromAscii( s ) ) );
}

QString toJSON( const std::string & s ) {
	return QString( "\'%1\'" ).arg( escape( QString::fromStdString( s ) ) );
}

QString toJSON( const wchar_t * ws ) {
	return QString( "\'%1\'" ).arg( escape( QString::fromWCharArray( ws ) ) );
}

QString toJSON( const std::wstring & ws ) {
	return QString( "\'%1\'" ).arg( escape( QString::fromStdWString( ws ) ) );
}

QString toJSON( const QString & us ) {
	return QString( "\'%1\'" ).arg( escape( QString( us ) ) );
}

QVariant toVariant( bool b ) {
	return b;
}

QVariant toVariant( int i ) {
	return i;
}

QVariant toVariant( double d ) {
	return d;
}

QVariant toVariant( const char * s ) {
	return QString::fromAscii( s );
}

QVariant toVariant( const std::string & s ) {
	return QString::fromStdString( s );
}

QVariant toVariant( const wchar_t * ws ) {
	return QString::fromWCharArray( ws );
}

QVariant toVariant( const std::wstring & ws ) {
	return QString::fromStdWString( ws );
}

QVariant toVariant( const QString & us ) {
	return us;
}

}
