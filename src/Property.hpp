#ifndef PropertyHPP
#define PropertyHPP

#include <functional>

using std::bind;
using std::function;
using namespace std::placeholders; // for _1, _2, ...

/**
 * @brief utility class to emulate the read only Property functionality of .NET
 * @tparam T The type of the Property
 */
template <typename T>
struct PropertyR
{
    using Getter = function<T(void)>;

    /**
     * @brief Construct a new reaf only Property object
     * 
     * The constructor installs a getter lambda that
     * operates on a reference of the passed variable     * 
     * 
     * @param value 
     */
    PropertyR(T &value) : getter_([&value]() -> T {
                              return value;
                          }) {}

    PropertyR(Getter getter) : getter_(getter) {}

    operator T const() { return getter_(); }

    
    template <class... _Args>
	void operator()(_Args &&... _Ax)
	{
		getter_()(forward<_Args>(_Ax)...);
	}


protected:
    Getter getter_;
};
/**
 * @brief utility class to emulate the Property functionality of .NET
 * @tparam T The type of the Property
 */
template <typename T>
struct PropertyRW : public PropertyR<T>
{
    using Getter = typename PropertyR<T>::Getter;   ///> Prototype from getter function
    using Setter = function<void(T newValue)>;      ///> Protoype of setter function

    /**
     * @brief Construct a new read/write Property object
     * 
     * The constructor installs a setter lambda that
     * operates on a reference of the passed variable     * 
     * 
     * @param value 
     */
    PropertyRW(T &value) : PropertyR<T>(value), setter_([&value](T v) {
                               value = v;
                           }) {}


    PropertyRW(Getter getter, T &value) : PropertyR<T>(getter), setter_([&value](T v) {
                               value = v;
                           }) {}
                           
    PropertyRW(T & value, Setter setter) : PropertyR<T>(value), setter_(setter) {}
    
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