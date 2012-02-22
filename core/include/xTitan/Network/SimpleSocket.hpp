#ifndef XTITAN_NETWORK_SIMPLESOCKET_HPP
#define XTITAN_NETWORK_SIMPLESOCKET_HPP

#include "xTitan/Utility/Config.hpp"

#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtNetwork/QLocalSocket>

namespace xtitan {
	namespace network {

		class XTITAN_CORE_DLL SimpleSocket : public QObject {
			Q_OBJECT
		public:
			typedef QPair< QString, QVariant > Packet;

			explicit SimpleSocket( QObject * parent = 0 );

			void abort();
			bool atEnd() const;
			void close();
			void connectToServer( const QString & name, QIODevice::OpenMode openMode = QIODevice::ReadWrite );
			Q_INVOKABLE void disconnectFromServer();
			QLocalSocket::LocalSocketError error() const;
			QString errorString() const;
			bool flush();
			QString fullServerName() const;
			bool isValid() const;
			QString serverName() const;
			QLocalSocket::LocalSocketState state() const;
			Packet read();
			bool waitForReadyRead( int msecs = 30000 );
			Q_INVOKABLE void write( const QString & command, const QVariant & data ) const;

		signals:
			void connected();
			void disconnected();
			void error( QLocalSocket::LocalSocketError socketError );
			void readyRead();
			void stateChanged( QLocalSocket::LocalSocketState socketState );

		private:
			friend class SimpleServer;
			explicit SimpleSocket( QLocalSocket *, QObject * = 0 );
			class Private;
			Private * p_;
		};

	}
}

#endif
