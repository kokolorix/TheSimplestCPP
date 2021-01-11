
#include "dbg_new.h"
#include <assert.h>
#include "Edit.h"
#include "Thread.h"
#include <memory>

using namespace std;

extern HINSTANCE hInstance;
Control::ControlManagerImpl<Edit> Edit::Manager;

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
   ThreadId mainId = mainThread->Id;
   if(this_thread::get_id() == mainId )
	{
		SendMessageA(hWnd, EM_SETSEL, -1, -1);
		SendMessageA(hWnd, EM_REPLACESEL, FALSE, (LPARAM)line.c_str());
	}
   else
   { 
		PostMessageA(hWnd, EM_SETSEL, -1, -1);
		PostMessageA(hWnd, EM_REPLACESEL, FALSE, (LPARAM)line.c_str());
   }
}
