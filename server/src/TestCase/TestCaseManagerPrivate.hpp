#ifndef XTITAN_SERVER_TESTCASE_TESTCASEMANAGER_PRIVATE_HPP
#define XTITAN_SERVER_TESTCASE_TESTCASEMANAGER_PRIVATE_HPP

#include "TestCaseManager.hpp"

namespace xtitan {
	namespace testcase {

		class TestCaseManager::Private {
		public:
			static std::shared_ptr< TestCaseManager > self;
			static void destory( TestCaseManager * );

			Private();
		};

	}
}

#endif
