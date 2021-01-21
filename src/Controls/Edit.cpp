
#include "dbg_new.h"
#include <assert.h>
#include "Edit.h"
#include "Thread.h"
#include <memory>
#include <algorithm>
#include <array>

using namespace std;

extern HINSTANCE hInstance;
Control::ControlManagerImpl<Edit> Edit::Manager;

struct  Edit::Impl
{	 
	 bool readOnly_ = false, clientedge_ = false, hscroll_ = false, vscroll_ = true;
	 uint32_t lm_ = 0, tm_ = 0, rm_ = 0, bm_ = 0;
	 uint32_t  getMargins() const;
	 void setMargins(uint32_t m);
};

uint32_t Edit::Impl::getMargins() const
{
	std::array<uint32_t,4> m = { lm_, tm_, rm_ , bm_};
	return *std::max_element(m.begin(), m.end());
}

void Edit::Impl::setMargins(uint32_t m)
{
	tm_ = lm_ = bm_ = rm_ = m;
}

Edit::Edit() 
: pImpl_(make_unique<Edit::Impl>())
, ReadOnly(pImpl_->readOnly_)
, ClientEdge(pImpl_->clientedge_)
, HScroll(pImpl_->hscroll_)
, VSCroll(pImpl_->vscroll_)
, LeftMargin(pImpl_->lm_)
, TopMargin(pImpl_->tm_)
, RightMargin(pImpl_->rm_)
, BottomMargin(pImpl_->bm_)
, Margins([this]() {return pImpl_->getMargins(); }, [this](uint32_t m) {pImpl_->setMargins(m); })
{
}

Edit::~Edit()
{

}

HWND Edit::create(HWND hParent, int x, int y, int cx, int cy, string caption /*= string()*/)
{
	DWORD wsStyle = WS_CHILD | WS_VISIBLE  | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL;
	if (VSCroll) wsStyle |= WS_VSCROLL;
	if (HScroll) wsStyle |= WS_HSCROLL;
	if (ReadOnly) wsStyle |= ES_READONLY;

	DWORD wsExStyle = 0;	
	if (ClientEdge)  wsExStyle |= WS_EX_CLIENTEDGE;

	HWND hWnd = ::CreateWindowExA(
		wsExStyle,
		"Edit",
		caption.c_str(),
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
	rc.left += LeftMargin;
	rc.top += TopMargin;
	rc.right -= RightMargin;
	rc.bottom -= BottomMargin;
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
