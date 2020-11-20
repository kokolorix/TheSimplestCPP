
#include "dbg_new.h"
#include <assert.h>
#include "Button.h"

using namespace std;

extern HINSTANCE hInstance;

Button::ButtonManager Button::Manager;

struct  Button::Impl
{
    function<void(Button*)> onClicked_;
};

Button::Button() 
: pImpl_(make_unique<Button::Impl>())
, OnClicked(pImpl_->onClicked_)
{
}

Button::~Button()
{

}

HWND Button::create(HWND hParent, int x, int y, int cx, int cy, string caption)
{
    return Control::create(hParent,
                           x,
                           y,
                           cx,
                           cy,
                           "BUTTON",
                           caption,
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                           );
}

void Button::execute(int command)
{
    switch(command)
    {
        case BN_CLICKED:
        OnClicked(this);
    }
}

Button::ButtonManager::ButtonManager() { }  // : pImpl_(make_unique<Button::ButtonManager::Impl>()){}

ButtonPtr Button::ButtonManager::operator[](const string &name)
{
    ControlPtr &control = get(name);
    if (control)
    {
        ButtonPtr button = dynamic_pointer_cast<Button>(control);
        assert(button);
        return button;
    }
    else
    {
        ButtonPtr button = make_shared<Button>();
        control = button;
        return button;
    }
}
ButtonPtr Button::ButtonManager::operator[](HWND hWnd)
{
    ButtonPtr button = dynamic_pointer_cast<Button>(Control::Manager[hWnd]);
    return button;
}
