#include "SpyCheck_p.hpp"

#include <QtCore/QMetaObject>
#include <QtCore/QFileInfo>

#include "Spy/Spy.hpp"
#include "Utility.hpp"


using xtitan::Spy;
using xtitan::detail::SpyCheck;


SpyCheck::Private::Private( const char * file, int line ):
file(),
line( line ),
id(),
args() {
	QFileInfo fi( file );
	this->file = fi.fileName();
}

SpyCheck::SpyCheck( const char * file, int line ): p_( new Private( file, line ) ) {
}

SpyCheck::~SpyCheck() {
	// commit chained input
	// NOTE this indirect invoke is necessary
	// because Spy::instance().thread() may different from QThread::currentThread()
	QMetaObject::invokeMethod( &xtitan::Spy::instance(), "encodeCheck", Q_ARG( const QString &, this->p_->file ), Q_ARG( int, this->p_->line ), Q_ARG( const QString &, this->p_->id ), Q_ARG( const QStringList &, this->p_->args ) );
}

SpyCheck & SpyCheck::id( const std::string & s ) {
	this->p_->id = QString::fromStdString( s );
	return *this;
}

SpyCheck & SpyCheck::id( const std::wstring & ws ) {
	this->p_->id = QString::fromStdWString( ws );
	return *this;
}

SpyCheck & SpyCheck::operator %( bool b ) {
	this->p_->args.append( toQString( b ) );
	return *this;
}

SpyCheck & SpyCheck::operator %( int i ) {
	this->p_->args.append( toQString( i ) );
	return *this;
}

SpyCheck & SpyCheck::operator %( double d ) {
	this->p_->args.append( toQString( d ) );
	return *this;
}

SpyCheck & SpyCheck::operator %( const char * s ) {
	this->p_->args.append( toQString( s ) );
	return *this;
}

SpyCheck & SpyCheck::operator %( const std::string & s ) {
	this->p_->args.append( toQString( s ) );
	return *this;
}

SpyCheck & SpyCheck::operator %( const wchar_t * ws ) {
	this->p_->args.append( toQString( ws ) );
	return *this;
}

SpyCheck & SpyCheck::operator %( const std::wstring & ws ) {
	this->p_->args.append( toQString( ws ) );
	return *this;
}
