#pragma once

#include "Control.h"

class Edit;
using EditPtr = shared_ptr<Edit>;

class Edit : public  Control
{ 
public:
    Edit();
    virtual ~Edit();

	 static Control::ControlManagerImpl<Edit> Manager;

    HWND create(HWND hParent, int x, int y, int cx, int cy);

	 void addLine(const string& line);


private:
    struct Impl;
    unique_ptr<Impl> pImpl_;

public:
};