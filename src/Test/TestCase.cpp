#include "TestCase.h"
#include <map>
using std::map;
#include <regex>
using std::regex;
using std::regex_match;
#include <vector>
using stringVector = std::vector<string>;
using VectorOfstringVectors = std::vector<stringVector>;
#include <chrono>
using namespace std::chrono;
#include <sstream>
using std::endl;
#include "cast.hpp"

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

bool TestCase::addTestCase(const string& name, TestCasePtr testCase)
{
	CaseMap& testCases = getCases();
	return !testCases.insert(make_pair(name, testCase)).second;
}

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

			stringVector values;
			values.reserve(labels.size());
			values.push_back(name);
			try
			{
				auto start = std::chrono::high_resolution_clock::now();
				{
					//					out << "Buildup Test: " << name << std::endl;
					auto start = std::chrono::high_resolution_clock::now();
					pTest->buildUp();
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> elapsed = end - start;
					values.push_back(cast<string>(elapsed.count()));
				}
				{
					//				out << "Run Test: " << name << std::endl;
					auto start = std::chrono::high_resolution_clock::now();
					switch (pTest->runTest())
					{
					case TestResult::Successful:
						values.push_back("Succeed");
						//out << "Test " << name << " successfully completed" << std::endl;
						break;
					case TestResult::Failed:
						values.push_back("Failed");
						//out << "Test " << name << " failed" << std::endl;
						break;
					default:
						values.push_back("Error");
						//out << "Test " << name << " failed with unexpected result" << std::endl;
						break;
					}
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> elapsed = end - start;
					values.push_back(cast<string>(elapsed.count()));
				}

				{
					//out << "Breakdown Test: " << name << std::endl;
					auto start = std::chrono::high_resolution_clock::now();
					pTest->breakDown();
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> elapsed = end - start;
					values.push_back(cast<string>(elapsed.count()));
				}
				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double, std::milli> elapsed = end - start;
				values.push_back(cast<string>(elapsed.count()));
				outputs.push_back(values);
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


void TestCase::writeOutput(const VectorOfstringVectors& outputs)
{
	if (outputs.size() > 1) // if 1, no tests were performed
	{
		const stringVector& labels = outputs.front();
		for (const stringVector& values : outputs)
		{
			for (size_t i = 0; i < labels.size(); ++i)
			{
				const string label = labels.at(i);
				if (values.size() > i)
				{
					string value = values.at(i);
					if (value.length() > label.length())
						value.erase(label.size());
					else if (value.length() < label.length())
						value += string((label.length() - value.length()), ' ');
					out << value << "\t";
				}
				else
				{
					string value(label.size(), ' ');
					out << value << "\t";
				}
			}
			out << endl;
		}
	}
}
