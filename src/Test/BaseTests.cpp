#include "BaseTests.h"

namespace
{
	bool test = TestCase::addTestCase(S(BaseTests), make_shared<BaseTests>());
}

TestResult BaseTests::runTest()
{
	return TestResult::Successful;
}
