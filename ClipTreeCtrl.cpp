// ClipTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "ClipTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClipTreeCtrl

CClipTreeCtrl::CClipTreeCtrl()
{
	// accelerator table
	m_hAccelTable = ::LoadAccelerators(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_CLIPLISTBOX));
}

CClipTreeCtrl::~CClipTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CClipTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CClipTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_COPY, OnCopy)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClipTreeCtrl message handlers

void CClipTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
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
		if (GetSelectedItem() == NULL)
		{
			VERIFY(pPopup->EnableMenuItem((UINT)ID_COPY, MF_BYCOMMAND | MF_GRAYED) != -1);
		}

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CClipTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UNUSED(nFlags);
	HTREEITEM hItem = HitTest(point);
	if (hItem != NULL)
	{
		SelectItem(hItem);
		SetFocus();
	}
	else
	{
		// The next line will cause OnContextMenu to NOT be called
		CTreeCtrl::OnRButtonDown(nFlags, point);
	}
}

void CClipTreeCtrl::OnCopy() 
{
	HTREEITEM hItem = GetSelectedItem();
	if (hItem ==  NULL) return;

	if (::OpenClipboard(m_hWnd))
	{
		// Retrieve string from ListBox
		CString strText = GetItemText(hItem);
		
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

void CClipTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);
	OnCopy();	
	*pResult = 0;
}

BOOL CClipTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators
		if (m_hAccelTable != NULL && ::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
