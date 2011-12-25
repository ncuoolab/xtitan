#ifndef  _SETTING_H_
#define _SETTING_H_

#include <QString>
#include <QMap>
#include <QStringList>
#include <QFile>
#include <cassert>

#include <loki/Singleton.h>

class Setting {
public:
	QString get( const QString & key ) const;
	void set( const QString & key, const QString & value );

	void save() const;

	void setFilePath( const QString & path );

private:
	friend Loki::CreateUsingNew< Setting >;
	Setting();
	~Setting();
	QMap< QString, QString > settings_;
	QString filePath_;
};

typedef Loki::SingletonHolder< Setting > QxSetting;

#endif
