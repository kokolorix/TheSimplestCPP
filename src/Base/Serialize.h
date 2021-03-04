#pragma once
#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <cereal/types/polymorphic.hpp>


//#define BOOST_JSON_STANDALONE
//#include <boost\json.hpp>

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

	template <class Archive>
	void serialize(Archive&) 
	{
	}

	virtual string writeString() const;
	virtual void readString(const string& str);

	virtual string toJson() const;
	virtual void fromJson(const string& json);

	virtual string toXml() const;
	virtual void fromXml(const string& xml);
};

using SerializePtr = shared_ptr<Serialize>;

