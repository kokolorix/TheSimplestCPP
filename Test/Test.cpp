#include "Test.hpp"


namespace
{
bool test = addTestCase(std::make_shared<Test>(), "Test");
} // namespace

TestResult Test::runTest()
{
    return TestResult::Successful;
}
