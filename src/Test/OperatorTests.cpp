//#include "operators.hpp"
#include "OperatorTests.h"
#include "Value.h"
#include "Property.h"
#include "Object.h"

namespace
{
	bool test = TestCase::addTestCase(S(OperatorTests), make_shared<OperatorTests>());
}

TestResult OperatorTests::runTest()
{
	Int32Value v1 = 33, v2 = 33, v3 = 42;
	AssertTrue(v1 == v2, "");
	AssertTrue(v1 != v3, "");

	Property p1("One third", v1), p2("One third", v2), p3("Answer to Everything", v3);
	AssertTrue(p1 == p2, "");
	AssertTrue(p1 != p3, "");

	Object obj1{ "Obj", {p1, p3} }, obj2{ "Obj", {p2, p3} };
	AssertTrue(obj1 == obj2, "");

	return TestResult::Successful;
}
