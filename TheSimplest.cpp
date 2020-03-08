#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <map>
using namespace std;

#include "Helpers.hpp"
#include "aTest.hpp"

namespace impl { extern AppArguments appArguments; }

int main(int argc, char* argv[])
{
	TestResult r = TestResult::Successful;
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			istringstream is(argv[i]);
			string key, val;
			getline(is, key, '=');
			getline(is, val);
			impl::appArguments.insert(std::make_pair(key,val));

			r = runTests(argv[i]);
			if (r != TestResult::Successful)
				return static_cast<int>(r);
		}
	}
	else
	{
		r = runTests();
	}
	return static_cast<int>(r);
}

// int main() 
// {
//     int theInt = -1;
//     PropertyRW<int> aProp(theInt);
//     aProp = 42;
//     cout << "int property: " << aProp << endl;  
//     aProp += 42;  
//     cout << "int property: " << aProp << endl; 
//     theInt -= aProp;
//     cout << "int property: " << aProp << endl; 
//     if(!aProp || aProp)
//     {
//         cout << boolalpha << "aProp is: ";
//         cout << static_cast<bool>(aProp) << endl;        
//     }
    

//     istringstream  is("42");   
//     is >> aProp;
//     cout << "int property: " << aProp << endl; 

//     aOtherClass ao;
//     cout << "Name of other class: " << ao.Name << endl;

//     shared_ptr<aClass> a = make_shared<aClass>();
//     cout << a->toString();

//     aValueImpl<double> pi = 3.14;
//     aValueImpl<bool> itsTrue = true;
//     aValueImpl<string> aText = string("Thats my Text");

//     PropertyR piProp(pi);

//     std::vector<aValue *> allValues = {&pi, &itsTrue, &aText};
//     for (aValue *  value : allValues)
//     {
//         if (value->canConvert(aValueType::vtString))
//         {
//             string pi_str = value->writeString();
//         }
//     }
//     return 0;
// }
