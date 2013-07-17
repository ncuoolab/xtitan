#include "Setting_p.hpp"

#include <algorithm>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>

#include "xTitan/Exception/InternalError.hpp"
#include "xTitan/Exception/KeyError.hpp"


/**
 * @defgroup core Core feature
 */

/**
 * @ingroup core
 * @class xtitan::Setting
 * @brief Global setting utility
 *
 * This class provides simply setting features.
 */


using xtitan::Setting;


namespace {

const char * const SETTING_FILENAME = "configuration.ini";

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
	for( auto it = this->settings.begin(); it != this->settings.end(); ++it ) {
		s.setValue( it->first, it->second );
	}

	locker.unlock();
}

/**
 * @breif Initialize singleton
 */
void Setting::initialize() {
	if( Private::self ) {
		return;
	}
	Private::self.reset( new Setting, Private::destroy );
}

/**
 * @breif Get global instance
 * @note Must call initialize first
 * @warning This function is NOT thread-safe
 * @sa initialize()
 */
Setting & Setting::instance() {
	if( !Private::self ) {
		throw InternalError( "xtitan::Setting does not initialized yet" );
	}
	return *Private::self;
}

Setting::Setting():
p_( new Private ) {
	QDir appDir = QCoreApplication::applicationDirPath();
	this->setFilePath( appDir.filePath( SETTING_FILENAME ) );
}

Setting::~Setting() {
}

void Setting::setFilePath( const QString & path ) {
	QWriteLocker locker( &this->p_->lock );
	Q_UNUSED( locker );

	this->p_->filePath = path;
	this->p_->settings.clear();
	QSettings s( this->p_->filePath, QSettings::IniFormat );
	QStringList keys( s.allKeys() );
	std::for_each( keys.begin(), keys.end(), [this, &s]( const QString & key )->void {
		this->p_->settings.insert( std::make_pair( key, s.value( key ).toString() ) );
	} );
}

void Setting::save() const {
	this->p_->save();
}

QVariant Setting::get( const QString & key, const QVariant & defaultValue /*= QVariant()*/ ) const {
	QReadLocker locker( &this->p_->lock );
	Q_UNUSED( locker );

	auto it = this->p_->settings.find( key );
	if( it == this->p_->settings.end() ) {
		if( defaultValue.isValid() ) {
			return defaultValue;
		}
		throw KeyError( QObject::tr( "item `%1\' not found" ).arg( key ) );
	}

	return it->second;
}

void Setting::set( const QString & key, const QVariant & value ) {
	QWriteLocker locker( &this->p_->lock );
	Q_UNUSED( locker );

	auto it = this->p_->settings.find( key );
	if( it != this->p_->settings.end() ) {
		it->second = value;
	} else {
		this->p_->settings.insert( std::make_pair( key, value ) );
	}
}
