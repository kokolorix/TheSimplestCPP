#pragma once
#include <string>
using std::string;

#include <memory>
using std::shared_ptr;


class Serialize
{
public:
	virtual ~Serialize() {}

	virtual string toJson() const = 0;
	virtual void fromJson(const string& json) = 0;

	virtual string toXml() const = 0;
	virtual void fromXml(const string& xml) = 0;
};

using SerializePtr = shared_ptr<Serialize>;

