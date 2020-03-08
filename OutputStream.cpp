/**
 * @file OutputStream.cpp
 * @brief This Implementation file prevents the use of include windows.h everywhere
 */

#include "OutputStream.hpp"
#include "Windows.h"

/**
 * @brief the global output stream, used from everywhere 
 */
OutputStream out;

bool isDebuggerPresent()
{
    return ::IsDebuggerPresent();
}
// template <typename CharT>
// void outputbuf<CharT>::debug_out(std::basic_string<CharT> &&msg)
/**
 * @brief write to debug output for std::string 
 */
template<>
void outputbuf<char>::debug_out(std::basic_string<char> &&msg)
{
    OutputDebugStringA(msg.c_str());
}
/**
 * @brief write to debug output for std::wstring 
 */
template<>
void outputbuf<wchar_t>::debug_out(std::basic_string<wchar_t> &&msg)
{
    OutputDebugStringW(msg.c_str());
}
