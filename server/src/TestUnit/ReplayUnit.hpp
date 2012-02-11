#ifndef XTITAN_TESTUNIT_REPLAYUNIT_HPP
#define XTITAN_TESTUNIT_REPLAYUNIT_HPP

#include "AbstractUnit.hpp"

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPair>

namespace xtitan {
	namespace testunit {

		class ReplayUnit : public AbstractUnit {
			Q_OBJECT
		public:
			explicit ReplayUnit( QObject * parent );

		protected:
			virtual void doOpen();
			virtual void doTest();
			virtual void doClose();
			virtual void doCheck( testcase::CheckPoint * point );
			virtual void doInput( testcase::InputPoint * point );
			virtual network::SimpleSocket::Packet onCheck( int id, const QString & label, const QString & value );
			virtual network::SimpleSocket::Packet onInput( int id, const QString & label, const QString & script, qint64 waitTime );

		signals:
			void releaseWaiting();

		private:
			class Private;
			Private * p_;
		};

	}
}

#endif
