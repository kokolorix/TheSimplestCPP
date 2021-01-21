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

    HWND create(HWND hParent, int x, int y, int cx, int cy, string caption = string());

	 void addLine(const string& line);


private:
    struct Impl;
    unique_ptr<Impl> pImpl_;	
	 
public:
	PropertyRW<bool> ReadOnly;
	PropertyRW<bool> ClientEdge;
	PropertyRW<bool>  HScroll;
	PropertyRW<bool> VSCroll;
	PropertyRW<uint32_t> Margins;
	PropertyRW<uint32_t> LeftMargin;
	PropertyRW<uint32_t> TopMargin;
	PropertyRW<uint32_t> RightMargin;
	PropertyRW<uint32_t> BottomMargin;
};