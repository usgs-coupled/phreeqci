// RichDocInSheet.cpp : implementation file
//
// $Date: 2000/12/19 22:55:44 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RichDocInSheet.h"

#include "phreeqci2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichDocInSheet

IMPLEMENT_DYNAMIC(CRichDocInSheet, baseRichDocInSheet)

CRichDocInSheet::CRichDocInSheet(CWnd* pWndParent)
	 : baseRichDocInSheet(IDS_PS_CAPTION_IN, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);

	// hide Apply button
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	// hide Help button
	m_psh.dwFlags &= ~PSH_HASHELP;
}

CRichDocInSheet::~CRichDocInSheet()
{
}


BEGIN_MESSAGE_MAP(CRichDocInSheet, baseRichDocInSheet)
	//{{AFX_MSG_MAP(CRichDocInSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRichDocInSheet message handlers



int CRichDocInSheet::DoModal() 
{
	int nDoModal = baseRichDocInSheet::DoModal();

	if (nDoModal == IDOK && m_Page1.m_bSetAsDefault)
	{
		// set and save default database if nec
		if (m_Page1.m_props.m_strDBPathName.CompareNoCase(((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName) != 0)
		{
			// set in app
			((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName = m_Page1.m_props.m_strDBPathName;
			// save in registry
			((CPhreeqciApp*)AfxGetApp())->SaveSettings();
		}
	}
	return nDoModal;
}
