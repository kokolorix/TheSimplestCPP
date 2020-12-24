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
     * @brief The Property himself is  not copyable 
     */
    PropertyR(const PropertyR&) = delete;

    /**
     * @brief Construct a new read only Property object
     * 
     * @param value The constructor installs a getter lambda that operates on a reference of the passed variable 
     */
    PropertyR(T &value) : getter_([&value]() -> T {
                              return value;
                          }) {}
    /**
     * @brief Construct a new read only Property object
     * 
     * @param getter A functor that returns a value of the property type
     */
    PropertyR(Getter getter) : getter_(getter) {}

    /**
     * @brief Conversion operator
     * 
     * @return T const 
     */
    operator T const() { return getter_(); }

    /**
     * @brief Explicit bool operator, used by control structures
     */
    operator bool() const { return static_cast<bool>(getter_()); }

    /**
     * @brief Universal call operator, which also allows properties with std::function
     * 
     * @tparam _Args 
     * @param _Ax 
     */
    template <class... _Args>
	void operator()(_Args &&... _Ax)
	{
		if (getter_())
			getter_()(std::forward<_Args>(_Ax)...);
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
     * @param value The constructor installs a getter and setter lambda that operates on a reference of the passed variable 
     */
    PropertyRW(T &value) : PropertyR<T>(value), setter_([&value](T v) {
                               value = v;
                           }) {}

    /**
     * @brief Construct a new read/write Property object
     * 
     * @param getter A functor that returns a value of the property type
     * @param value The constructor installs a setter lambda that operates on a reference of the passed variable 
     */
    PropertyRW(Getter getter, T &value) : PropertyR<T>(getter), setter_([&value](T v) {
                               value = v;
                           }) {}

    /**
     * @brief Construct a new read/write Property object
     * 
     * @param value The constructor installs a getter lambda that operates on a reference of the passed variable 
     * @param setter A functor that transmits a value of the property type
     */
    PropertyRW(T & value, Setter setter) : PropertyR<T>(value), setter_(setter) {}
    
    /**
     * @brief Construct a new read/write Property object
     * 
     * @param getter A functor that returns a value of the property type
     * @param setter A functor that transmits a value of the property type
     */
    PropertyRW(Getter getter, Setter setter) : PropertyR<T>(getter), setter_(setter) {}

    /**
     * @brief The property himself should not be assignable
     */
    PropertyRW &operator=(PropertyRW&) = delete;

   /**
     * @brief The assignment operator for the property type
     * 
     * @param value 
     * @return PropertyRW& 
     */
    PropertyRW &operator=(T &&value) {
      setter_(value);
      return *this;
    }
    
    /**
     * @name Arithmetic operators
     * 
     * Arithmetic operators, so that the properties are more intuitive to use.
     *
     * Sample:
     * @code {.cpp}
     * class SomeObject {
     *      int someInt;
     * public:
     *      SomeObject() : SomeProeprty(someInt)
     *      {}
     *
     *      PropertyRW<int> SomeProeprty;
     * };
     *
     * @endcode
     * Usage:
     * @code {.cpp}
     * someObject->SomeProperty += 15;
     * @endcode
     *
     *
     */
    /**@{*/
    /** To use as operators of the underlying type*/
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
    /**@}*/ 

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

/**
 * @brief overloded operator, to help Properties reading her value from a stream
 * 
 * @tparam iosT type of io stream
 * @tparam T type of the property
 * @param ios the stream
 * @param prop the property
 * @return iosT& 
 */
template <typename iosT, typename T>
inline iosT &operator>>(iosT &ios, PropertyRW<T> &prop)
{
    T val;
    ios >> val;
    prop = std::move(val);
    return ios;
}

#endif