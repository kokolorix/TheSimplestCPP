#ifndef HelpersHPP
#define HelpersHPP

template <typename T>
bool is_one_of(T value, T first) // End of recursive call
{
    return value == first;
}
template <typename T, typename... Targs>
bool is_one_of(T value, T first, Targs... args) // Recursive variadic function
{
    if (value == first)
        return true;
    else
        return is_one_of(value, args...); // ecursive call
}
namespace impl
{
// template <typename R, class In, template <class In> class P>
// struct cast_helper
// {
//     inline R operator()(P<In> p) { return *p; }
// };
template <typename R, typename In>
struct cast_helper
{
    inline R operator()(In in) { return static_cast<R>(in); }
};

template <typename In>
struct cast_helper<string, In>
{
    inline string operator()(In in)
    {
        std::ostringstream os;
        os << in;
        return string(os.str());
    }
};

template <typename R>
struct cast_helper<R, string>
{
    inline R operator()(string in)
    {
        R r;
        std::istringstream is(in);
        is >> r;
        return r;
    }
};

template <>
struct cast_helper<string, string>
{
    inline string operator()(string in)
    {
        return in;
    }
};
} // namespace impl
template <typename R, typename In>
R cast(In in)
{
    impl::cast_helper<R, In> helper;
    return helper(in);
}

#endif