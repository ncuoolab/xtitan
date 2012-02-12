#ifndef XTITAN_NETWORK_SIMPLESERVER_HPP
#define XTITAN_NETWORK_SIMPLESERVER_HPP

#include "xTitan/Utility/Config.hpp"

#include <QtCore/QObject>
#include <QtNetwork/QLocalSocket>

namespace xtitan {
	namespace network {

		class SimpleSocket;

		class XTITAN_CORE_DLL SimpleServer : public QObject {
			Q_OBJECT
		public:
			explicit SimpleServer( QObject * parent = 0 );

			void close();
			QString errorString() const;
			QString fullServerName() const;
			bool hasPendingConnections() const;
			bool isListening() const;
			bool listen( const QString & name );
			SimpleSocket * nextPendingConntion();
			QAbstractSocket::SocketError serverError() const;
			QString serverName() const;

		signals:
			void newConnection();

		private:
			class Private;
			Private * p_;
		};

	}
}

#endif
