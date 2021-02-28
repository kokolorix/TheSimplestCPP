#pragma once

#include "Control.h"

class Button;
using ButtonPtr = shared_ptr<Button>;

class Button : public  Control
{ 
public:
    Button();
    virtual ~Button();

    static Control::ControlManagerImpl<Button> Manager;

    HWND create(HWND hParent, int x, int y, int cx, int cy, string caption);
	 virtual bool execute(int command);

private:
    struct Impl;
    unique_ptr<Impl> pImpl_;

public:
    PropertyRW<function<void(Button*)>> OnClicked;
};