#include "ObjectTest.h"
#include "Object.h"

namespace
{
	bool test = TestCase::addTestCase(S(ObjectTests), make_shared<ObjectTests>());
}

TestResult ObjectTests::runTest() 
{
	Object obj1, obj2;
	AssertTrue(obj1 == obj2, "");
	return TestResult::Successful;
}
