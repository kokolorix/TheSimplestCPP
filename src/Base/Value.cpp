#include "Value.h"

Value::Value()
{

}

Value::~Value()
{

}

bool operator<(ValuePtr v1, ValuePtr v2)
{
	return false;// (*v1) < (*v2);
}
