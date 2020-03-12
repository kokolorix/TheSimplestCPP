#ifndef PropertyHPP
#define PropertyHPP

#include <functional>

using std::bind;
using std::function;
using namespace std::placeholders; // for _1, _2, ...

/**
 * @brief utility class to emulate the read only property functionality of .NET
 * @tparam T The type of the Property
 */
template <typename T>
struct PropertyR
{
    using Getter = function<T(void)>;

    // PropertyR(aValueImpl<T> &value) {
    //     auto a = bind(&aValueImpl<T>::get_value, value);
    // }
    PropertyR(T &value) : getter_([&value]() -> T {
                              return value;
                          }) {}
    PropertyR(Getter getter) : getter_(getter) {}
    operator T const() { return getter_(); }

protected:
    Getter getter_;
};
/**
 * @brief utility class to emulate the property functionality of .NET
 * @tparam T The type of the Property
 */
template <typename T>
struct PropertyRW : public PropertyR<T>
{
    using Getter = typename PropertyR<T>::Getter;
    using Setter = function<void(T newValue)>;
    PropertyRW(T & value) : PropertyR<T>(value), setter_([&value](T v) {
                                value = v;
                            }) {}
    PropertyRW(Getter getter, Setter setter) : PropertyR<T>(getter), setter_(setter) {}

    PropertyRW &operator=(T &&value)
    {
        setter_(value);
        return *this;
    }
    PropertyRW &operator+=(T &&value)
    {
        setter_(PropertyR<T>::getter_() + value);
        return *this;
    }
    PropertyRW &operator-=(T &&value)
    {
        setter_(PropertyR<T>::getter_() - value);
        return *this;
    }
    PropertyRW &operator*=(T &&value)
    {
        setter_(PropertyR<T>::getter_() * value);
        return *this;
    }
    PropertyRW &operator/=(T &&value)
    {
        setter_(PropertyR<T>::getter_() / value);
        return *this;
    }

protected:
    Setter setter_;
};

/**
 * @brief overloded operator, to help Properties writing her value to a stream
 * 
 * @tparam iosT type of io stream
 * @tparam T type of the property
 * @param ios the stream
 * @param prop the property
 * @return iosT& 
 */
template <typename iosT, typename T>
inline iosT &operator<<(iosT &ios, PropertyR<T> &prop)
{
    return ios << static_cast<T>(prop);
}
template <typename iosT, typename T>
inline iosT &operator>>(iosT &ios, PropertyRW<T> &prop)
{
    T val;
    ios >> val;
    prop = std::move(val);
    return ios;
}

#endif