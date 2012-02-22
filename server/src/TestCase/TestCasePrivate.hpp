#ifndef XTITAN_TESTCASE_QXTESTCASE_PRIVATE_HPP
#define XTITAN_TESTCASE_QXTESTCASE_PRIVATE_HPP

#include "TestCase.hpp"

#include <QtCore/QByteArray>
#include <QtCore/QStringList>

namespace xtitan {
	namespace testcase {

		class TestCase::Private {
		public:
			Private();

			int nClients;
			QString name;
			QString creator;
			QString description;
			QList< Point > points;
			TestCase::CheckMap checkPoints;
			QByteArray remoteDatabase;
			QByteArray database;
			QString databasePath;
			QStringList recordedScript;
		};

	}
}

#endif
