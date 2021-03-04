#pragma once
#include "Serialize.h"

#include <sstream>
#include <stdint.h>
using std::ostringstream;
using std::istringstream;
using std::boolalpha;

class Value;
//using ValuePtr = shared_ptr<const Value>;
struct ValuePtr;

template<typename T>	class	ValueImpl;

/**
 * @brief Value is the base class for all
 * value types in the system 
 */
class Value	:
	public Serialize
{
public:
	Value();
	virtual ~Value();
	template <class Archive>
	void serialize(Archive&)
	{
	}

protected:
};
CEREAL_REGISTER_TYPE(Value)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Serialize, Value)

template<typename T>
class	ValueImpl;

/**
 * @brief ValuePtr is the smart pointer for thread-wide 
 * used values. 
 * @details
 * In order not to provoke incalculable side effects,
 * the value in the pointer is constant.
 * If a new value is assigned, a new instance is created. 
 */
struct ValuePtr : public shared_ptr<const Value>
{
	using BasePtr = shared_ptr<const Value>;
	ValuePtr() = default;
	ValuePtr(const ValuePtr&) = default;
	ValuePtr(const BasePtr& basePtr) : BasePtr(basePtr) {}

	template<typename T>
	ValuePtr(T value);

	template<typename T>
	operator shared_ptr<const ValueImpl<T>>()
	{
		return dynamic_pointer_cast<const ValueImpl<T>>(*this);
	}
};

/**
 * @brief The template ValueImpl is the concrete implementation 
 * of any value type.
 * @details
 * Special behavior in the serialize methods is realized
 * as specialization of template methods.
 */
template<typename T>
class	ValueImpl : public Value
{	
public:
	ValueImpl():value_(T()) {}
	ValueImpl(const ValueImpl& v) :value_(v.value_) {}
	
	ValueImpl(T v) : value_(v) { 	}

	ValueImpl& operator = (const ValueImpl& v) 
	{
		value_ = v.value_;
		return *this;
	}
	ValueImpl& operator = (T v)
	{
		value_ = v;
		return *this;
	}

	operator T() const { return value_; }

	template <class Archive>
	void serialize(Archive& a)
	{
		a(CEREAL_NVP(value_));
	}

private:
	T value_;
};


using StringValue = ValueImpl<string>;
using StringValuePtr = shared_ptr<const ValueImpl<string>>;
CEREAL_REGISTER_TYPE(StringValue)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Value, StringValue)

using BooleanValue = ValueImpl<bool>;
using BooleanValuePtr = shared_ptr<const ValueImpl<bool>>;
CEREAL_REGISTER_TYPE(BooleanValue)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Value, BooleanValue)

using Int32Value = ValueImpl<int32_t>;
using Int32ValuePtr = shared_ptr<const ValueImpl<int32_t>>;
CEREAL_REGISTER_TYPE(Int32Value)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Value, Int32Value)


template<typename T>
inline ValuePtr::ValuePtr(T value)
	: BasePtr(make_shared<const ValueImpl<T>>(value))
{

}

template<>
inline ValuePtr::ValuePtr(const char* value)
	: BasePtr(make_shared<const ValueImpl<string>>(value))
{

}

//template<typename T>
//inline ValuePtr::operator shared_ptr<const ValueImpl<T>>()
//{
//	return dynamic_pointer_cast<const ValueImpl<T>>(*this);
//}

