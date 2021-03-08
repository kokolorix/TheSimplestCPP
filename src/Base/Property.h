#pragma once
#include "Property.hpp"
#include "Serialize.h"
#include "Value.h"

class Property 
	: public Serialize
	, public Comparable
{
public:
	Property();
	Property(const Property& op);
	Property(const string & name, ValuePtr value = ValuePtr());
	~Property() override;

	template <class Archive>
	void serialize(Archive& a)
	{
		a(
			CEREAL_NVP(name_),
			CEREAL_NVP(value_)
		);
	}

	bool operator <(const Comparable& other) const override;

public:
	PropertyRW<string> Name;
	PropertyRW<ValuePtr> Value;

private:
	string name_;
	ValuePtr value_;
};
//using PropertyPtr = shared_ptr<Property>;

/**
 * @brief PropertyPtr is the shared_ptr for properties
 * @details
 * PropertyPtr bietet mehr Komfort beim erzeugen als 
 * der std::shared_ptr 
 */
struct PropertyPtr : public shared_ptr<const Property>
{
	using BasePtr = shared_ptr<const Property>;
	PropertyPtr() = default;
	PropertyPtr(const PropertyPtr&) = default;
	PropertyPtr(const BasePtr& basePtr) : BasePtr(basePtr) {}

	PropertyPtr(const Property& p) : BasePtr(make_shared<Property>(p)) {}
};
using PropertyList = std::vector<PropertyPtr>;

