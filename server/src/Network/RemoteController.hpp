#ifndef XTITAN_NETWORK_REMOTECONTROLLER_HPP
#define XTITAN_NETWORK_REMOTECONTROLLER_HPP

#include <QtCore/QIODevice>

namespace xtitan {
	namespace network {

		class RemoteController {
		public:
			static bool exportDatabase( QIODevice * data );
			static bool importDatabase( QIODevice * data );
			static void restartServer();
		};

	}
}

#endif
