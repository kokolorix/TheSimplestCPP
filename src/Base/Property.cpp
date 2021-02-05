#include "Property.h"

Property::Property()
	: Property(string(), ValuePtr())
{

}

Property::Property(const string& name, ValuePtr value /*= ValuePtr()*/)
	: Name( name_)
	, Value(value_)
	, name_(name)
	, value_(value)
{

}

Property::~Property()
{

}

string Property::toJson() const
{
	throw std::exception("The method or operation is not implemented.");
}

void Property::fromJson(const string& json)
{
	throw std::exception("The method or operation is not implemented.");
}

string Property::toXml() const
{
	throw std::exception("The method or operation is not implemented.");
}

void Property::fromXml(const string& xml)
{
	throw std::exception("The method or operation is not implemented.");
}

string Property::writeString() const
{
	throw std::exception("The method or operation is not implemented.");
}

void Property::readString(const string& str)
{
	throw std::exception("The method or operation is not implemented.");
}
