// AccelPropertySheet.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "AccelPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccelPropertySheet

IMPLEMENT_DYNAMIC(CAccelPropertySheet, baseAccelPropertySheet)

CAccelPropertySheet::CAccelPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseAccelPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CAccelPropertySheet::CAccelPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseAccelPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CAccelPropertySheet::~CAccelPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CAccelPropertySheet, baseAccelPropertySheet)
	//{{AFX_MSG_MAP(CAccelPropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccelPropertySheet message handlers

BOOL CAccelPropertySheet::PreTranslateMessage(MSG* pMsg) 
{
	// Handle accelerator keys
	if (pMsg->message == WM_SYSKEYDOWN)
	{
		TCHAR buffer[100];
		CTabCtrl* pTabCtrl =  GetTabControl();
		// load tab accelerators
		// Won't handle pages added on the fly
		if (pTabCtrl && pTabCtrl->GetSafeHwnd() && m_accel.IsEmpty())
		{
			CString str;
			int nTabs = pTabCtrl->GetItemCount();
			TCITEM item;
			item.mask       = TCIF_TEXT;
			item.cchTextMax = 100;
			item.pszText    = buffer;
			int nPos;
			for (int i = 0; i < nTabs; ++i)
			{
				pTabCtrl->GetItem(i, &item);
				str = item.pszText;
				nPos = str.Find("&", 0);
				if (nPos != -1 && ((nPos + 1) < str.GetLength()))
				{
					int j = toupper(str.GetAt(nPos + 1));
					// make sure accelerator not already defined
					ASSERT(m_accel.Lookup(j, i) == FALSE);
					m_accel.SetAt(j, i);
				}
			}
		}
		if (!m_accel.IsEmpty())
		{
			int nPage;
			if (m_accel.Lookup((int)pMsg->wParam, nPage))
			{
				if (GetActiveIndex() != nPage)
					SetActivePage(nPage);
				return TRUE; /* handled */
			}
		}

	}

	// Set focus to next control on Ctrl+Tab
	// and to previous control on Shift+Ctrl+Tab
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_TAB :
			TRACE("CPropertySheetAccel::PreTranslateMessage\n");
			if (GetKeyState(VK_SHIFT) < 0)
			{
				GetActivePage()->PrevDlgCtrl();
			}
			else
			{
				GetActivePage()->NextDlgCtrl();
			}
			{
				CWnd* pWnd = GetFocus();
				if (pWnd != NULL)
					pWnd->SetFocus();
			}
			return TRUE; /* handled */
			break;

		default :
			return baseAccelPropertySheet::PreTranslateMessage(pMsg);
			break;
		}
	}
	
	return baseAccelPropertySheet::PreTranslateMessage(pMsg);
}
