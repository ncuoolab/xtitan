#ifndef XTITAN_CLIENT_COMMAND_MESSAGEENCODER_PRIVATE_HPP
#define XTITAN_CLIENT_COMMAND_MESSAGEENCODER_PRIVATE_HPP

#include "xTitanClient/Command/MessageEncoder.hpp"

#include <QtCore/QReadWriteLock>

#include <map>

namespace xtitan {
	namespace command {

		class MessageEncoder::Private {
		public:
			static std::shared_ptr< MessageEncoder > self;
			static void destory( MessageEncoder * );
			Private();

			QReadWriteLock lock;
			std::map< QString, Encoder > encoders;
		};

	}
}

#endif
