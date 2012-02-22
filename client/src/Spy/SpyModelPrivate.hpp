#ifndef XTITAN_CLIENT_SPY_SPYMODEL_PRIVATE_HPP
#define XTITAN_CLIENT_SPY_SPYMODEL_PRIVATE_HPP

#include "Spy/SpyModel.hpp"
#include "xTitan/Network/SimpleSocket.hpp"
#include "Command/CommandParser.hpp"

namespace xtitan {
	namespace spy {

		class SpyModel::Private : public QObject {
			Q_OBJECT
		public:
			Private( SpyModel * );

			void send( const network::SimpleSocket::Packet & );

		public slots:
			void onConnected();
			void onReadyRead();

		signals:
			void ready();

		public:
			network::SimpleSocket * socket;
			command::CommandParser commands;
		};

	}
}

#endif
