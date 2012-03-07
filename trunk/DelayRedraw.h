#pragma once

class CDelayRedraw
{
public:
	CDelayRedraw(CWnd *pWnd);
	~CDelayRedraw(void);
protected:
	CWnd *Wnd;
	BOOL Visible;
};

inline CDelayRedraw::CDelayRedraw(CWnd *pWnd)
: Wnd(pWnd)
{
	this->Visible = this->Wnd && this->Wnd->IsWindowVisible();
	if (this->Visible)
	{
		if (this->Wnd) this->Wnd->SetRedraw(FALSE);
	}
}

inline CDelayRedraw::~CDelayRedraw(void)
{
	if (this->Visible)
	{
		if (this->Wnd)
		{
			this->Wnd->SetRedraw(TRUE);
			this->Wnd->SendMessage(WM_NCPAINT);  // scrollbars
			this->Wnd->RedrawWindow();
		}
	}
}
