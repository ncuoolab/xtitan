#include "SpyInput_p.hpp"

#include <QtCore/QMetaObject>

#include "Spy/Spy.hpp"
#include "Utility.hpp"


using xtitan::Spy;
using xtitan::detail::SpyInput;


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
	QMetaObject::invokeMethod( &Spy::instance(), "input", Q_ARG( const QString &, this->p_->objectName ), Q_ARG( const QString &, this->p_->methodName ), Q_ARG( const QVariantList &, this->p_->args ) );
}

SpyInput & SpyInput::operator %( bool b ) {
	this->p_->args.append( toVariant( b ) );
	return *this;
}

SpyInput & SpyInput::operator %( int i ) {
	this->p_->args.append( toVariant( i ) );
	return *this;
}

SpyInput & SpyInput::operator %( double d ) {
	this->p_->args.append( toVariant( d ) );
	return *this;
}

SpyInput & SpyInput::operator %( const char * s ) {
	this->p_->args.append( toVariant( s ) );
	return *this;
}

SpyInput & SpyInput::operator %( const std::string & s ) {
	this->p_->args.append( toVariant( s ) );
	return *this;
}

SpyInput & SpyInput::operator %( const wchar_t * ws ) {
	this->p_->args.append( toVariant( ws ) );
	return *this;
}

SpyInput & SpyInput::operator %( const std::wstring & ws ) {
	this->p_->args.append( toVariant( ws ) );
	return *this;
}
