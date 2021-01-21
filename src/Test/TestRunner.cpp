
#include "TestRunner.h"
#include "Edit.h"

//struct FormatHelper
//{
	string format(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		size_t len = _vscprintf(fmt, args) + (size_t)1; // _vscprintf doesn't count terminating '\0'
		char* buf = new char[len * sizeof(char)];
		_vsnprintf_s(buf, len, len, fmt, args);
		string msg_;
		msg_ = buf;
		delete[] buf;
		va_end(args);
		return msg_;
	}
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
	output->addLine(format("Start tests with filter '%s'", pattern.c_str()));
}
