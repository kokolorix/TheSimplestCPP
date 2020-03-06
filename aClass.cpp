#include <sstream>
using std::ostringstream;

#include "aClass.hpp"

aClass::aClass()
    : aInt_(42), aString_("some value"), aMember_(make_shared<aOtherClass>())
{
    // aMember_->se
}

aClass::~aClass()
{
}

string aClass::toString() const
{
    ostringstream os;
    string s = aMember_->Name;
    os << aInterface::toString() << ", int: " << aInt_.get_value();
    return os.str();
}
