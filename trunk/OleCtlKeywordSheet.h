// OleCtlKeywordSheet.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLECTLKEYWORDSHEET_H__DDCF4252_3933_4821_94C3_D3F68FBF8757__INCLUDED_)
#define AFX_OLECTLKEYWORDSHEET_H__DDCF4252_3933_4821_94C3_D3F68FBF8757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COleCtlKeywordSheet

#include "KeywordSheet.h"

#ifndef baseOleCtlKeywordSheet
#define baseOleCtlKeywordSheet CKeywordSheet
#endif

class COleCtlKeywordSheet : public baseOleCtlKeywordSheet
{
	DECLARE_DYNAMIC(COleCtlKeywordSheet)

// Construction
public:
	COleCtlKeywordSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COleCtlKeywordSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COleCtlKeywordSheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COleCtlKeywordSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(COleCtlKeywordSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLECTLKEYWORDSHEET_H__DDCF4252_3933_4821_94C3_D3F68FBF8757__INCLUDED_)
