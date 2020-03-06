#include "aTest.hpp"
#include "aInterface.hpp"
#include "OutputStream.hpp"

#include <map>
using std::map;

#include <vector>
using std::vector;

#include <regex>
using std::regex;
using std::regex_match;

#include <chrono>
using namespace std::chrono;

OutputStream out;

namespace
{
using CaseMap = map<string, aTestCasePtr>;
CaseMap &getCaseMap()
{
	static CaseMap caseMap;
	return caseMap;
}

struct NameChecker
{
	NameChecker(const string &pattern) : _pattern(pattern) {}
	bool operator()(const string &name);
	string _pattern;
};

bool NameChecker::operator()(const string &name)
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
} // namespace

void writeOutput(const vector<vector<string>> & outputs);

TestResult runTests(const string& testPattern /*= "*"*/)
{
	vector<string> labels = { "Test name","Buildup (ms)","Test result","Test (ms)","Breakdown (ms)", "Total (ms)" };
	vector<vector<string>> outputs = { labels };
	//writeOutput(labels);

	NameChecker check(testPattern);
	for (CaseMap::value_type& entry : getCaseMap())
	{
		const string& name = entry.first;
		aTestCasePtr pTest = entry.second;
		if (check(name))
		{

			vector<string> values; 
			values.reserve(labels.size());
			values.push_back(name);
			try
			{
				auto start = std::chrono::high_resolution_clock::now();
				{
//					out << "Buildup Test: " << name << std::endl;
					auto start = std::chrono::high_resolution_clock::now();
					pTest->setUp();
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> elapsed = end - start;
					// values.push_back(cast<string>(elapsed.count()));
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
					// values.push_back(cast<string>(elapsed.count()));
				}

				{
					//out << "Breakdown Test: " << name << std::endl;
					auto start = std::chrono::high_resolution_clock::now();
					pTest->tearDown();
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> elapsed = end - start;
					// values.push_back(cast<string>(elapsed.count()));
				}
				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double, std::milli> elapsed = end - start;
				// values.push_back(cast<string>(elapsed.count()));
				outputs.push_back(values);
			}
			catch (std::exception &e)
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

void writeOutput(const vector<vector<string>> & outputs)
{
	if(outputs.size() > 1) // if 1, no tests were performed
	{
		const vector<string> & labels = outputs.front();
		for (const vector<string>& values : outputs)
		{
			for (size_t i=0; i < labels.size(); ++i)
			{
				const string label = labels.at(i);
				if (values.size() > i)
				{
					string value = values.at(i);
					if (value.length() > label.length())
						value.erase(label.size());
					else if (value.length() < label.length())
						value += string(label.length() - value.length(), ' ');
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

bool addTestCase(aTestCasePtr pTest, const string& name)
{
	auto res = getCaseMap().insert(make_pair(name, pTest));
	return res.second;
}
