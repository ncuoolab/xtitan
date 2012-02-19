#include "SettingPrivate.hpp"

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include <algorithm>
#include <cassert>

using namespace xtitan::utilities;

namespace {

	const char * const SETTING_FILEPATH = "configuration.ini";

}

std::shared_ptr< Setting > Setting::Private::self;

void Setting::Private::destroy( Setting * self ) {
	delete self;
}

Setting::Private::Private():
QObject(),
lock(),
settings(),
filePath() {
	this->connect( QCoreApplication::instance(), SIGNAL( aboutToQuit() ), SLOT( save() ) );
}

void Setting::Private::save() {
	QReadLocker locker( &this->lock );

	QSettings s( this->filePath, QSettings::IniFormat );
	std::for_each( this->settings.begin(), this->settings.end(), [&s]( const std::pair< QString, QVariant > & p )->void {
		s.setValue( p.first, p.second );
	} );

	locker.unlock();
}

/**
 * @warning This function is NOT thread-safe
 */
Setting & Setting::getInstance() {
	if( !Private::self ) {
		Private::self.reset( new Setting, Private::destroy );
	}
	return *Private::self;
}

Setting::Setting():
p_( new Private ) {
	QDir appDir( QCoreApplication::applicationDirPath() );
	this->setFilePath( appDir.filePath( SETTING_FILEPATH ) );
}

Setting::~Setting() {
}

void Setting::setFilePath( const QString & path ) {
	QWriteLocker locker( &this->p_->lock );

	this->p_->filePath = path;
	this->p_->settings.clear();
	QSettings s( this->p_->filePath, QSettings::IniFormat );
	QStringList keys( s.allKeys() );
	std::for_each( keys.begin(), keys.end(), [this, &s]( const QString & key )->void {
		this->p_->settings.insert( std::make_pair( key, s.value( key ).toString() ) );
	} );

	locker.unlock();
}

void Setting::save() const {
	this->p_->save();
}

QVariant Setting::get( const QString & key ) const {
	QReadLocker locker( &this->p_->lock );

	std::map< QString, QVariant >::const_iterator it = this->p_->settings.find( key );
	assert( it != this->p_->settings.end() );

	locker.unlock();
	return it->second;
}

void Setting::set( const QString & key, const QVariant & value ) {
	QWriteLocker locker( &this->p_->lock );

	std::map< QString, QVariant >::iterator it( this->p_->settings.find( key ) );
	if( it != this->p_->settings.end() ) {
		it->second = value;
	} else {
		this->p_->settings.insert( std::make_pair( key, value ) );
	}

	locker.unlock();
}
