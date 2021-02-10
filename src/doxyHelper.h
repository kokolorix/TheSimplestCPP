/**
 * @file doxyHelper.h
 * This file is for doxygens pure operator handling
 * Every shared_ptr alias must faked here as own class 
 * with -> operator to make call graphs and references 
 * correct
 */

class Thread;
struct ThreadPtr
{
    Thread* ptr_;
    Thread *operator->() const { return ptr_; }
    Thread *operator&() { return ptr_; }
    Thread &operator*() { return *ptr_; }
};

class Control;
struct ControlPtr
{
    Control* ptr_;
    Control *operator->() const { return ptr_; }
    Control *operator&() { return ptr_; }
    Control &operator*() { return *ptr_; }
};

class Button;
struct ButtonPtr
{
    Button* ptr_;
    Button *operator->() const { return ptr_; }
    Button *operator&() { return ptr_; }
    Button &operator*() { return *ptr_; }
};

class Edit;
struct EditPtr
{
    Edit* ptr_;
    Edit *operator->() const { return ptr_; }
    Edit *operator&() { return ptr_; }
    Edit &operator*() { return *ptr_; }
};