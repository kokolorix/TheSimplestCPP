#pragma once
#include "Serialize.h"

class Value	:
	public Serialize
{
public:
	virtual ~Value();

	string toJson() const override;


	void fromJson(const string& json) override;


	string toXml() const override;


	void fromXml(const string& xml) override;

};
using ValuePtr = shared_ptr<Value>;

