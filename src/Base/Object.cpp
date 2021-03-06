#include "Object.h"

Object::Object()
	:Object(string(), PropertyList())
{

}

Object::Object(const string name, PropertyList properties)
	: Name(name_)
	, Properties(properties_)
	, name_(name)
	, properties_(properties)
{

}

string Object::toJson() const
{
	throw std::exception("The method or operation is not implemented.");
}

void Object::fromJson(const string& json)
{
	throw std::exception("The method or operation is not implemented.");
}

string Object::toXml() const
{
	throw std::exception("The method or operation is not implemented.");
}

void Object::fromXml(const string& xml)
{
	throw std::exception("The method or operation is not implemented.");
}

bool operator<(const Object& obj1, const Object& obj2)
{
	return std::tie(obj1.name_, obj1.properties_) == std::tie(obj2.name_, obj2.properties_);
}
