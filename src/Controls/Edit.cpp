
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
	DWORD wsStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL;
	DWORD wsExStyle = WS_EX_CLIENTEDGE;

	HWND hWnd = ::CreateWindowExA(
		wsExStyle,
		"Edit",
		"",
		wsStyle ,
		x,
		y,
		cx,
		cy,
		hParent,
		0,
		hInstance,
		NULL);

	RECT rc;
	SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rc);
	rc.left += 2;
	rc.top += 2;
	rc.right -= 2;
	rc.bottom -= 2;
	SendMessage(hWnd, EM_SETRECT, 0, (LPARAM)&rc);

	insert(hWnd);
	return hWnd;
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
      HWND h = hWnd;
      mainThread->call([h, line]() {
		      SendMessageA(h, EM_SETSEL, -1, -1);
		      SendMessageA(h, EM_REPLACESEL, FALSE, (LPARAM)line.c_str());
         });
		//PostMessageA(hWnd, EM_SETSEL, -1, -1);
		//PostMessageA(hWnd, EM_REPLACESEL, FALSE, (LPARAM)line.c_str());
   }
}
