#ifndef XTITAN_TESTCASE_QXCHECKPOINT_PRIVATE_HPP
#define XTITAN_TESTCASE_QXCHECKPOINT_PRIVATE_HPP

#include "CheckPoint.hpp"

namespace xtitan {
	namespace testcase {

		class CheckPoint::Private {
		public:
			explicit Private( const QString & value );

			QString value;
			bool checked;
		};

	}
}

#endif
