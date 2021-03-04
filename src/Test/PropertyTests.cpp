#include "PropertyTests.h"
#include "Property.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

namespace
{
	bool test = TestCase::addTestCase(S(PropertyTests), make_shared<PropertyTests>());
}

TestResult PropertyTests::runTest()
{
	PropertyList properties = {
		PropertyPtr(new Property("AnswerToEverything", ValuePtr(42)))
		, make_shared<Property>("HelloWorld", ValuePtr("Hello World"))
	};

	std::stringstream ss; // any stream can be used
	string before, after;
	{
		cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

		oarchive(*properties[0],*properties[1]); // Write the data to the archive
		before = properties[0]->toJson();
	}
	{
		cereal::BinaryInputArchive iarchive(ss); // Create an input archive
		Property p0, p1;
		iarchive(p0, p1); // Read the data from the archive
		after = p0.toJson();
	}
	AssertTrue(before == after, "");

	return TestResult::Successful;
}
