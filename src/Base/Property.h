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

	string toJson() const override;


	void fromJson(const string& json) override;


	string toXml() const override;


	void fromXml(const string& xml) override;


	string writeString() const override;

public:
	PropertyRW<string> Name;
	PropertyRW<ValuePtr> Value;

private:
	string name_;
	ValuePtr value_;
public:
	void readString(const string& str) override;

};
using PropertyPtr = shared_ptr<Property>;
using PropertyList = std::vector<PropertyPtr>;

