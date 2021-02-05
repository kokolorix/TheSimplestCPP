#pragma once


#define STRINGIZE(asString) STRINGIZE_A((asString))
#define STRINGIZE_A(arg) STRINGIZE_I arg
#define STRINGIZE_I(asString) #asString
#define S(asString)  STRINGIZE((asString))
