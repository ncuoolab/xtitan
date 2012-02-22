#ifndef XTITAN_TESTUNIT_TESTUNITMANAGER_PRIVATE_HPP
#define XTITAN_TESTUNIT_TESTUNITMANAGER_PRIVATE_HPP


#include "TestUnitManager.hpp"
#include "AbstractUnit.hpp"
#include "xTitan/Network/SimpleServer.hpp"

namespace xtitan {
	namespace testunit {

		class TestUnitManager::Private : public QObject {
			Q_OBJECT
		public:
			Private();
			virtual ~Private();

			void makeServerReport();
			void createLogDir();
			void setupReplay( bool );
			void nextReplay();

			network::SimpleServer * server;
			AbstractUnit * currentUnit;

		public slots:
			void onNewConnection();
			void onUnitError( const QString &, const QString & );
			void onReplayFinished();
			void onRecordFinished();
			void onUpdateFinished();

		signals:
			void error( const QString &, const QString & );
			void finished();
			void finished( const xtitan::testcase::TestCase & testCase );
			void log( const QString &, const QString & );

		public:
			bool testing;
			bool oneShot;
			int currentTestCount;
			QStringList testList;
			QStringList errorList;
		};

	}
}

#endif
