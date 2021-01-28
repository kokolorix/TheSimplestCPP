#pragma once
#include "Serialize.h"

#include <sstream>
#include <stdint.h>
using std::ostringstream;
using std::istringstream;
using std::boolalpha;

class Value	:
	public Serialize
{
protected:
	Value();
	virtual ~Value();
};
using ValuePtr = shared_ptr<Value>;

template<typename T> 
class	ValueImpl : public Value
{	
public:
	ValueImpl(const T&& v)
		: value_(v)
	{

	}

	string writeString() const override;

	void readString(const string& str) override;

	string toJson() const override;

	void fromJson(const string& json) override;

	string toXml() const override;

	void fromXml(const string& xml) override;

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
using BooleanValue = ValueImpl<bool>;
using Int32Value = ValueImpl<int32_t>;

