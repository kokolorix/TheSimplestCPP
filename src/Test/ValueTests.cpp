#include "ValueTests.h"
#include "Value.h"

namespace
{
	bool test = TestCase::addTestCase(S(ValueTests), make_shared<ValueTests>());

	void stackTests();
	void heapTests();
}

TestResult ValueTests::runTest()
{
	try
	{
		stackTests();
		heapTests();
		return TestResult::Successful;
	}
	catch (exception&)
	{
		return TestResult::Failed;
	}

}
namespace
{
	void stackTests()
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
	}
//#define AssertTrue(condition, message) { if(!(condition)) { throw exception(format("AssertTrue: %s\t%s\t%d\t%s", #condition, __FILE__, __LINE__, (message)).c_str()); }  }

	void heapTests()
	{
		ValuePtr bv1(false);
		ValuePtr	bv2 = bv1;
		AssertTrue( bv2.use_count() == 2, "");
		BooleanValuePtr ptr1 = bv1, ptr2 = bv2;
		AssertTrue((*ptr1 == *ptr2), "");
		AssertTrue( bv2.use_count() == 4, "");
		bv2 = true;  
		BooleanValuePtr ptr3 = bv2;
		AssertFalse((*ptr1 == *ptr3), "");
		AssertTrue( bv2.use_count() == 2, "");
		AssertTrue( bv1.use_count() == 3, "");
	}
}
