#ifndef XTITAN_TESTCASE_CHECKPOINT_HPP
#define XTITAN_TESTCASE_CHECKPOINT_HPP

#include "AbstractPoint.hpp"

namespace xtitan {
	namespace testcase {

		class CheckPoint : public AbstractPoint {
		public:
			CheckPoint( int id, const QString & label, const QString & value, int line );

			const QString & getValue() const;
			void setChecked( bool checked );
			bool isChecked() const;

			virtual QString toString() const;
			virtual void execute( testunit::AbstractUnit * unit );

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif
