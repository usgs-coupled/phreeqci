// DropDownButton.cpp : implementation file
//

#include "stdafx.h"
///#include "GridCtrlDemo.h"
#include "DropDownButton.h"

#include "GridCtrl.h"

// CDropDownButton

IMPLEMENT_DYNAMIC(CDropDownButton, CButton)
CDropDownButton::CDropDownButton()
: m_pListBoxDropDown(0)
{
}

CDropDownButton::~CDropDownButton()
{
	delete this->m_pListBoxDropDown;
}


BEGIN_MESSAGE_MAP(CDropDownButton, CButton)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_INPLACE_CONTROL, OnEndInPlaceEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CDropDownButton message handlers


UINT CDropDownButton::OnGetDlgCode()
{
	// Add your message handler code here and/or call default
	return CButton::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTCHARS;
}

void CDropDownButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Add your message handler code here and/or call default
	if (this->m_pListBoxDropDown->IsWindowVisible()) {
		this->m_pListBoxDropDown->SendMessage(WM_KEYDOWN, nChar, MAKEWORD(nRepCnt, nFlags));
		return;
		//switch (nChar) {
		//	case VK_PRIOR: // Page Down
		//	case VK_NEXT:  // Page Up
		//	case VK_END:
		//	case VK_HOME:
		//	case VK_LEFT:
		//	case VK_UP:
		//	case VK_RIGHT:
		//	case VK_DOWN:
		//		this->m_pListBoxDropDown->SendMessage(WM_KEYDOWN, nChar, MAKEWORD(nRepCnt, nFlags));
		//		break;
		//}
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDropDownButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CString strText;
	this->GetWindowText(strText);

	CRect rc;
	this->GetWindowRect(&rc); // screen coordinates

	GV_DISPINFO dispinfo;    
	dispinfo.hdr.hwndFrom = GetSafeHwnd();
	dispinfo.hdr.idFrom   = GetDlgCtrlID();
	dispinfo.hdr.code     = LBDM_EDITLABEL;    
	dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
	dispinfo.item.row     = 0;
	dispinfo.item.col     = 0;
	dispinfo.item.szText  = strText;
	dispinfo.item.lParam  = (LPARAM)&rc; // screen coordinates

	this->m_pListBoxDropDown->SendMessage(LBDM_EDITLABEL, 0, (LPARAM)&dispinfo);
	ASSERT(this->m_pListBoxDropDown->IsWindowVisible());

	this->m_pListBoxDropDown->GetWindowRect(&this->m_rcListBox); // screen coordinates
	this->ScreenToClient(&this->m_rcListBox);

	// Add your message handler code here and/or call default

	CButton::OnLButtonDown(nFlags, point);
}

void CDropDownButton::PreSubclassWindow()
{
	// Add your specialized code here and/or call the base class

	this->SetWindowText(_T(""));

	ASSERT(this->m_pListBoxDropDown == 0);
	this->m_pListBoxDropDown = new CListBoxDrop();
	ASSERT(this->m_pListBoxDropDown);

	CRect rect(0, 0, 0, 0);
	VERIFY(this->m_pListBoxDropDown->CreateEx(
		WS_EX_TOPMOST|WS_EX_TOOLWINDOW,
		_T("LISTBOX"),
		_T("CDropDownButton"),
		WS_CHILDWINDOW|WS_CLIPSIBLINGS|WS_BORDER,
		rect,
		CWnd::GetDesktopWindow(),
		IDC_INPLACE_CONTROL,
		NULL
		));

	this->m_pListBoxDropDown->SetFont(this->GetParent()->GetFont());

	this->m_pListBoxDropDown->SetOwner(this);
	ASSERT(this->m_pListBoxDropDown->GetOwner() == this);	
	ASSERT(this->m_pListBoxDropDown->GetParent() == CWnd::GetDesktopWindow());	

	this->m_pListBoxDropDown->AddString(_T("seconds"));
	this->m_pListBoxDropDown->AddString(_T("minutes"));
	this->m_pListBoxDropDown->AddString(_T("hours"));
	this->m_pListBoxDropDown->AddString(_T("days"));
	this->m_pListBoxDropDown->AddString(_T("years"));

	CButton::PreSubclassWindow();
}

void CDropDownButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	if (this->m_pListBoxDropDown->IsWindowVisible()) {
		this->m_pListBoxDropDown->SendMessage(LBDM_SETCAPTURE, 0, 0);
	}
	CButton::OnLButtonUp(nFlags, point);
}

void CDropDownButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// Add your message handler code here and/or call default
	// Note: point is relative to client
	if (nFlags & MK_LBUTTON && this->m_rcListBox.PtInRect(point)) {
		::ReleaseCapture();
	}

	CButton::OnMouseMove(nFlags, point);
}

BOOL CDropDownButton::PreTranslateMessage(MSG* pMsg)
{
	// Add your specialized code here and/or call the base class
	if (this->m_pListBoxDropDown->IsWindowVisible()) {
		// Esc
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
			return this->m_pListBoxDropDown->SendMessage(LBDM_ENDEDITLABELNOW, TRUE, 0);
		}

		// Enter
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
			return this->m_pListBoxDropDown->SendMessage(LBDM_ENDEDITLABELNOW, FALSE, 0);
		}

		// F4
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4) {
			return this->m_pListBoxDropDown->SendMessage(LBDM_ENDEDITLABELNOW, FALSE, 0);
		}

		// Alt+Down
		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_DOWN) {
			return this->m_pListBoxDropDown->SendMessage(LBDM_ENDEDITLABELNOW, FALSE, 0);
		}
	}
	else {
		// F4
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4) {
			CRect rc;
			this->GetClientRect(&rc);
			this->OnLButtonDown(0, rc.CenterPoint());
			this->OnLButtonUp(0, rc.CenterPoint());
			return TRUE;
		}

		// Alt+Down
		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_DOWN) {
			CRect rc;
			this->GetClientRect(&rc);
			this->OnLButtonDown(0, rc.CenterPoint());
			this->OnLButtonUp(0, rc.CenterPoint());
			return TRUE;
		}
	}

	return CButton::PreTranslateMessage(pMsg);
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CDropDownButton::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{    
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;

    // In case OnEndInPlaceEdit called as window is being destroyed
    if (!IsWindow(GetSafeHwnd()))
        return;

	this->SetWindowText(pgvItem->szText);

    *pResult = 0;
}

void CDropDownButton::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Add your message handler code here and/or call default
	if (m_pListBoxDropDown->IsWindowVisible()) {
		// Forward all WM_CHARS to list
		this->m_pListBoxDropDown->SendMessage(WM_CHAR, nChar, MAKEWORD(nRepCnt, nFlags));
		return;
	}

	CButton::OnChar(nChar, nRepCnt, nFlags);
}

void CDropDownButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	// Add your message handler code here
	if (m_pListBoxDropDown->IsWindowVisible()) {
		this->m_pListBoxDropDown->SendMessage(LBDM_ENDEDITLABELNOW, TRUE, 0);
	}
}
