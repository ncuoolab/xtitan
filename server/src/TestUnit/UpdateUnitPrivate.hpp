#ifndef XTITAN_TESTUNIT_UPDATEUNIT_PRIVATE_HPP
#define XTITAN_TESTUNIT_UPDATEUNIT_PRIVATE_HPP

#include "UpdateUnit.hpp"

namespace xtitan {
	namespace testunit {

		class UpdateUnit::Private {
		public:
			Private();

			testcase::TestCase inputCase;
		};

	}
}

#endif
