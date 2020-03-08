#ifndef OutputStreamHPP
#define OutputStreamHPP

// #include <windows.h>
#include <sstream>
using std::basic_ostream;
using std::basic_ostringstream;
using std::ostringstream;

#include <iostream>
using std::cout;
using std::endl;
using std::wcout;

bool isDebuggerPresent();

template <typename CharT>
class outputbuf : public std::basic_streambuf<CharT>
{
private:
    typedef typename std::basic_streambuf<CharT>::int_type int_type;
    std::basic_string<CharT> msg; ///< buffer for current log message

    void flushMsg()
    {
        if (msg.length() > 0)
        {
            flush(msg);
            msg.erase(); // erase message buffer
        }
    }
    inline void flush(std::basic_string<CharT> &msg)
    {
        if (msg.empty())
            return;

        basic_ostringstream<CharT> os;
        os << msg << endl;
        cout << os.str();
        if (isDebuggerPresent())
            debug_out(os.str());
    }

    void debug_out(std::basic_string<CharT> &&msg);

public:
    outputbuf(){};
    ~outputbuf() { flushMsg(); }

protected:
    virtual int_type overflow(int_type c)
    {
        std::basic_ostream<CharT> tmp(this);
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

template <typename CharT>
class output_stream
    : private outputbuf<CharT>,
      public std::basic_ostream<CharT>
{
public:
    explicit output_stream() : basic_ostream<CharT>(this) {}
};

using OutputStream = output_stream<char>;
extern OutputStream out;

#endif // OutputStreamHPP
