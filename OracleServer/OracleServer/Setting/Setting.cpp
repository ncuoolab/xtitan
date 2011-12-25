#include "Setting/Setting.h"
#include <QtCore/QSettings>

#define SETTING_FILEPATH "configuration.ini"

Setting::Setting():
settings_(),
filePath_( SETTING_FILEPATH ) {
	QSettings s( this->filePath_, QSettings::IniFormat );
	QStringList keys( s.allKeys() );
	foreach( QString key, keys ) {
		this->settings_.insert( key, s.value( key ).toString() );
	}
}

Setting::~Setting() {
	this->save();
}

void Setting::setFilePath( const QString & path ) {
	this->filePath_ = path;
	this->settings_.clear();
	QSettings s( this->filePath_, QSettings::IniFormat );
	QStringList keys( s.allKeys() );
	foreach( QString key, keys ) {
		this->settings_.insert( key, s.value( key ).toString() );
	}
}

void Setting::save() const {
	QSettings s( this->filePath_, QSettings::IniFormat );
	for( QMap< QString, QString >::const_iterator it( this->settings_.begin() ); it != this->settings_.end(); ++it ) {
		s.setValue( it.key(), it.value() );
	}
}

QString Setting::get( const QString & key ) const {
	QMap< QString, QString >::const_iterator it = this->settings_.find( key );
	assert( it != this->settings_.end() );
	return it.value();
}

void Setting::set( const QString & key, const QString & value ) {
	QMap< QString, QString >::iterator it( this->settings_.find( key ) );
	if( it != this->settings_.end() ) {
		it.value() = value;
	} else {
		this->settings_.insert( key, value );
	}
}
