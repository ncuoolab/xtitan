#include "TestCaseManagerPrivate.hpp"

using namespace xtitan::testcase;

namespace {
	const char * CONNECTION_NAME = "default";
}

std::shared_ptr< TestCaseManager > TestCaseManager::Private::self;

void TestCaseManager::Private::destory( TestCaseManager * host ) {
	delete host;
}

TestCaseManager::Private::Private() {
}

TestCaseManager & TestCaseManager::getInstance() {
	if( !Private::self ) {
		Private::self.reset( new TestCaseManager, TestCaseManager::Private::destory );
	}
	return *Private::self;
}

TestCaseManager::TestCaseManager():
p_( new Private ) {
}

TestCaseManager::~TestCaseManager() {
}
