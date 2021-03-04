#include "BaseTests.h"
#include "Property.hpp"

namespace
{
	bool test = TestCase::addTestCase(S(BaseTests), make_shared<BaseTests>());
}

TestResult BaseTests::runTest()
{
	struct Type
	{
		string name() const { return "Type"; }
	};
	using TypePtr = shared_ptr<Type>;
	TypePtr type_ = make_shared<Type>();
	PropertyRW<TypePtr> type(type_);
	type->name();

	return TestResult::Successful;
}
