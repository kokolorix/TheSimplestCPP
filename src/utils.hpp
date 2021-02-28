#pragma once

#include "output.hpp"

#include <exception>
using std::exception;

#define STRINGIZE(asString) STRINGIZE_A((asString))
#define STRINGIZE_A(arg) STRINGIZE_I arg
#define STRINGIZE_I(asString) #asString
#define S(asString)  STRINGIZE(asString)

#define LOG_SOURCE		__FILE__ "(" S(__LINE__) ",1): "	


#define AssertFail(message) \
 { log("%s", message); \
 throw exception(format("AssertFail: %s\t%s\t%d\t%s", "", __FILE__, __LINE__, (message)).c_str());	}

#define AssertTrue(condition, message) \
{ if(!(condition)) { LOG("%s, %s", (#condition), message); \
throw exception(format("AssertTrue: %s\t%s\t%d\t%s", "", __FILE__, __LINE__, (message)).c_str()); }  }

#define AssertFalse(condition, message) \
{ if((condition)) { LOG("%s, %s", (#condition), message); \
throw exception(format("AssertFalse: %s\t%s\t%d\t%s", "", __FILE__, __LINE__, (message)).c_str()); }  }

//#define AssertNULL(condition, message) { if((condition) != NULL) { DETAILED_LOG((#condition), message); throw exception("AssertNULL", #condition, __FILE__, __LINE__, (message)); }  }
//#define AssertNotNULL(condition, message) { if((condition) == NULL) { DETAILED_LOG((#condition), message); throw exception("AssertNotNULL", #condition, __FILE__, __LINE__, (message)); }  }
