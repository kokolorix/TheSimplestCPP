
#include "Button.h"

#include <map>
using std::map;

extern HINSTANCE hInstance;

Button::ButtonManager Button::Manager;

struct  Button::Impl
{
    HWND hWnd_;
    string caption_;
    void set_caption(const string& caption)
    {

    }
};

Button::Button() 
: pImpl_(make_unique<Button::Impl>())
, Caption(pImpl_->caption_, Impl::set_caption)
{
}

Button::~Button()
{

}

HWND Button::create(HWND hParent, int x, int y, int cx, int cy, string caption)
{
    pImpl_->hWnd = ::CreateWindow(L"BUTTON",
                               L"Start ...",
                               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                               x,
                               y,
                               cx,
                               cy,
                               hParent,
                               0,
                               hInstance,
                               NULL);
    return pImpl_->hWnd;
}

struct  Button::ButtonManager::Impl
{
	map<string, ButtonPtr> nameMap_;
};

Button::ButtonManager::ButtonManager() : pImpl_(make_unique<Button::ButtonManager::Impl>()){}

ButtonPtr Button::ButtonManager::operator[](const string &name)
{
    ButtonPtr &button = pImpl_->nameMap_[name];
    if (!button)
        button = make_shared<Button>();
    return button;
}
