#ifndef XTITAN_UTILITIES_SETTING_PRIVATE_HPP
#define XTITAN_UTILITIES_SETTING_PRIVATE_HPP

#include "xTitan/Utility/Setting.hpp"

#include <map>

#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>

namespace xtitan {

class Setting::Private: public QObject {
	Q_OBJECT
public:
	static std::shared_ptr< Setting > self;
	static void destroy( Setting * self );

	Private();

public slots:
	void save();

public:
	QReadWriteLock lock;
	std::map< QString, QVariant > settings;
	QString filePath;
};

}

#endif
