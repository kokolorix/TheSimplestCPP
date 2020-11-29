#pragma once

#include "Control.h"

class Progress;
using ProgressPtr = shared_ptr<Progress>;

class Progress : public Control
{
public:
	Progress();
	virtual ~Progress();

	static struct ProgressManager : public ControlManager
	{
		ProgressManager();
		virtual ~ProgressManager() = default;
		ProgressPtr operator[](const string& name);
		ProgressPtr operator[](HWND hWnd);

	private:
		friend class Progress;
	} Manager;

	HWND create(HWND hParent, int x, int y, int cx, int cy);
	void stepIt();

private:
	struct Impl;
	unique_ptr<Impl> pImpl_;

public:
	PropertyRW<int16_t> MinRange;
	PropertyRW<int16_t> MaxRange;
	PropertyRW<int16_t> Step;
	PropertyRW<int16_t> Pos;
};

