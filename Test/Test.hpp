#ifndef TestHPP
#define TestHPP

#include "aTest.hpp"

struct Test : public aTestCase
{
    virtual TestResult runTest() override;
};

#endif
