#pragma once
#include <string>
using std::string;

class Description
{
public:
	virtual ~Description() {}

	virtual string summary() = 0;
	virtual string detailed() = 0;
};
