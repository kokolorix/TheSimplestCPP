#include "PropertyTest.hpp"
#include "Property.hpp"
#include "aValue.hpp"

namespace
{
bool test = addTestCase(std::make_shared<PropertyTest>(), "PropertyTest");
} // namespace

TestResult PropertyTest::runTest()
{
    int theInt = -1;
    PropertyRW<int> aProp(theInt);
    aProp = 42;
    out << "int property: " << aProp << endl;
    aProp += 42;
    out << "int property: " << aProp << endl;
    theInt -= aProp;
    out << "int property: " << aProp << endl;
    if (!aProp || aProp)
    {
        out << boolalpha << "aProp is: ";
        out << static_cast<bool>(aProp) << endl;
    }

    istringstream is("42");
    is >> aProp;
    out << "int property: " << aProp << endl;

    aValueImpl<double> pi = 3.14;
    PropertyR<aValueImpl<double>> piProp(pi);
    // out << "piProp: " << piProp << endl;

    return TestResult::Successful;
}
