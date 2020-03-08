/**
 * @file doxyHelper.h
 * This file is for doxygens pure operator handling
 * Every shared_ptr alias must faked here as own class 
 * with -> operator to make call graphs and references 
 * correct
 */

class aTestCase;
struct aTestCasePtr {
    aTestCase* ptr_ = nullptr;
    aTestCase* operator->()const{return ptr_;}
    aTestCase* operator&(){return ptr_;}
    aTestCase& operator*(){return *ptr_;}
};