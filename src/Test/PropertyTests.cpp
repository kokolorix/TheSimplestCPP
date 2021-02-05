#include "ProeprtyTests.h"

namespace
{
	bool test = TestCase::addTestCase(S(ProeprtyTests), make_shared<ProeprtyTests>());
}

TestResult ProeprtyTests::runTest()
{
	return TestResult::Successful;
}
