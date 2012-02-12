#ifndef XTITAN_TESTCASE_ABSTRACTPOINT_HPP
#define XTITAN_TESTCASE_ABSTRACTPOINT_HPP

#include <memory>

#include <QtCore/QString>

namespace xtitan {

	namespace testunit {
		class AbstractUnit;
	}

	namespace testcase {

		class AbstractPoint {
		public:
			virtual ~AbstractPoint();

			int getID() const;
			const QString & getLabel() const;
			int getLine() const;

			virtual QString toString() const = 0;
			virtual void execute( testunit::AbstractUnit * unit ) = 0;

		protected:
			AbstractPoint( int id, const QString & label, int line );

		private:
			AbstractPoint( const AbstractPoint & );
			AbstractPoint & operator =( const AbstractPoint & );

			class Private;
			std::shared_ptr< Private > p_;
		};

		typedef std::shared_ptr< AbstractPoint > Point;

	}

}

#endif
