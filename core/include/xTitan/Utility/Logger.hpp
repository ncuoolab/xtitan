#ifndef XTITAN_UTILITIES_LOGGER_HPP
#define XTITAN_UTILITIES_LOGGER_HPP

#include "xTitan/Utility/Config.hpp"

#include <QtCore/QString>

#include <memory>

namespace xtitan {
	namespace utilities {

		class XTITAN_CORE_DLL Logger {
		public :
			static Logger & getInstance();

			void setFileName( const QString & filename );

			void log( const QString & msg );

		private:
			class Private;

			Logger();
			~Logger();

			std::shared_ptr< Private > p_;
		};

	}
}

#endif
