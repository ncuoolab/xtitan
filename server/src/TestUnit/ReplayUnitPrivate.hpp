#ifndef XTITAN_TESTUNIT_REPLAYUNIT_PRIVATE_HPP
#define XTITAN_TESTUNIT_REPLAYUNIT_PRIVATE_HPP

#include "ReplayUnit.hpp"

namespace xtitan {
	namespace testunit {

		class ReplayUnit::Private : public QObject {
			Q_OBJECT
		public:
			explicit Private( ReplayUnit * );

			testcase::CheckPoint * waitingPoint;
			QList< QPair< QString, QString > > errorMessage;

		public slots:
			void onError( const QString & );

		signals:
			void releaseWaiting();
		};

	}
}

#endif
