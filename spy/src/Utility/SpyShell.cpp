#include "SpyShell_p.hpp"

#include <QtCore/QMetaObject>

#include "Spy/Spy.hpp"


using xtitan::Spy;
using xtitan::SpyInput;


SpyInput::Private::Private( const char * objectName, const char * signature ):
objectName( QString::fromUtf8( objectName ) ),
methodName( QString::fromUtf8( signature ).section( "::", -1 ) ),
args() {
}

SpyInput::SpyInput( const char * objectName, const char * signature ): p_( new Private( objectName, signature ) ) {
}

SpyInput::~SpyInput() {
	// commit chained input
	// NOTE this indirect invoke is necessary
	// because Spy::instance().thread() may different from QThread::currentThread()
	QMetaObject::invokeMethod( &Spy::instance(), "recordInput", Q_ARG( const QString &, this->p_->objectName ), Q_ARG( const QString &, this->p_->methodName ), Q_ARG( const QStringList &, this->p_->args ) );
}

SpyInput & xtitan::operator %( SpyInput & si, bool b ) {
	si.p_->args.append( b ? "true" : "false" );
	return si;
}

SpyInput & xtitan::operator %( SpyInput & si, int i ) {
	si.p_->args.append( QString::number( i ) );
	return si;
}

SpyInput & xtitan::operator %( SpyInput & si, double d ) {
	si.p_->args.append( QString::number( d ) );
	return si;
}

SpyInput & xtitan::operator %( SpyInput & si, const std::string & s ) {
	si.p_->args.append( QString( "\'%1\'" ).arg( QString::fromStdString( s ) ) );
	return si;
}

SpyInput & xtitan::operator %( SpyInput & si, const std::wstring & ws ) {
	si.p_->args.append( QString( "\'%1\'" ).arg( QString::fromStdWString( ws ) ) );
	return si;
}

void xtitan::encodeCheck( const std::string & signature, bool value ){
	QMetaObject::invokeMethod( &Spy::instance(), "encodeCheck", Q_ARG( const QString &, QString::fromStdString( signature ) ), Q_ARG( const QString &, value ? "true" : "false" ) );
}

void xtitan::encodeCheck( const std::string & signature, int value ){
	QMetaObject::invokeMethod( &Spy::instance(), "encodeCheck", Q_ARG( const QString &, QString::fromStdString( signature ) ), Q_ARG( const QString &, QString::number( value ) ) );
}

void xtitan::encodeCheck( const std::string & signature, double value ){
	QMetaObject::invokeMethod( &Spy::instance(), "encodeCheck", Q_ARG( const QString &, QString::fromStdString( signature ) ), Q_ARG( const QString &, QString::number( value ) ) );
}

void xtitan::encodeCheck( const std::string & signature, const std::string & value ) {
	QMetaObject::invokeMethod( &Spy::instance(), "encodeCheck", Q_ARG( const QString &, QString::fromStdString( signature ) ), Q_ARG( const QString &, QString::fromStdString( value ) ) );
}

void xtitan::encodeCheck( const std::string & signature, const std::wstring & value ) {
	QMetaObject::invokeMethod( &Spy::instance(), "encodeCheck", Q_ARG( const QString &, QString::fromStdString( signature ) ), Q_ARG( const QString &, QString::fromStdWString( value ) ) );
}