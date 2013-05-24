#include "SpyAsyncCheck_p.hpp"

#include <QtCore/QMetaObject>
#include <QtCore/QFileInfo>

#include "Spy/Spy.hpp"
#include "Utility.hpp"


using xtitan::Spy;
using xtitan::detail::SpyAsyncCheck;


SpyAsyncCheck::Private::Private( const char * file, int line ):
file(),
line( line ),
id(),
pre(),
args() {
	QFileInfo fi( file );
	this->file = fi.fileName();
}

SpyAsyncCheck::SpyAsyncCheck( const char * file, int line ): p_( new Private( file, line ) ) {
}

SpyAsyncCheck::~SpyAsyncCheck() {
	// commit chained input
	// NOTE this indirect invoke is necessary
	// because Spy::instance().thread() may different from QThread::currentThread()
	QMetaObject::invokeMethod( &xtitan::Spy::instance(), "encodeAsyncCheck", Q_ARG( const QString &, this->p_->file ), Q_ARG( int, this->p_->line ), Q_ARG( const QString &, this->p_->id ), Q_ARG( const QString &, this->p_->pre ), Q_ARG( const QStringList &, this->p_->args ) );
}

SpyAsyncCheck & SpyAsyncCheck::id( const std::string & s ) {
	this->p_->id = QString::fromStdString( s );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::id( const std::wstring & ws ) {
	this->p_->id = QString::fromStdWString( ws );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::pre( const std::string & s ) {
	this->p_->pre = QString::fromStdString( s );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::pre( const std::wstring & ws ) {
	this->p_->pre = QString::fromStdWString( ws );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( bool b ) {
	this->p_->args.append( toQString( b ) );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( int i ) {
	this->p_->args.append( toQString( i ) );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( double d ) {
	this->p_->args.append( toQString( d ) );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( const char * s ) {
	this->p_->args.append( toQString( s ) );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( const std::string & s ) {
	this->p_->args.append( toQString( s ) );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( const wchar_t * ws ) {
	this->p_->args.append( toQString( ws ) );
	return *this;
}

SpyAsyncCheck & SpyAsyncCheck::operator %( const std::wstring & ws ) {
	this->p_->args.append( toQString( ws ) );
	return *this;
}
