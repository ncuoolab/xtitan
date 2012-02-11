#ifndef XTITAN_TESTUNIT_RECORDUNIT_PRIVATE_HPP
#define XTITAN_TESTUNIT_RECORDUNIT_PRIVATE_HPP

#include "RecordUnit.hpp"

namespace xtitan {
	namespace testunit {

		class RecordUnit::Private {
		public:
			Private();

			qint64 lastTick;
			bool ready;
		};

	}
}

#endif
