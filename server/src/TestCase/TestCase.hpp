#ifndef XTITAN_TESTCASE_TESTCASE_HPP
#define XTITAN_TESTCASE_TESTCASE_HPP

#include "CheckPoint.hpp"
#include "InputPoint.hpp"

#include <QtCore/QPair>

#include <map>
#include <memory>

namespace xtitan {
	namespace testcase {

		class TestCase {
		public:
			typedef QPair< int, QString > CheckKey;
			typedef std::map< CheckKey, QList< CheckPoint * > > CheckMap;

			TestCase();

			bool load();
			bool save();
			void writeLine( const QString & line );

			void setName( const QString & name );
			const QString & getName() const;
			void setCreator( const QString & creator );
			const QString & getCreator() const;
			void setDescription( const QString & description );
			const QString & getDescription() const;

			void backupRemoteDatabase();
			void restoreRemoteDatabase();
			void importDatabase();

			int getClients() const;
			void setClients( int nClients );
			const QList< Point > & getPoints() const;
			const CheckMap & getCheckPoints() const;

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif
