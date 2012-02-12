#ifndef QXABSTRACTUNITPRIVATE_H
#define QXABSTRACTUNITPRIVATE_H

#include "AbstractUnit.hpp"
#include "TestCase/TestCase.hpp"

#include <QtCore/QProcess>

#include <functional>

namespace xtitan {
	namespace testunit {

		class AbstractUnit::Private : public QObject {
			Q_OBJECT

		public:
			typedef std::map< QString, std::function< network::SimpleSocket::Packet ( network::SimpleSocket *, const QVariant & ) > > CommandMap;

			explicit Private( AbstractUnit * host );

		public slots:
			void onSocketReadyRead();
			void onSocketError( QLocalSocket::LocalSocketError socketError );
			void onSocketDisconnected();

		signals:
			void readyForTest();
			void log( const QString &, const QString & );
			void error( const QString &, const QString & );

		public:
			void addClient( network::SimpleSocket * socket );
			void spawn();
			void restartServer();

			network::SimpleSocket::Packet onCheck( network::SimpleSocket * sender, const QVariant & data );
			network::SimpleSocket::Packet onInput( network::SimpleSocket * sender, const QVariant & data );

			AbstractUnit * host;
			QList< network::SimpleSocket * > sockets;
			QList< QProcess * > children_;
			testcase::TestCase testCase;
			CommandMap commands;
			bool canceled;
		};

	}
}

#endif
