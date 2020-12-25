
#include "dbg_new.h"
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
        size_t len = ::GetWindowTextLengthA(hWnd_);
        auto buf = unique_ptr<char[]>(new char[len + 1]);
        ::GetWindowTextA(hWnd_, buf.get(), (int)len);
        return string(buf.get());
    }
};

struct Control::ControlManager::Impl
{
	bool deleted_ = false;
	map<HWND, ControlPtr> hMap_;
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
	hWnd = NULL;
   if(!Control::Manager.pImpl_->deleted_)
	{
		auto begin = Control::Manager.pImpl_->nameMap_.begin();
		auto end = Control::Manager.pImpl_->nameMap_.end();
		auto it = find_if(begin, end, [this](pair<const string, ControlPtr>& p) {return p.second.get() == this; });
		if (it != end)
			Control::Manager.pImpl_->nameMap_.erase(it);
	}
}

HWND Control::create(HWND hParent, int x, int y, int cx, int cy,  string windowClass, string caption, int windowStyle)
{
    HWND hWnd = ::CreateWindowA(windowClass.c_str(),
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

    insert(hWnd);
    return hWnd;
}

void Control::insert(HWND hWnd)
{
   pImpl_->hWnd_ = hWnd;

	auto begin = Control::Manager.pImpl_->nameMap_.begin();
	auto end = Control::Manager.pImpl_->nameMap_.end();
	auto it = find_if(begin, end, [this](pair<const string, ControlPtr>& p) {return p.second.get() == this; });
	if (it != end)
		Control::Manager.pImpl_->hMap_[pImpl_->hWnd_] = it->second;
}

void Control::execute(int command)
{
    //switch(command)
    //{
    //}
}

Control::ControlManager::ControlManager() : pImpl_(make_unique<Control::ControlManager::Impl>()){}

Control::ControlManager::~ControlManager() { pImpl_->deleted_ = true; }

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
