// KSTitle.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSTitle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSTitle

IMPLEMENT_DYNAMIC(CKSTitle, baseKSTitle)

CKSTitle::CKSTitle(CWnd* pWndParent)
	 : baseKSTitle(IDS_PROPSHT_CAPTION1, pWndParent)
{
	m_bNeedDatabase = false;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CKSTitle::~CKSTitle()
{
}


BEGIN_MESSAGE_MAP(CKSTitle, baseKSTitle)
	//{{AFX_MSG_MAP(CKSTitle)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSTitle message handlers
CString CKSTitle::GetString()
{
	return CString(_T("TITLE ")) + m_Page1.m_strTitle + s_strNewLine;
}

void CKSTitle::Edit(CString& rStr)
{
	CWaitCursor wait;

	CString strTitle(rStr);

	strTitle.TrimLeft();
	strTitle.TrimRight();

	switch (rStr[0])
	{
	case _T('t') :
	case _T('T') :
		// remove title
		strTitle = strTitle.Mid(5);
		break;
	case _T('c') :
	case _T('C') :
		// remove comment
		strTitle = strTitle.Mid(7);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	strTitle.TrimLeft();

	m_Page1.m_strTitle = strTitle;
}

