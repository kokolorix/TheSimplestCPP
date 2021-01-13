#include "dbg_new.h"
#include "Progress.h"
#include <CommCtrl.h>

using namespace std;

extern HINSTANCE hInstance;
Control::ControlManagerImpl<Progress> Progress::Manager;

struct  Progress::Impl
{
	Impl(Progress* control)	: control_(control) {}
	Progress* control_;

	int16_t minRange_ = 0;
	void set_minRange(int16_t minRange)
	{
		minRange_ = minRange;
		::PostMessage(control_->hWnd, PBM_SETRANGE, 0, MAKELPARAM(minRange_, maxRange_));
	}

	int16_t maxRange_ = 100;
	void set_maxRange(int16_t maxRange) 
	{
		maxRange_ = maxRange;
		::PostMessage(control_->hWnd, PBM_SETRANGE, 0, MAKELPARAM(minRange_, maxRange_));
	}

	int16_t step_ = 10;
	void set_step(int16_t step) 
	{
		step_ = step;
	 ::PostMessage(control_->hWnd, PBM_SETSTEP, (WPARAM)step_, 0);
	}

	int16_t pos_ = 0;
	void set_pos(int16_t pos) 
	{
		pos_ = pos;
		::PostMessage(control_->hWnd, PBM_SETPOS, (WPARAM)pos_, 0);
	}

	void stepIt() 
	{
		::PostMessage(control_->hWnd, PBM_STEPIT, 0, 0);
	}
};

Progress::Progress()
	: pImpl_(make_unique<Progress::Impl>(this))
	, MinRange(pImpl_->minRange_, bind(&Impl::set_minRange, pImpl_.get(), _1))
	, MaxRange(pImpl_->maxRange_, bind(&Impl::set_maxRange, pImpl_.get(), _1))
	, Step(pImpl_->step_, bind(&Impl::set_step, pImpl_.get(), _1))
	, Pos(pImpl_->pos_, bind(&Impl::set_pos, pImpl_.get(), _1))
{

}

Progress::~Progress()
{

}

HWND Progress::create(HWND hParent, int x, int y, int cx, int cy)
{
	HWND hWnd = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE,
		x,  y, cx, cy,
		hParent, (HMENU)0, ::hInstance, NULL);

	insert(hWnd);

	return hWnd;
}

void Progress::stepIt()
{
	pImpl_->stepIt();
}
