#include "TestCase.h"
#include <map>
using std::map;
#include <regex>
using std::regex;
using std::regex_match;
using VectorOfstringVectors = std::vector<stringVector>;
#include <chrono>
using namespace std::chrono;
#include <sstream>
using std::endl;
#include "cast.hpp"
#include <algorithm>
using std::max;

namespace
{
	using CaseMap = map<string, TestCasePtr>;

	CaseMap& getCases()
	{
		static CaseMap testCases;
		return testCases;
	}

	struct NameChecker
	{
		NameChecker(const string& pattern) : _pattern(pattern) {}
		bool operator()(const string& name);
		string _pattern;
	};


	bool NameChecker::operator()(const string& name)
	{
		if (_pattern == "*")
			return true;

		if (_pattern == name)
			return true;

		regex rx(_pattern);
		if (regex_match(name, rx))
			return true;
		else
			return false;
	}
}

OutputStream out;

/**
 * @brief adds the test case to the registry of test cases
 * 
 * @param name 
 * @param testCase 
 * @return whether the test case was successfully entered into the registry 
 */
bool TestCase::addTestCase(const string& name, TestCasePtr testCase)
{
	CaseMap& testCases = getCases();
	return !testCases.insert(make_pair(name, testCase)).second;
}

/**
 * @brief Executes the desired test cases
 * 
 * @param  testPattern Regular expression which filters the tet cases to be executed
 */
TestResult TestCase::runTests(const string& testPattern /*= "*"*/)
{
	stringVector labels = { "Test name","Buildup (ms)","Test result","Test (ms)","Breakdown (ms)", "Total (ms)" };
	VectorOfstringVectors outputs = { labels };

	NameChecker check(testPattern);
	CaseMap& testCases = getCases();
	for (CaseMap::value_type& entry :  testCases)
	{
		const string& name = entry.first;
		TestCasePtr pTest = entry.second;
		if (check(name))
		{

			stringVector outValues;
			outValues.reserve(labels.size());
			outValues.push_back(name);
			try
			{
				auto start = std::chrono::high_resolution_clock::now();

				buildUp(pTest, outValues);
				runTest(pTest, outValues);
				breakDown(pTest, outValues);

				auto end = std::chrono::high_resolution_clock::now();

				std::chrono::duration<double, std::milli> elapsed = end - start;
				outValues.push_back(cast<string>(elapsed.count()));
				outputs.push_back(outValues);
			}
			catch (std::exception& e)
			{
				out << "Test " << name << " failed: " << e.what() << std::endl;
				return TestResult::Exception;
			}
			catch (...)
			{
				out << "Test " << name << " failed with unexpected exception" << std::endl;
				return TestResult::Unexpected;
			}
		}
	}

	writeOutput(outputs);

	return TestResult::Successful;
}

/**
 * @brief performs the setup of the test environment for this tet case, 
 * measures the duration and writes the protocol for the output
 * 
 * @param pTest 
 * @param outValues 
 */
void TestCase::buildUp(TestCasePtr pTest, stringVector& outValues)
{
	//					out << "Buildup Test: " << name << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	pTest->buildUp();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	outValues.push_back(cast<string>(elapsed.count()));
}

/**
 * @brief Executes the test case and logs duration and result
 * 
 * @param pTest 
 * @param outValues 
 */
void TestCase::runTest(TestCasePtr pTest, stringVector& outValues)
{
	//				out << "Run Test: " << name << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	switch (pTest->runTest())
	{
	case TestResult::Successful:
		outValues.push_back("Succeed");
		//out << "Test " << name << " successfully completed" << std::endl;
		break;
	case TestResult::Failed:
		outValues.push_back("Failed");
		//out << "Test " << name << " failed" << std::endl;
		break;
	default:
		outValues.push_back("Error");
		//out << "Test " << name << " failed with unexpected result" << std::endl;
		break;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	outValues.push_back(cast<string>(elapsed.count()));
}

/**
 * @brief performs the dismantling of the test environment for this test case, 
 * measures the duration and writes the log for the output
 *  * 
 * @param pTest 
 * @param outValues 
 */
void TestCase::breakDown(TestCasePtr pTest, stringVector& outValues)
{
	//out << "Breakdown Test: " << name << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	pTest->breakDown();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	outValues.push_back(cast<string>(elapsed.count()));
}

/**
 * @brief writes the log of executed test cases to the output window
 * 
 * @param outputs 
 */
void TestCase::writeOutput(const VectorOfstringVectors& outputs)
{
	if (outputs.size() > 1) // if 1, no tests were performed
	{
		const stringVector& labels = outputs.front();
		vector<size_t> colLengts(labels.size(), 0);
		for (size_t col = 0; col < labels.size(); ++col)
		{
			std::for_each(outputs.begin(), outputs.end(), [col, &colLengts](auto &ov) {
				colLengts[col] = max(ov[col].length(), colLengts[col]);
				});
			//for (const stringVector& values : outputs)
			//	colLengts[col] = max(values[col].length(), colLengts[col]);
		}

		for (const stringVector& outValues : outputs)
		{
			writeLine(labels, outValues, colLengts);
			out << endl;
		}
	}
}

/**
 * @brief writes one line formatted into the output window
 * 
 * @param labels the titles
 * @param outValues  the values
 * @param colLengts the column widths
 */
void TestCase::writeLine(const stringVector& labels, const stringVector& outValues, vector<size_t> colLengts)
{
	for (size_t col = 0; col < labels.size(); ++col)
	{
		const string label = labels.at(col);
		if (outValues.size() > col)
		{
			string value = outValues.at(col);
			value += string((colLengts[col] - value.length()), ' ');
			out << value << '\t';
		}
		else
		{
			string value(colLengts[col], ' ');
			out << value << '\t';
		}
	}
}
