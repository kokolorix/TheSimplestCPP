#include "Serialize.h"

#include <sstream>
using std::ostringstream;
using std::istringstream;

#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

/*!
 * \class Serialize
 *
 * \ingroup GroupName
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note 
 *
 * \author srebe
 *
 * \version 1.0
 *
 * \date Januar 2021
 *
 * Contact: user@company.com
 *
 */

string Serialize::writeString() const
{
	string str = toJson();
	 str.erase(remove_if(str.begin(), str.end(), std::isspace), str.end());	
	 return str;
}

void Serialize::readString(const string& str)
{
	fromJson(str);
}

string Serialize::toJson() const
{
	ostringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss); // Create an output archive
		oarchive(*this); // Write the data to the archive
	} // archive goes out of scope, ensuring all contents are flushed
	return ss.str();
}

void Serialize::fromJson(const string& json)
{
	istringstream ss(json);
	{
		cereal::JSONInputArchive iarchive(ss); // Create an input archive
		iarchive(*this);  // Read the data from the archive
	}
}

string Serialize::toXml() const
{
	ostringstream ss;
	{
		cereal::XMLOutputArchive oarchive(ss); // Create an output archive
		oarchive(*this); // Write the data to the archive
	} // archive goes out of scope, ensuring all contents are flushed
	return ss.str();
}

void Serialize::fromXml(const string& xml)
{
	istringstream ss(xml);
	{
		cereal::XMLInputArchive iarchive(ss); // Create an input archive
		iarchive(*this);  // Read the data from the archive
	}
}
