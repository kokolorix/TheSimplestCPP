#pragma once
#include "Serialize.h"
#include "Property.h"
#include "operators.hpp"

class Object;
using ObjectPtr = shared_ptr<Object>;

class Object :
	public Serialize
{
public:
	Object();
	Object(const string name, PropertyList proprties);

public:
	string toJson() const override;


	void fromJson(const string& json) override;


	string toXml() const override;


	void fromXml(const string& xml) override;

public:
	PropertyRW<string> Name;
	PropertyRW<PropertyList> Properties;

private:
	string name_;
	PropertyList properties_;

	friend bool operator < (const Object& obj1, const Object& obj2);
};

bool operator < (const Object& obj1, const Object& obj2);

