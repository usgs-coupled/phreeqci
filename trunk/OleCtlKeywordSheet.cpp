// OleCtlKeywordSheet.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "OleCtlKeywordSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleCtlKeywordSheet

IMPLEMENT_DYNAMIC(COleCtlKeywordSheet, baseOleCtlKeywordSheet)

COleCtlKeywordSheet::COleCtlKeywordSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseOleCtlKeywordSheet(nIDCaption, pParentWnd, iSelectPage)
{
	// this says that an ascendant of the CWnd contains a OLE control
	// and eliminates the need to override PreTranslateMessage on each page
	m_nFlags |= WF_OLECTLCONTAINER;
}

COleCtlKeywordSheet::COleCtlKeywordSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseOleCtlKeywordSheet(pszCaption, pParentWnd, iSelectPage)
{
	// this says that an ascendant of the CWnd contains a OLE control
	// and eliminates the need to override PreTranslateMessage on each page
	m_nFlags |= WF_OLECTLCONTAINER;
}

COleCtlKeywordSheet::~COleCtlKeywordSheet()
{
}


BEGIN_MESSAGE_MAP(COleCtlKeywordSheet, baseOleCtlKeywordSheet)
	//{{AFX_MSG_MAP(COleCtlKeywordSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleCtlKeywordSheet message handlers


