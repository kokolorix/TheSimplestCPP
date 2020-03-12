#pragma once

#include "windows.h"

#include <string>
using std::string;

#include <memory>
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

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

    private:
        struct Impl;
        unique_ptr<Impl> pImpl_;
    } Manager;

    HWND create(HWND hParent, int x, int y, int cx, int cy);

private:
    struct Impl;
    unique_ptr<Impl> pImpl_;
};