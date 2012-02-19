#ifndef XTITAN_CLIENT_COMMAND_MESSAGEENCODER_HPP
#define XTITAN_CLIENT_COMMAND_MESSAGEENCODER_HPP

#include "xTitan/Network/SimpleSocket.hpp"

#include <functional>
#include <memory>

namespace xtitan {
	namespace command {

		class MessageEncoder {
		public:
			typedef std::function< network::SimpleSocket::Packet ( const QVariant & ) > Encoder;
			static MessageEncoder & getInstance();

			bool registerEncoder( const QString & tag, const Encoder & encoder );
			network::SimpleSocket::Packet encode( const QString & tag, const QVariant & data ) const;

		private:
			MessageEncoder();
			~MessageEncoder();
			MessageEncoder( const MessageEncoder & );
			MessageEncoder & operator =( const MessageEncoder & );
			class Private;
			friend class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif
