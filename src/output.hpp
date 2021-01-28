#pragma once
//#include <windows.h>
//#include <assert.h>
//#include <debugapi.h>
#include <vector>
using std::vector;
#include <string>
using std::string;

using StringVector = vector<string>;
using VectorOfStringVectors = vector<StringVector>;

#include "Edit.h"

inline string format(const char* fmt, ...)
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

template<typename Elem>
inline void flush(std::basic_string<Elem>& msg)
{
	if (msg.empty())
		return;

	EditPtr output = Edit::Manager["Output:Edit"];
	output->add(msg.c_str());
	output->add("\r\n");

	if (::IsDebuggerPresent())
	{
		OutputDebugStringA(msg.c_str());
		OutputDebugStringA("\n");
	}
}

template<>
inline void flush(std::basic_string<wchar_t>& msg)
{
	if (msg.empty())
		return;

	if (::IsDebuggerPresent())
	{
		OutputDebugStringW(msg.c_str());
		OutputDebugStringW(L"\n");
	}
}

template<typename Elem>
class outputbuf : public std::basic_streambuf<Elem>
{
private:
	typedef typename std::basic_streambuf<Elem>::int_type int_type;
	std::basic_string<Elem> msg;	///< buffer for current log message

	void flushMsg()
	{
		if (msg.length() > 0)
		{
			flush(msg);
			msg.erase();	// erase message buffer
		}
	}

public:
	outputbuf() { };
	~outputbuf() { flushMsg(); }

protected:
	virtual int_type overflow(int_type c)
	{
		std::basic_ostream<Elem> tmp(this);
		if (c == tmp.widen('\n'))
		{
			flushMsg();
		}
		else
		{
			msg += c;
		}
		return c == -1 ? -1 : ' ';
	}

private:
	outputbuf(outputbuf const&);                 // disallow copy construction
	void operator= (outputbuf const&);           // disallow copy assignment
};

template<typename Elem>
class output_stream
	: private outputbuf<Elem>
	, public std::basic_ostream<Elem>
{
public:
	explicit output_stream() : std::basic_ostream<Elem>(this) { }
};

using OutputStream = output_stream<char>;
