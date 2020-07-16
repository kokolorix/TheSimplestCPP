
#include "Control.h"

#include <map>
#include <algorithm>
using namespace std;

extern HINSTANCE hInstance;

Control::ControlManager Control::Manager;

struct  Control::Impl
{
    HWND hWnd_;
    void set_caption(const string &caption)
    {
        ::SetWindowTextA(hWnd_, caption.c_str());
    }
    string get_caption()
    {
        int len = ::GetWindowTextLengthA(hWnd_);
        auto buf = unique_ptr<char[]>(new char[len + 1]);
        ::GetWindowTextA(hWnd_, buf.get(), len);
        return string(buf.get());
    }
};

struct Control::ControlManager::Impl
{
    map<HWND,ControlPtr> hMap_;  
	map<string, ControlPtr> nameMap_;
};

Control::Control() 
: pImpl_(make_unique<Control::Impl>())
, Caption(bind(&Impl::get_caption, pImpl_.get()), bind(&Impl::set_caption, pImpl_.get(), _1))
, hWnd(pImpl_->hWnd_)
{
}

Control::~Control()
{

}

HWND Control::create(HWND hParent, int x, int y, int cx, int cy,  string windowClass, string caption, int windowStyle)
{
    pImpl_->hWnd_ = ::CreateWindowA(windowClass.c_str(),
                                   caption.c_str(),
                                   windowStyle,
                                   x,
                                   y,
                                   cx,
                                   cy,
                                   hParent,
                                   0,
                                   hInstance,
                                   NULL);

    auto begin = Control::Manager.pImpl_->nameMap_.begin();
    auto end = Control::Manager.pImpl_->nameMap_.end();
    auto it = find_if(begin, end, [this](pair<const string, ControlPtr>& p ){return p.second.get() == this;});
    if(it != end)
        Control::Manager.pImpl_->hMap_[pImpl_->hWnd_] = it->second;

    return pImpl_->hWnd_;
}

void Control::execute(int command)
{
    //switch(command)
    //{
    //}
}

Control::ControlManager::ControlManager() : pImpl_(make_unique<Control::ControlManager::Impl>()){}

Control::ControlManager::~ControlManager() = default;

ControlPtr Control::ControlManager::operator[](const string &name)
{
    ControlPtr &control = Control::Manager.pImpl_->nameMap_[name];
    if (!control)
        control = make_shared<Control>();
    return control;
}
ControlPtr Control::ControlManager::operator[](HWND hWnd)
{
    ControlPtr &control = Control::Manager.pImpl_->hMap_[hWnd];
    if (!control)
        control = make_shared<Control>();
    return control;
}
ControlPtr& Control::ControlManager::get(const string &name)
{
    ControlPtr &control = Control::Manager.pImpl_->nameMap_[name];
    return control;
}
