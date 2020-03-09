#ifndef aInterfaceHPP
#define aInterfaceHPP

#include <typeinfo>

#include <string>
using std::string;

#include <memory>
using std::make_shared;
using std::shared_ptr;


struct aInterface;
using InterfacePtr = shared_ptr<aInterface>;

struct aInterface
{
    virtual ~aInterface() {}
    virtual string toString() const
    {
        return typeid(*this).name(); ///> Returns a informative string represantation of this object
    }
};

#endif
