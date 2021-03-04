#pragma once
#include "Property.hpp"
#include "Serialize.h"
#include "Value.h"

class Property :
    public Serialize
{
public:
	Property();
	Property(const string&  name, ValuePtr value = ValuePtr());
	~Property() override;

	template <class Archive>
	void serialize(Archive& a)
	{
		a(
			CEREAL_NVP(name_),
			CEREAL_NVP(value_)
		);
	}

public:
	PropertyRW<string> Name;
	PropertyRW<ValuePtr> Value;

private:
	string name_;
	ValuePtr value_;
};
using PropertyPtr = shared_ptr<Property>;
using PropertyList = std::vector<PropertyPtr>;

