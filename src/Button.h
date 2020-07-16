#pragma once

#include "Control.h"

class Button;
using ButtonPtr = shared_ptr<Button>;

class Button : public  Control
{ 
public:
    Button();
    virtual ~Button();

    static struct ButtonManager : public ControlManager
    {
        ButtonManager();
        virtual ~ButtonManager() = default;
        ButtonPtr operator[](const string &name);
        ButtonPtr operator[](HWND hWnd);

    private:
        friend class Button;
    } Manager;

    HWND create(HWND hParent, int x, int y, int cx, int cy, string caption);
    void execute(int command);

private:
    struct Impl;
    unique_ptr<Impl> pImpl_;

public:
    PropertyRW<function<void(Button*)>> OnClicked;
};