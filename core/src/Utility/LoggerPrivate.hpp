#ifndef XTITAN_UTILITIES_LOGGER_PRIVATE_HPP
#define XTITAN_UTILITIES_LOGGER_PRIVATE_HPP

#include "xTitan/Utility/Logger.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMutex>
#include <QtCore/QTextStream>

namespace xtitan {
	namespace utilities {

		class Logger::Private {
		public:
			static std::shared_ptr< Logger > self;
			static void destroy( Logger * self );

			Private();

			QMutex lock;
			QDir tcDir;
			QFile fout;
			QTextStream stream;
		};

	}
}

#endif
