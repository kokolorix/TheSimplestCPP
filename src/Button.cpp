
#include "Button.h"

#include <map>
#include <algorithm>
using namespace std;

// #include <locale>
// #include <codecvt>
// #include <string>
// wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
// wstring wide = converter.from_bytes(caption);
// string narrow = converter.to_bytes(wide_utf16_source_string);

extern HINSTANCE hInstance;

Button::ButtonManager Button::Manager;

struct  Button::Impl
{
    HWND hWnd_;
    function<void(Button*)> onClicked_;
    void set_caption(const string &caption)
    {
        // wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
        // wstring wcaption = converter.from_bytes(caption);
        // ::SetWindowText(hWnd_, wcaption.c_str());
        ::SetWindowTextA(hWnd_, caption.c_str());
    }
    string get_caption()
    {
        // wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
        int len = ::GetWindowTextLengthA(hWnd_);
        auto buf = unique_ptr<char[]>(new char[len + 1]);
        ::GetWindowTextA(hWnd_, buf.get(), len);
        return string(buf.get());
        // string caption = converter.to_bytes(buf.get());
        // return caption;
    }
};

struct  Button::ButtonManager::Impl
{
    map<HWND,ButtonPtr> hMap_;  
	map<string, ButtonPtr> nameMap_;
};

Button::Button() 
: pImpl_(make_unique<Button::Impl>())
, Caption(bind(&Impl::get_caption, pImpl_.get()), bind(&Impl::set_caption, pImpl_.get(), _1))
, OnClicked(pImpl_->onClicked_)
, hWnd(pImpl_->hWnd_)
{
}

Button::~Button()
{

}

HWND Button::create(HWND hParent, int x, int y, int cx, int cy, string caption)
{
    // wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    // wstring wcaption = converter.from_bytes(caption);

    pImpl_->hWnd_ = ::CreateWindowA("BUTTON",
                                   caption.c_str(),
                                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                   x,
                                   y,
                                   cx,
                                   cy,
                                   hParent,
                                   0,
                                   hInstance,
                                   NULL);

    auto begin = Button::Manager.pImpl_->nameMap_.begin();
    auto end = Button::Manager.pImpl_->nameMap_.end();
    auto it = find_if(begin, end, [this](pair<const string, ButtonPtr>& p ){return p.second.get() == this;});
    if(it != end)
        Button::Manager.pImpl_->hMap_[pImpl_->hWnd_] = it->second;

    return pImpl_->hWnd_;
}

void Button::execute(int command)
{
    switch(command)
    {
        case BN_CLICKED:
        OnClicked(this);
    }
}

Button::ButtonManager::ButtonManager() : pImpl_(make_unique<Button::ButtonManager::Impl>()){}

ButtonPtr Button::ButtonManager::operator[](const string &name)
{
    ButtonPtr &button = pImpl_->nameMap_[name];
    if (!button)
        button = make_shared<Button>();
    return button;
}
ButtonPtr Button::ButtonManager::operator[](HWND hWnd)
{
    ButtonPtr &button = pImpl_->hMap_[hWnd];
    if (!button)
        button = make_shared<Button>();
    return button;
}
