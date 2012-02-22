#ifndef XTITAN_UTILITIES_SETTING_PRIVATE_HPP
#define XTITAN_UTILITIES_SETTING_PRIVATE_HPP

#include "xTitan/Utility/Setting.hpp"

#include <QtCore/QReadWriteLock>
#include <QtCore/QObject>

#include <map>

namespace xtitan {
	namespace utilities {

		class Setting::Private : public QObject {
			Q_OBJECT
		public:
			static std::shared_ptr< Setting > self;
			static void destroy( Setting * self );

			Private();

		public slots:
			void save();

		public:
			QReadWriteLock lock;
			std::map< QString, QString > settings;
			QString filePath;
		};

	}
}

#endif
