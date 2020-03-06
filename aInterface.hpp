#ifndef aInterfaceHPP
#define aInterfaceHPP

#include <typeinfo>
#include <string>
#include <memory>

using std::string;
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
