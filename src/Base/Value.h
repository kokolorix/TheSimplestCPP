#pragma once
#include "Serialize.h"

#include <sstream>
#include <stdint.h>
using std::ostringstream;
using std::istringstream;
using std::boolalpha;

class Value;
//using ValuePtr = shared_ptr<const Value>;
struct ValuePtr;

template<typename T>	class	ValueImpl;

class Value	:
	public Serialize
{
public:
	template<typename T> static ValuePtr create(T value);
protected:
	Value();
	virtual ~Value();
};

struct ValuePtr : public shared_ptr<const Value>
{
	using BasePtr = shared_ptr<const Value>;
	ValuePtr() = default;
	ValuePtr(const ValuePtr&) = default;
	ValuePtr(const BasePtr& basePtr) : BasePtr(basePtr) {}

	template<typename T>
	ValuePtr(T value);

	template<typename T>
	operator shared_ptr<const ValueImpl<T>>();
};

template<typename T> 
class	ValueImpl : public Value
{	
public:
	ValueImpl(T v) : value_(v) { 	}

	string writeString() const override;

	void readString(const string& str) override;

	string toJson() const override;

	void fromJson(const string& json) override;

	string toXml() const override;

	void fromXml(const string& xml) override;

	operator T() const { return value_; }

private:
	T value_;
};

template<typename T>
string ValueImpl<T>::writeString() const
{
	ostringstream os;
	os << boolalpha << value_;
	return os.str();
}

template<typename T>
void ValueImpl<T>::readString(const string& str)
{
	istringstream is(str);
	is >> boolalpha >> value_;
}

template<typename T>
string ValueImpl<T>::toJson() const
{
	return writeString();
}

template<typename T>
void ValueImpl<T>::fromJson(const string& json)
{
	readString(json);
}

template<typename T>
string ValueImpl<T>::toXml() const
{
	return writeString();
}

template<typename T>
void ValueImpl<T>::fromXml(const string& xml)
{
	readString(xml);
}

using StringValue = ValueImpl<string>;
using StringValuePtr = shared_ptr<const ValueImpl<string>>;

using BooleanValue = ValueImpl<bool>;
using BooleanValuePtr = shared_ptr<const ValueImpl<bool>>;

using Int32Value = ValueImpl<int32_t>;
using Int32ValuePtr = shared_ptr<const ValueImpl<int32_t>>;


template<typename T>
ValuePtr::ValuePtr(T value)
	: BasePtr(make_shared<const ValueImpl<T>>(value))
{

}
template<typename T>
ValuePtr::operator shared_ptr<const ValueImpl<T>>()
{
	return dynamic_pointer_cast<const ValueImpl<T>>(*this);
}



template<typename T>
inline ValuePtr Value::create(T value)
{
	auto valuePtr = make_shared<const ValueImpl<T>>(value);
	return  dynamic_pointer_cast<const Value>(valuePtr);
}

template<>
inline ValuePtr Value::create(const char *value)
{
	return create<string>(value);
}
