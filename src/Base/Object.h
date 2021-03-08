#pragma once
#include "Serialize.h"
#include "Property.h"

class Object;
using ObjectPtr = shared_ptr<Object>;

class Object
	: public Serialize
	, public Comparable
{
public:
	Object();
	Object(const string name);
	Object(PropertyList proprties);
	Object(const string name, PropertyList proprties);

	template <class Archive>
	void serialize(Archive& a)
	{
		a(
			CEREAL_NVP(name_),
			CEREAL_NVP(properties_)
		);
	}

	bool operator <(const Comparable& other) const override;

public:
	PropertyRW<string> Name;
	PropertyRW<PropertyList> Properties;

private:
	string name_;
	PropertyList properties_;
};

