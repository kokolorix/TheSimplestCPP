#pragma once
#include <string>
using std::string;

class Description
{
	  virtual ~Description() {}

	  virtual string summary() = 0;
	  virtual string detailed() = 0;
};

