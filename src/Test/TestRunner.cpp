
#include "TestRunner.h"
#include "Edit.h"
#include "output.hpp"
#include "TestCase.h"

//struct FormatHelper
//{
//};
//#define format FormatHelper()

//string format(const string& frmt)
//{ 
//	return frmt;
//}
//
//template <class T, class... _Args>
//string format(const string& frmt, T&& first, _Args &&... args)
//{
//	return format(frmt, args ...);
//}


void TestRunner::StartTests(const string& pattern)
{
	EditPtr output = Edit::Manager["Output:Edit"];
	output->Caption = "";
	output->add(format("Start tests with filter '%s'\r\n", pattern.c_str()));
	TestCase::runTests(pattern);
}
