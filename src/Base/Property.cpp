#include "Property.h"

CEREAL_REGISTER_TYPE(Property)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Serialize, Property)

/**
 * @class Property
 * @brief Properties sind Name, Wert Paare, welche
 * ein Objekt ausmachen
 */

/**
 * @brief Construct a new, empty Property object
 * 
 */
Property::Property()
	: Property(string(), ValuePtr())
{

}
/**
 * @brief Copy-Constructor, Construct a new Property from anotherProperty object
 * 
 * @param op other property 
 */
Property::Property(const Property& op)
	: Name(name_)
	, Value(value_)
	, name_(op.name_)
	, value_(op.value_)
{

}

/**
 * @brief Construct a new Property object with name and value.
 * 
 */
Property::Property(const string& name, ValuePtr value /*= ValuePtr()*/)
	: Name( name_)
	, Value(value_)
	, name_(name)
	, value_(value)
{

}

/**
 * @brief Destroy the Property object
 * 
 */
Property::~Property()
{

}

/**
 * @brief Compare name and value of the property
 * 
 * @param other 
 * @return true if name and value is less to others 
 * @return false if name and value is greather or equal to others
 */
bool Property::operator<(const Comparable& other) const
{
	if (const Property* op = dynamic_cast<const Property*>(&other))
		return std::tie(name_, *value_) < std::tie(op->name_, *op->value_);
	
	return __super::operator<(other);
}
