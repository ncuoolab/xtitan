#ifndef XTITAN_TESTCASE_ABSTRACTPOINT_PRIVATE_HPP
#define XTITAN_TESTCASE_ABSTRACTPOINT_PRIVATE_HPP

#include "AbstractPoint.hpp"

namespace xtitan {
	namespace testcase {

		class AbstractPoint::Private {
		public:
			Private( int id, const QString & label, int line );

			int id;
			QString label;
			int line;
		};

	}
}

#endif
