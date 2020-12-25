
#include "dbg_new.h"
#include <assert.h>
#include "Edit.h"
#include <memory>

using namespace std;

extern HINSTANCE hInstance;

Edit::EditManager Edit::Manager;

struct  Edit::Impl
{
    function<void(Edit*)> onClicked_;
};

Edit::Edit() 
: pImpl_(make_unique<Edit::Impl>())
{
}

Edit::~Edit()
{

}

HWND Edit::create(HWND hParent, int x, int y, int cx, int cy)
{
    return Control::create(hParent,
                           x,
                           y,
                           cx,
                           cy,
                           "Edit",
                           "",
		                     WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL
                           );
}

void Edit::addLine(const string& line)
{
   SendMessageA(hWnd, EM_SETSEL, -1, -1);
   SendMessageA(hWnd, EM_REPLACESEL, FALSE, (LPARAM)line.c_str());
}

Edit::EditManager::EditManager() { }  // : pImpl_(make_unique<Edit::EditManager::Impl>()){}

EditPtr Edit::EditManager::operator[](const string &name)
{
    ControlPtr &control = get(name);
    if (control)
    {
        EditPtr edit = dynamic_pointer_cast<Edit>(control);
        assert(edit);
        return edit;
    }
    else
    {
        EditPtr edit = make_shared<Edit>();
        control = edit;
        return edit;
    }
}
EditPtr Edit::EditManager::operator[](HWND hWnd)
{
    EditPtr edit = dynamic_pointer_cast<Edit>(Control::Manager[hWnd]);
    return edit;
}
