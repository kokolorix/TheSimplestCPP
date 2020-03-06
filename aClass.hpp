#ifndef aClassHPP
#define aClassHPP

#include <memory>
using std::shared_ptr;

#include "aValue.hpp"
#include "aOtherClass.hpp"

/**
 * @brief aClass to do something
 * @details stores a int value and a pointer to aOtherClass
 */
class aClass : public aInterface
{
public:
    aClass();
    aClass(const aClass &) = default;
    virtual ~aClass();

    virtual string toString() const override;

private:
    aValueImpl<int> aInt_;                ///< A simple value member
    shared_ptr<aOtherClass> aMember_; ///< A little bit more complex member as shared _ptr
    aValueImpl<string> aString_;
};

#endif
