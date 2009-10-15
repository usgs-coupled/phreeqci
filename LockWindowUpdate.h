#pragma once

class CLockWindowUpdate
{
public:
	CLockWindowUpdate(CWnd *pWnd);
	~CLockWindowUpdate(void);
protected:
	CWnd *Wnd;
};

inline CLockWindowUpdate::CLockWindowUpdate(CWnd *pWnd)
: Wnd(pWnd)
{
	if (this->Wnd)
	{
		VERIFY(this->Wnd->LockWindowUpdate());
	}
}

inline CLockWindowUpdate::~CLockWindowUpdate(void)
{
	if (this->Wnd)
	{
		this->Wnd->UnlockWindowUpdate();
	}
}


