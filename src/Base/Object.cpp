#include "Object.h"

CEREAL_REGISTER_TYPE(Object)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Serialize, Object)

/**
 * @class Object
 * @brief Objects are named containers of properties
 * 
 */

/**
 * @brief Construct a new, empty Object
 * 
 */
Object::Object()
	:Object(string(), PropertyList())
{

}

/**
 * @brief Construct a new named Object without properties
 * 
 * @param name 
 */
Object::Object(const string name)
	: Name(name_)
	, Properties(properties_)
	, name_(name)
{

}

/**
 * @brief Construct a new unnamed Object with some properties
 * 
 * @param proprties 
 */
Object::Object(PropertyList proprties) 
	: Name(name_)
	, Properties(properties_)
{
	
}

/**
 * @brief Construct a new named Object with some properties
 * 
 * @param name 
 * @param properties 
 */
Object::Object(const string name, PropertyList properties)
	: Name(name_)
	, Properties(properties_)
	, name_(name)
	, properties_(properties)
{

}

/**
 * @brief The less operator for PropertyList.
 * @details
 * Compares the list of properties from one object to another
 * 
 * @param pl1 
 * @param pl2 
 * @return true 
 * @return false 
 */
bool operator<(const PropertyList& pl1, const PropertyList& pl2)
{
	return std::lexicographical_compare(
		pl1.begin(), pl1.end(), pl2.begin(), pl2.end(),
		[](const PropertyPtr p1, const PropertyPtr p2)
		{
			return *p1 < *p2;
		});
}

bool Object::operator<(const Comparable& other) const
{
	if (const Object* oo = dynamic_cast<const Object*>(&other))
		return std::tie(name_, properties_) < std::tie(oo->name_, oo->properties_);

	return __super::operator<(other);
}

