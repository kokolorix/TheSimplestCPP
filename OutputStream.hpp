#ifndef OutputStreamHPP
#define OutputStreamHPP

#include <windows.h>
#include <sstream>
using std::basic_ostream;
using std::basic_ostringstream;
using std::ostringstream;

#include <iostream>
using std::cout;
using std::wcout;
using std::endl;

template <typename Elem>
inline void flush(std::basic_string<Elem> &msg)
{
    if (msg.empty())
        return;

    basic_ostringstream<Elem> os;
    os << msg << endl;
    cout << os.str();
    if (::IsDebuggerPresent())
        debug_out(os.str());
}

template <typename Elem>
inline void debug_out(std::basic_string<Elem> &&msg)
{
    OutputDebugStringA(msg.c_str());
}
template <>
inline void debug_out(std::basic_string<wchar_t> &&msg)
{
    OutputDebugStringW(msg.c_str());
}

template <typename Elem>
class outputbuf : public std::basic_streambuf<Elem>
{
private:
    typedef typename std::basic_streambuf<Elem>::int_type int_type;
    std::basic_string<Elem> msg; ///< buffer for current log message

    void flushMsg()
    {
        if (msg.length() > 0)
        {
            flush(msg);
            msg.erase(); // erase message buffer
        }
    }

public:
    outputbuf(){};
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
    outputbuf(outputbuf const &);      // disallow copy construction
    void operator=(outputbuf const &); // disallow copy assignment
};

template <typename Elem>
class output_stream
    : private outputbuf<Elem>,
      public std::basic_ostream<Elem>
{
public:
    explicit output_stream() : basic_ostream<Elem>(this) {}
};

using OutputStream = output_stream<char>;
extern OutputStream out;

#endif // OutputStreamHPP
