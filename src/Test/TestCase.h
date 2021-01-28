#pragma once
#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

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
	static TestResult runTests(const string& testPattern = "*");


};

extern OutputStream out;

