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
