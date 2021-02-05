#include "ValueTests.h"
#include "Value.h"

namespace
{
	bool test = TestCase::addTestCase(S(ValueTests), make_shared<ValueTests>());
}

TestResult ValueTests::runTest()
{
	BooleanValue booleanValue1(false);		
	booleanValue1 = true;

	string booeanString = booleanValue1.writeString();

	BooleanValue booleanValue2(false);
	booleanValue2.readString(booeanString);

	Int32Value intValue1 = 33;
	string intStr = intValue1.writeString();
	Int32Value intValue2 = -42;
	intValue2.readString(intStr);

	return TestResult::Successful;
}
