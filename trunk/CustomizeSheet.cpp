// CustomizeSheet.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CustomizeSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

IMPLEMENT_DYNAMIC(CCustomizeSheet, CPropertySheet)

CCustomizeSheet::CCustomizeSheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION15, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	// AddPage(&m_Page2);
}

CCustomizeSheet::~CCustomizeSheet()
{
}


BEGIN_MESSAGE_MAP(CCustomizeSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCustomizeSheet)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet message handlers



BOOL CCustomizeSheet::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();
	
	// TODO: Add extra initialization here	
    LONG style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
    style |= WS_EX_CONTEXTHELP;
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, style);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCustomizeSheet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    if (pHelpInfo->iContextType == HELPINFO_WINDOW)
    {
		// do nothing for context help
		return TRUE;
	}	
	return CPropertySheet::OnHelpInfo(pHelpInfo);
}
