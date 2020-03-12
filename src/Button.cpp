
#include "Button.h"

#include <map>
using std::map;

extern HINSTANCE hInstance;

Button::ButtonManager Button::Manager;

struct  Button::Impl
{
};

Button::Button() : pImpl_(make_unique<Button::Impl>())
{

}

Button::~Button()
{

}

HWND Button::create(HWND hParent, int x, int y, int cx, int cy)
{
    MSG msg = {0};
    WNDCLASS wc = {0};
    // wc.lpfnWndProc = WndProc;
    // wc.Instance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = L" BUTTON";

    if (!::RegisterClass(&wc))
        return  NULL;

    HWND hWnd = ::CreateWindow(L"BUTTON",
                               L"Start ...",
                               WS_CHILD | WS_VISIBLE,
                               x,
                               y,
                               cx,
                               cy,
                               hParent,
                               0,
                               hInstance,
                               NULL);
    return hWnd;
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
