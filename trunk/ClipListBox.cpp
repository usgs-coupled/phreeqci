// ClipListBox.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "ClipListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClipListBox

CClipListBox::CClipListBox()
{
	// accelerator table
	m_hAccelTable = ::LoadAccelerators(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_CLIPLISTBOX));
}

CClipListBox::~CClipListBox()
{
}


BEGIN_MESSAGE_MAP(CClipListBox, CListBox)
	//{{AFX_MSG_MAP(CClipListBox)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(LBN_DBLCLK, OnDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClipListBox message handlers


BOOL CClipListBox::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators
		if (m_hAccelTable != NULL && ::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}

void CClipListBox::OnCopy() 
{
	int nCurSel = GetCurSel();
	if (nCurSel == LB_ERR) return;

	if (::OpenClipboard(m_hWnd))
	{
		// Retrieve string from ListBox
		CString strText;
		GetText(nCurSel, strText);

		// Allocate global memory
		HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_DDESHARE, (strText.GetLength() + 1) * sizeof(TCHAR)); 

		if (hglbCopy)
		{
			// Lock it and get a pointer to it
			LPTSTR lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy);

			if (lptstrCopy)
			{
				// Empty existing clipboard contents
				VERIFY(::EmptyClipboard());

				// Copy string into memory
				VERIFY(::lstrcpy(lptstrCopy, (LPCTSTR)strText));

				// Unlock memory
				VERIFY(::GlobalUnlock(hglbCopy));

				// Send memory to clipboard
#ifdef UNICODE
				VERIFY(::SetClipboardData(CF_UNICODETEXT, hglbCopy));
#else
				VERIFY(::SetClipboardData(CF_TEXT, hglbCopy));
#endif

			}
			else
			{
				// Free memory if it couldn't be locked
				::GlobalFree(hglbCopy);
			}

		}
		// Close the clipboard. 
		VERIFY(::CloseClipboard());
	}
}

void CClipListBox::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UNUSED(pWnd);
	ASSERT(pWnd == this);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_CLIPLISTBOX));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);

	if (pPopup)
	{
		if (point.x == -1 && point.y == -1)
		{
			// keystroke invocation
			point.x = point.y = 0;
			ClientToScreen(&point);
		}

		// Note: All commands except copy are grayed by default
		// disable copy item if there's no selection
		if (GetCurSel() == LB_ERR)
		{
			VERIFY(pPopup->EnableMenuItem((UINT)ID_COPY, MF_BYCOMMAND | MF_GRAYED) != -1);
		}

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

BOOL CClipListBox::OnDblclk() 
{
	OnCopy();
	return FALSE;	// let parent window be notified
}

void CClipListBox::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// Make like LButtonDown
	SendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
	CListBox::OnRButtonDown(nFlags, point);
}
