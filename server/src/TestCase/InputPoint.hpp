#ifndef XTITAN_TESTCASE_INPUTPOINT_HPP
#define XTITAN_TESTCASE_INPUTPOINT_HPP

#include "AbstractPoint.hpp"

namespace xtitan {
	namespace testcase {

		class InputPoint : public AbstractPoint {
		public:
			InputPoint( int id, const QString & label, const QString & script, qint64 waitTime, int line );

			const QString & getScript() const;
			qint64 getWaitTime() const;

			virtual QString toString() const;
			virtual void execute( testunit::AbstractUnit * unit );

		private:
			InputPoint( const InputPoint & );
			InputPoint & operator =( const InputPoint & );

			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif
