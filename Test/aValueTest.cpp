#include "aValueTest.hpp"
#include "aClass.hpp"
#include "aOtherClass.hpp"
#include "aValue.hpp"

namespace
{
bool test = addTestCase(std::make_shared<aValueTest>(), "aValueTest");
} // namespace

TestResult aValueTest::runTest()
{
    aOtherClass ao;
    out << "Name of other class: " << ao.Name << endl;

    shared_ptr<aClass> a = make_shared<aClass>();
    out << a->toString() << endl;

    aValueImpl<double> pi = 3.14;
    // out << "pi: " << pi << endl;                
    
    aValueImpl<bool> itsTrue = true;
    // out << "itsTrue: " << itsTrue << endl;

    aValueImpl<string> aText = string("Thats my Text");
    // out << "aText: "  << aText << endl;   


    std::vector<aValue *> allValues = {&pi, &itsTrue, &aText};
    for (aValue *value : allValues)
    {
        if (value->canConvert(aValueType::vtString))
        {
            string pi_str = value->writeString();
        }
    }
    return TestResult::Successful;
}
