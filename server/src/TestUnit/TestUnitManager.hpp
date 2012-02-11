#ifndef XTITAN_TESTUNIT_TESTUNITMANAGER_HPP
#define XTITAN_TESTUNIT_TESTUNITMANAGER_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <memory>

class QxTestCase;

namespace xtitan {

	namespace testcase {
		class TestCase;
	}

	namespace testunit {

		class TestUnitManager : public QObject {
			Q_OBJECT
		public:
			explicit TestUnitManager( QObject * parent );

			void RegressionTest();
			void SmokeTest();
			void specificTest( const QStringList & tests, bool killing );
			void stopTest();

			void startRecord( int nClients );
			void startUpdate( const QString & name );
			testcase::TestCase stopRecord();

			bool isOneShot() const;
			void setOneShot( bool oneShot );

		signals:
			void error( const QString & type, const QString & errorString );
			void finished();
			void finished( const QxTestCase & testCase );
			void log( const QString & type, const QString & message );

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}

}

#endif
