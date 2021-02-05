#pragma once
#include <utils.hpp>
/**
 * @brief Enables the debug heap to detect memory leaks
 * 
 */
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

#include <assert.h>
#include <stdarg.h>
#include <string>
using std::string;
#include <sstream>
#include <debugapi.h>
#include <stdio.h>
#include <processthreadsapi.h>
using std::ostringstream;
using std::endl;

#define LOG_SOURCE		__FILE__ "(" S(__LINE__) ",1): "	

struct LogHelper
{
	LogHelper(const string& logSrc)
	{
		msg_ << logSrc << " ThreadId: " << ::GetCurrentThreadId();
	}
	~LogHelper()
	{
		msg_ << endl;
		::OutputDebugStringA(msg_.str().c_str());
	}
	void operator()(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		size_t len = _vscprintf(format, args) + (size_t)1; // _vscprintf doesn't count terminating '\0'
		char* buf = new char[len * sizeof(char)];
		_vsnprintf_s(buf, len, len, format, args);
		msg_ << ", Msg: " << buf;
		delete[] buf;
		va_end(args);
	}
	ostringstream msg_;
};
#define LOG	LogHelper(LOG_SOURCE)
