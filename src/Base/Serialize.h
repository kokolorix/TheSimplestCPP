#pragma once
#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

/**
 * @brief Serialize is the interface for objects
 * which can be persisted in a simple string,
 * as JSON or XML structure.
 * 
 */
class Serialize
{
public:
	virtual ~Serialize() {}

	virtual string writeString() const = 0;
	virtual void readString(const string& str) = 0;

	virtual string toJson() const = 0;
	virtual void fromJson(const string& json) = 0;

	virtual string toXml() const = 0;
	virtual void fromXml(const string& xml) = 0;
};

using SerializePtr = shared_ptr<Serialize>;

