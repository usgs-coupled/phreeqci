// ListBoxDrop.cpp : implementation file
//

#include "stdafx.h"
#include "ListBoxDrop.h"

// CListBoxDrop

IMPLEMENT_DYNAMIC(CListBoxDrop, CListBox)
CListBoxDrop::CListBoxDrop()
	: CListBox()
	, m_nCol(-1)
	, m_nRow(-1)
	, m_nLastChar(0)
{
}

CListBoxDrop::~CListBoxDrop()
{
}

BEGIN_MESSAGE_MAP(CListBoxDrop, CListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(LBDM_SETCAPTURE, OnSetCapture)
	ON_MESSAGE(LBDM_ENDEDITLABELNOW, OnEndEditLabelNow)
	ON_MESSAGE(LBDM_EDITLABEL, OnEditLabel)
	ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnLbnKillfocus)
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnLbnSetfocus)
END_MESSAGE_MAP()

// CListBoxDrop message handlers
void CListBoxDrop::OnMouseMove(UINT nFlags, CPoint point)
{
	// Capture mouse if not already
	if (CWnd::GetCapture() != this) {
		this->SetCapture();
		ASSERT(CWnd::GetCapture() == this);
	}

	BOOL bOutside;
	UINT nSelect = this->ItemFromPoint(point, bOutside);
	if (bOutside) {
		if (nFlags & MK_LBUTTON) {
			this->SetCurSel(-1);
		}
	}
	else {
		this->SetCurSel(nSelect);
	}

	// Add your message handler code here and/or call default

	CListBox::OnMouseMove(nFlags, point);
}

void CListBoxDrop::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Note: point is relative to client
	CRect rc;
	this->GetClientRect(&rc);
	if (rc.PtInRect(point)) {
		// this->EndEdit(FALSE);
	}
	else {
		this->EndEdit(TRUE);
	}

	// Add your message handler code here and/or call default

	CListBox::OnLButtonDown(nFlags, point);
}

void CListBoxDrop::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Note: point is relative to client
	CRect rc;
	this->GetClientRect(&rc);
	if (rc.PtInRect(point)) {
		this->EndEdit(FALSE);
	}
	else {
		this->EndEdit(TRUE);
	}

	// Add your message handler code here and/or call default

	CListBox::OnLButtonUp(nFlags, point);
}

void CListBoxDrop::OnCancelMode()
{
	CListBox::OnCancelMode();

	// Add your message handler code here

	this->EndEdit(TRUE);
}

LRESULT CListBoxDrop::OnSetCapture(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	ASSERT(wParam == 0);
	ASSERT(lParam == 0);
	ASSERT(this->IsWindowVisible());
	this->SetCapture();
	return 0;
}

void CListBoxDrop::EndEdit(BOOL bCancel)
{
	if (CWnd::GetCapture() == this) {
		::ReleaseCapture();
	}
	this->ShowWindow(SW_HIDE);

	if (!bCancel) {
		CString str;
		int nCurSel = this->GetCurSel();
		if (nCurSel != -1) {
			this->GetText(nCurSel, str);
		}
	 
		// Send Notification to parent
		GV_DISPINFO dispinfo;

		dispinfo.hdr.hwndFrom = GetSafeHwnd();
		dispinfo.hdr.idFrom   = GetDlgCtrlID();
		dispinfo.hdr.code     = GVN_ENDLABELEDIT;
	 
		dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
		dispinfo.item.row     = this->m_nRow;
		dispinfo.item.col     = this->m_nCol;
		dispinfo.item.szText  = str;
		dispinfo.item.lParam  = (LPARAM) this->m_nLastChar; 
	 
		CWnd* pOwner = GetOwner();
		ASSERT(::IsWindow(pOwner->GetSafeHwnd())); // this should no longer fail in wizard mode on back -> next
		if (::IsWindow(pOwner->GetSafeHwnd()))
		{
			pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo ); 
		}
	}
	ASSERT(CWnd::GetCapture() != this);
}

//LBDM_EDITLABEL
//    wParam = 0;
//    lParam = (LPARAM)(GV_DISPINFO*)pgvDispInfo;
//    Note: pgvDispInfo->item.lParam contains cell RECT
LRESULT CListBoxDrop::OnEditLabel(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	ASSERT(!wParam);  // must be zero
	ASSERT(lParam);   // GV_DISPINFO*
	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(::AfxIsValidAddress(pNMHDR, sizeof(NMHDR)));

	if (pNMHDR->code == LBDM_EDITLABEL) {
		GV_DISPINFO *pgvDispInfo = (GV_DISPINFO*)pNMHDR;
		ASSERT(::AfxIsValidAddress(pgvDispInfo, sizeof(GV_DISPINFO)));

		this->m_nRow = pgvDispInfo->item.row;
		this->m_nCol = pgvDispInfo->item.col;

		this->GetCurSel();
		this->SetTopIndex(0);

		int nSelect = this->FindStringExact(0, pgvDispInfo->item.szText);
		this->SetCurSel(nSelect);

		CRect* pRC = (CRect*)pgvDispInfo->item.lParam;
		ASSERT(::AfxIsValidAddress(pRC, sizeof(CRect)));

		this->Display(*pRC);
		return TRUE;
	}
	return FALSE;
}

//LBDM_ENDEDITLABELNOW
//    wParam = (WPARAM) (BOOL) fCancel;
//    lParam = 0;
LRESULT CListBoxDrop::OnEndEditLabelNow(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	ASSERT(!lParam);   // must be zero
	this->EndEdit((BOOL)wParam);
	return TRUE;
}

void CListBoxDrop::Display(const CRect& rc)
{
	static HWND hWndDesktopWindow = ::GetDesktopWindow();
	static RECT rcDW;

	int x, y, cx, cy;
	cx = rc.right - rc.left;
	cy = this->GetItemHeight(0) * this->GetCount() + 2;
	x = rc.left;

	::GetClientRect(hWndDesktopWindow, &rcDW);
	y = rc.top;
	if ((rc.top + rc.Height() + cy) > rcDW.bottom) {
		// show above
		y -= cy;
	}
	else {
		// show below
		y += rc.Height();
	}

	// place in front of all windows
	this->SetWindowPos(&wndTopMost, x, y, cx, cy, SWP_NOACTIVATE|SWP_SHOWWINDOW);
}

void CListBoxDrop::OnCaptureChanged(CWnd *pWnd)
{
	ASSERT(!pWnd || pWnd == this);

	// Add your message handler code here

	CListBox::OnCaptureChanged(pWnd);
}

void CListBoxDrop::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CListBox::OnShowWindow(bShow, nStatus);

	// Add your message handler code here

	ASSERT(bShow || CWnd::GetCapture() != this);
}

void CListBoxDrop::OnLbnKillfocus()
{
	// Add your control notification handler code here
	ASSERT(FALSE); // should never get focus
}

void CListBoxDrop::OnLbnSetfocus()
{
	// Add your control notification handler code here
	ASSERT(FALSE); // should never get focus
}
