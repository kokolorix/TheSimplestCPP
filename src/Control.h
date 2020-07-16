#pragma once

#include "windows.h"

#include <string>
using std::string;

#include <memory>
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

#include "Property.hpp"

class Control;
using ControlPtr = shared_ptr<Control>;

class Control
{
public:
    Control();
    virtual ~Control();

    static struct ControlManager
    {
        ControlManager();
        virtual ~ControlManager();
        ControlPtr operator[](const string &name);
        ControlPtr operator[](HWND hWnd);

    protected:        
        ControlPtr& get(const string& name);

    private:
        struct Impl;
        unique_ptr<Impl> pImpl_;
        friend class Control;
    } Manager;

    virtual HWND create(HWND hParent, int x, int y, int cx, int cy, string windowClass, string caption , int windowStyle);
    virtual void execute(int command);

private:
    struct Impl;
    unique_ptr<Impl> pImpl_;

public:
    PropertyRW<HWND> hWnd;
    PropertyRW<string> Caption;
};