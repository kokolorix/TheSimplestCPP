
#include "dbg_new.h"
#include <assert.h>
#include "Button.h"

using namespace std;

extern HINSTANCE hInstance;
Control::ControlManagerImpl<Button> Button::Manager;

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
