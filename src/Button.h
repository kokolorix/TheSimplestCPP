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

class Button;
using ButtonPtr = shared_ptr<Button>;

class Button
{
public:
    Button();
    virtual ~Button();

    static struct ButtonManager
    {
        ButtonManager();
        virtual ~ButtonManager() = default;
        ButtonPtr operator[](const string &name);
        ButtonPtr operator[](HWND hWnd);

    private:
        struct Impl;
        unique_ptr<Impl> pImpl_;
        friend class Button;
    } Manager;

    HWND create(HWND hParent, int x, int y, int cx, int cy, string caption);
    void execute(int command);

private:
    struct Impl;
    unique_ptr<Impl> pImpl_;

public:
    PropertyRW<HWND> hWnd;
    PropertyRW<string> Caption;
    PropertyRW<function<void(Button*)>> OnClicked;
};