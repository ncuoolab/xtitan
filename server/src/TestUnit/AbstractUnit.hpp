#ifndef XTITAN_TESTUNIT_ABSTRACTUNIT_HPP
#define XTITAN_TESTUNIT_ABSTRACTUNIT_HPP

#include "xTitan/Network/SimpleSocket.hpp"

#include <QtCore/QRunnable>

namespace xtitan {

	namespace network {
		class SimpleSocket;
	}

	namespace testcase {
		class CheckPoint;
		class InputPoint;
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
			const testcase::TestCase & getTestCase() const;
			void check( testcase::CheckPoint * point );
			void input( testcase::InputPoint * point );
			virtual void run();

		protected:
			explicit AbstractUnit( QObject * parent );

			testcase::TestCase & testCase();
			void sendMessage( int id, const QString & command, const QVariant & data );

			virtual void doOpen() = 0;
			virtual void doClose() = 0;
			virtual void doTest() = 0;
			virtual void doCheck( testcase::CheckPoint * point ) = 0;
			virtual void doInput( testcase::InputPoint * point ) = 0;
			virtual network::SimpleSocket::Packet onCheck( int id, const QString & label, const QString & value ) = 0;
			virtual network::SimpleSocket::Packet onInput( int id, const QString & label, const QString & script, qint64 waitTime ) = 0;

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
