#include <iostream>
#include <memory>
#include <vector>

#include "aClass.hpp"

using namespace std;

int main() 
{
    int theInt = -1;
    PropertyRW<int> aProp(theInt);
    aProp = 42;
    cout << "int property: " << aProp << endl;  
    aProp += 42;  
    cout << "int property: " << aProp << endl; 
    theInt -= aProp;
    cout << "int property: " << aProp << endl; 
    if(!aProp || aProp)
    {
        cout << boolalpha << "aProp is: ";
        cout << static_cast<bool>(aProp) << endl;        
    }
    

    istringstream  is("42");   
    is >> aProp;
    cout << "int property: " << aProp << endl; 

    aOtherClass ao;
    cout << "Name of other class: " << ao.Name << endl;

    shared_ptr<aClass> a = make_shared<aClass>();
    cout << a->toString();

    aValueImpl<double> pi = 3.14;
    aValueImpl<bool> itsTrue = true;
    aValueImpl<string> aText = string("Thats my Text");

    PropertyR piProp(pi);

    std::vector<aValue *> allValues = {&pi, &itsTrue, &aText};
    for (aValue *  value : allValues)
    {
        if (value->canConvert(aValueType::vtString))
        {
            string pi_str = value->writeString();
        }
    }
    return 0;
}
