#ifndef XTITAN_NETWORK_SIMPLESERVER_PRIVATE_HPP
#define XTITAN_NETWORK_SIMPLESERVER_PRIVATE_HPP

#include "xTitan/Network/SimpleServer.hpp"

#include <QtNetwork/QLocalServer>
#include <QtCore/QQueue>

namespace xtitan {
	namespace network {

		class SimpleServer::Private : public QObject {
			Q_OBJECT
		public:
			explicit Private( QObject * parent );

		public slots:
			void onNewConnection();

		signals:
			void newConnection();

		public:
			QLocalServer * server;
			QQueue< SimpleSocket * > queue;
		};

	}
}

#endif
