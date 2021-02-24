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
#include <assert.h>

class Control;
using ControlPtr = shared_ptr<Control>;

const LRESULT LRES_PROCESSED = -1; // 0xFFFFFFFFFFFFFFFF

class Control
{
public:
    Control();
    virtual ~Control();

    static struct ControlManager
    {
        ControlManager();
        virtual ~ControlManager();
        ControlPtr operator[](const string &name) const;
        ControlPtr operator[](HWND hWnd) const;

    protected:        
        ControlPtr& get(const string& name);

    private:
        struct Impl;
        unique_ptr<Impl> pImpl_;
        friend class Control;
    } Manager;

    virtual HWND create(HWND hParent, int x, int y, int cx, int cy, string windowClass, string caption , int windowStyle);
    virtual void insert(HWND hWnd);
    virtual bool execute(int command);

    protected:
    template<typename CT>
    struct ControlManagerImpl : public ControlManager
    {
        std::shared_ptr<CT> operator[](const string &name)
        {
            ControlPtr &ctrl = Control::Manager.get(name);
            if (ctrl)
            {
                std::shared_ptr<CT> impl = dynamic_pointer_cast<CT>(ctrl);
                assert(impl);
                return impl;
            }
            else
            {
                std::shared_ptr<CT> impl = make_shared<CT>();
                ctrl = dynamic_pointer_cast<Control>(impl);
                return impl;
            }
        }
        std::shared_ptr<CT> operator[](HWND hWnd)
        {
            std::shared_ptr<CT> impl = dynamic_pointer_cast<CT>(Control::Manager[hWnd]);
            return impl;
        }
    };

    virtual LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
    struct Impl;
    unique_ptr<Impl> pImpl_;

public:
    PropertyRW<HWND> hWnd;
    PropertyRW<string> Caption;
};