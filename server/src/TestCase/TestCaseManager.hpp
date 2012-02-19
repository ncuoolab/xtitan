#ifndef XTITAN_SERVER_TESTCASE_TESTCASEMANAGER_HPP
#define XTITAN_SERVER_TESTCASE_TESTCASEMANAGER_HPP

#include <memory>

namespace xtitan {
	namespace testcase {

		class TestCaseManager {
		public:
			static TestCaseManager & getInstance();

		private:
			TestCaseManager();
			~TestCaseManager();
			TestCaseManager( const TestCaseManager & );
			TestCaseManager & operator =( const TestCaseManager & );

			class Private;
			friend class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif
