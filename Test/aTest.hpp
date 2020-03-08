#ifndef aTestHPP
#define aTestHPP

#include "aInterface.hpp"
#include "Helpers.hpp"
#include "OutputStream.hpp"

enum struct TestResult
{
    Successful,
    Failed,
    Exception,
    Unexpected
};

struct aTestCase  : public aInterface
{
    virtual ~aTestCase() = default;
    virtual void setUp() {}
    virtual TestResult runTest() = 0;
    virtual void tearDown() {}
};
using aTestCasePtr = shared_ptr<aTestCase>;
bool addTestCase(aTestCasePtr pTest, const string &name);
TestResult runTests(const string &testPattern = "*");

#endif