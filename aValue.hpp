#ifndef aValueHPP
#define aValueHPP

#include <sstream>

#include "aInterface.hpp"
#include "Helpers.hpp"

enum struct aValueType : uint8_t
{
    vtUndefinesd, ///< unknown data type  
    vtInt32,    ///< 32bit signed integer  #aValueImpl<int>  
    vtUInt32,
    vtFloat32,
    vtFloat64,
    vtString,
    vtBoolean,
    vtEnum8 ///< 8bit enumeration
}; 

struct aValue : public  aInterface
{
    virtual bool canConvert(aValueType valueType) = 0;
    virtual string writeString() = 0; ///> A complete text representation of the object, the complement of the readString method
    virtual void readString(string str) = 0; ///> Method to convert the text of writeString back into a complete object
};

/**
 * @brief A simple value class to enable uniform serialization
 * 
 * @tparam T The type of the value, e.g. int, string, double, etc.
 */
template <typename T>
struct aValueImpl : public aValue
{
public:
    aValueImpl(const aValueImpl&) = default;
    aValueImpl(T value) : value_(value) {}             ///> The conversion constructor
    
    template<typename U>
    aValueImpl(U&& value) : value_(value) {}             ///> The conversion constructor
    template<typename U>
    aValueImpl &operator=(U&& value)
    {
        value_ = value;
        return *this;
    }

    operator T&() { return get_value(); }               ///> The implicit conversion
    operator const T&() const { return get_value(); }   ///> The implicit conversion

    T& get_value() { return value_; }               ///> The getter for the value
    const T& get_value() const { return value_; }   ///> The getter for the value
    void set_value(T& value) { value_ = value; }    ///> The setter for the value

    inline aValueType get_valueType() const { return aValueType::vtUndefinesd; }

    virtual bool canConvert(aValueType valueType) override { return is_one_of(valueType, aValueType::vtString, get_valueType()); }
    virtual string writeString() override
    {
        std::ostringstream os;
        os << value_;
        return os.str();
    }
    virtual void readString(string str) override
    {
        std::istringstream is(str);
        is >> value_;
    }

private:
    T value_; ///> The value
};

template <typename iosT, typename T>
inline iosT &operator<<(iosT &ios, const aValueImpl<T> &val)
{
    return ios <<  val.get_value();
}

using aInt32 = aValueImpl<int>;
template<> inline aValueType aValueImpl< int >::get_valueType() const { return aValueType::vtInt32; }
template<> inline aValueType aValueImpl< double >::get_valueType() const { return aValueType::vtFloat64; }
template<> inline aValueType aValueImpl< string >::get_valueType() const { return aValueType::vtString; }
template<> inline aValueType aValueImpl< bool >::get_valueType() const { return aValueType::vtBoolean; }

// template<> aValueImpl<string>::aValueImpl(string s)  :  value_(s) { }

#endif