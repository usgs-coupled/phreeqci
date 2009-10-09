// KeywordSheet.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KeywordSheet.h"

#include "RichDocIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeywordSheet

IMPLEMENT_DYNAMIC(CKeywordSheet, baseCKeywordSheet)

CKeywordSheet::CKeywordSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseCKeywordSheet(nIDCaption, pParentWnd, iSelectPage), bNeedDatabase(m_bNeedDatabase), bContextHelp(m_bContextHelp)
{
	// Hide the Apply button
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_bNeedDatabase = true;
	m_bContextHelp = false;
}

CKeywordSheet::CKeywordSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseCKeywordSheet(pszCaption, pParentWnd, iSelectPage), bNeedDatabase(m_bNeedDatabase), bContextHelp(m_bContextHelp)
{
	// Hide the Apply button
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_bNeedDatabase = true;
	m_bContextHelp = false;
}

CKeywordSheet::~CKeywordSheet()
{
}

CString CKeywordSheet::s_strNewLine = _T("\r\n");

BEGIN_MESSAGE_MAP(CKeywordSheet, baseCKeywordSheet)
	//{{AFX_MSG_MAP(CKeywordSheet)
	ON_WM_CONTEXTMENU()
	ON_WM_HELPINFO()
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeywordSheet message handlers

CString CKeywordSheet::GetString()
{
	return CString(_T("# Not Currently Implemented\r\n"));
}

void CKeywordSheet::Edit(CString& rStr)
{
	UNUSED_ALWAYS(rStr);
	ASSERT(FALSE);
}

CRichDocIn* CKeywordSheet::GetActiveDocument() const
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMDIFrameWnd, pFrame);

	// Get the active MDI child window.
	CMDIChildWnd *pChild = pFrame->MDIGetActive();
	CDocument *pDoc = pChild->GetActiveDocument();
	ASSERT_KINDOF(CRichDocIn, pDoc);
	return (CRichDocIn*)pDoc;
}

const CDatabase& CKeywordSheet::GetDatabase() const
{
	return GetActiveDocument()->database;
}

void CKeywordSheet::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UNUSED(point);
	// Don't display context menu for Ok, Cancel, and Help
	switch (pWnd->GetDlgCtrlID())
	{
	case 1: // Ok
		break;
	case 2: // Cancel
		break;
	case 9: // Help
		break;
	default:
		Default();
		break;
	}	
}

BOOL CKeywordSheet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));

	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	if (m_bContextHelp)
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}


	switch (pHelpInfo->iCtrlId)
	{
	case 1: // Ok
		myPopup.pszText = _T("Closes this keyword and saves any changes you have made.");
		break;
	case 2: // Cancel
		myPopup.pszText = _T("Closes this keyword without saving any changes you have made.");
		break;
	case 9: // Help
		myPopup.pszText = _T("Click to display an overview of this keyword.");
		break;
	default:
		if (pHelpInfo->hItemHandle != ::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL)) // all except tabctrl (AFX_IDC_TAB_CONTROL)
		{
			return baseCKeywordSheet::OnHelpInfo(pHelpInfo);
		}
		return TRUE;
	}	
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;

}

int CKeywordSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseCKeywordSheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Add What's This? button to caption
	ModifyStyleEx(0, WS_EX_CONTEXTHELP);
	
	return 0;
}

void CKeywordSheet::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CONTEXTHELP)
	{
		// What's This? begin
		m_bContextHelp = true;
	}
	
	baseCKeywordSheet::OnSysCommand(nID, lParam);

	if (nID == SC_CONTEXTHELP)
	{
		// What's This? end
		m_bContextHelp = false;
	}
}
