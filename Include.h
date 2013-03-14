// Include.h : header file
//
// This class defines custom modal property sheet
// CInclude.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_H__)
#define __INCLUDE_H__

#include "IncludePg1.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Include.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInclude

#include "OleCtlKeywordSheet.h"
#include "TreeCtrlIn.h"

#ifndef baseInclude
#define baseInclude COleCtlKeywordSheet
#endif


class CInclude : public baseInclude
{
	DECLARE_DYNAMIC(CInclude)

// Construction
public:
	CInclude(CWnd* pWndParent = NULL);

// Attributes
public:
	CIncludePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInclude)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();

	virtual void Edit(CString& rStr);

	virtual ~CInclude();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInclude)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__INCLUDE_H__)
