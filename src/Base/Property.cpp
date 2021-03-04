#include "Property.h"

CEREAL_REGISTER_TYPE(Property)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Serialize, Property)

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
