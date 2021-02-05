#include "PropertyTests.h"
#include "Property.h"

namespace
{
	bool test = TestCase::addTestCase(S(PropertyTests), make_shared<PropertyTests>());
}

TestResult PropertyTests::runTest()
{
	PropertyList properties = {
		PropertyPtr(new Property("AnswerToEverything", Value::create(42)))
		 , PropertyPtr(new Property("HelloWorld", Value::create("Hello World")))
	};

	return TestResult::Successful;
}
