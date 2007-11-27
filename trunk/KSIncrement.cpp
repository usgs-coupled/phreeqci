// KSIncrement.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSIncrement.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSIncrement

IMPLEMENT_DYNAMIC(CKSIncrement, baseCKSIncrement)

CKSIncrement::CKSIncrement(CWnd* pWndParent)
	 : baseCKSIncrement(IDS_PROPSHT_CAPTION20, pWndParent)
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

CKSIncrement::~CKSIncrement()
{
}


BEGIN_MESSAGE_MAP(CKSIncrement, baseCKSIncrement)
	//{{AFX_MSG_MAP(CKSIncrement)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSIncrement message handlers


CString CKSIncrement::GetString()
{
	/*
	Line 0:  INCREMENTAL_REACTIONS True
	*/

	// Line 0
	CString strLines;
	if (m_Page1.m_bTrueOrFalse)
	{
		strLines = _T("INCREMENTAL_REACTIONS True");
	}
	else
	{
		strLines = _T("INCREMENTAL_REACTIONS False");
	}

	return strLines + s_strNewLine;
}

void CKSIncrement::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	m_Page1.m_bTrueOrFalse = (incremental_reactions > 0) ? TRUE : FALSE;
}
