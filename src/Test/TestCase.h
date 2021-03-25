#pragma once
#include <utils.hpp>
#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <vector>
using stringVector = std::vector<string>;

#include "output.hpp"

enum struct TestResult
{
	Successful, Failed, Exception, Unexpected
};

class TestCase;
using TestCasePtr = shared_ptr<TestCase>;

class TestCase
{
public:
	virtual ~TestCase() = default;
	virtual void buildUp() {}
	virtual TestResult runTest() = 0;
	virtual void breakDown() {}

	static bool addTestCase(const string& name, TestCasePtr testCase);
	static void writeOutput(const VectorOfStringVectors& outputs);

	static void writeLine(const stringVector& labels, const stringVector& outValues, vector<size_t> colLengts);

	static TestResult runTests(const string& testPattern = "*");

	static void breakDown(TestCasePtr pTest, stringVector& outValues);

	static void runTest(TestCasePtr pTest, stringVector& outValues);

	static void buildUp(TestCasePtr pTest, stringVector& outValues);


};

extern OutputStream out;

