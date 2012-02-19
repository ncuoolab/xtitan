#ifndef XTITAN_TESTUNIT_ABSTRACTUNIT_HPP
#define XTITAN_TESTUNIT_ABSTRACTUNIT_HPP

#include "xTitan/Network/SimpleSocket.hpp"

#include <QtCore/QRunnable>

#include <memory>

namespace xtitan {

	namespace network {
		class SimpleSocket;
	}

	namespace testcase {
		class CheckPoint;
		class InputPoint;
		class SocketPoint;
		class TestCase;
	}

	namespace testunit {

		class AbstractUnit : public QObject, public QRunnable {
			Q_OBJECT

		public:
			virtual ~AbstractUnit();

			void addClient( network::SimpleSocket * socket );
			const QString & getName() const;
			void setName( const QString & name );
			int getClients() const;
			void setClients( int nClients );
			void setCanceled( bool canceled );
			bool isCancled() const;
			std::shared_ptr< testcase::TestCase > getTestCase() const;
			void check( testcase::CheckPoint * point );
			void input( testcase::InputPoint * point );
			void socket( testcase::SocketPoint * point );
			virtual void run();

		protected:
			explicit AbstractUnit( QObject * parent );

			void sendMessage( int id, const QString & command, const QVariant & data );

			virtual void doOpen() = 0;
			virtual void doClose() = 0;
			virtual void doTest() = 0;
			virtual void doCheck( testcase::CheckPoint * point ) = 0;
			virtual void doInput( testcase::InputPoint * point ) = 0;
			virtual void doSocket( testcase::SocketPoint * point ) = 0;
			virtual network::SimpleSocket::Packet onCheck( int id, const QString & label, const QString & value ) = 0;
			virtual network::SimpleSocket::Packet onInput( int id, const QString & label, const QString & script, qint64 waitTime ) = 0;
			virtual network::SimpleSocket::Packet onSocket( int id, const QString & message ) = 0;

		signals:
			void log( const QString & type, const QString & message );
			void error( const QString & type, const QString & errorString );
			void canceled();
			void finished();

		private:
			class Private;
			Private * p_;
		};

	}

}

#endif
