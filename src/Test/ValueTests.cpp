#include "ValueTests.h"

namespace
{
	bool test = TestCase::addTestCase("ValueTests", make_shared<ValueTests>());
}

TestResult ValueTests::runTest()
{
	return TestResult::Successful;
}
