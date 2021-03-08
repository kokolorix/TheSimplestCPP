#include "ObjectTest.h"
#include "Object.h"

namespace
{
	bool test = TestCase::addTestCase(S(ObjectTests), make_shared<ObjectTests>());
}

TestResult ObjectTests::runTest() 
{
	return TestResult::Successful;
}
