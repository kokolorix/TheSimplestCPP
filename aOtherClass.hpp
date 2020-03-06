#ifndef aOtherClassHPP
#define aOtherClassHPP


#include "Property.hpp"
#include "aInterface.hpp"
#include "aValue.hpp"

class aOtherClass : public aInterface
{
private:
    aValueImpl<string> name_;
public:
    aOtherClass()
        : name_("Greatings from aOtherClass")
        , Name(name_)
        // , Name(bind(&aValueImpl<string&>::get_value, &name_), bind(&aValueImpl<string>::set_value, &name_, _1))
    {

    }

    PropertyRW<string> Name;

    void callToServer() const 
    {
        getServerStub()->toString();
    }

    static InterfacePtr getServerStub() 
    {
        return nullptr;
    }

    
};

#endif
