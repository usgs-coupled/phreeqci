// CKSSolid_Solutions.h : header file
//
// This class defines custom modal property sheet
// CCKSSolid_Solutions.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __SOLIDSOLUTIONSSHEET_H__
#define __SOLIDSOLUTIONSSHEET_H__

#include "CKPSolid_SolutionsPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSSolid_Solutions

#include "CommonKeywordSheet.h"

#ifndef baseCKSSolid_Solutions
#define baseCKSSolid_Solutions CCommonKeywordSheet
#endif

class CCKSSolid_Solutions : public baseCKSSolid_Solutions
{
	DECLARE_DYNAMIC(CCKSSolid_Solutions)

// Construction
public:
	CCKSSolid_Solutions(CWnd* pWndParent = NULL);

// Attributes
public:
	CCKPSolid_SolutionsPg1 m_Page1;
	CCKPSolid_SolutionsPg2 m_Page2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSSolid_Solutions)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Edit(CString& rStr);
	virtual CString GetString();
	virtual ~CCKSSolid_Solutions();

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSSolid_Solutions)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __SOLIDSOLUTIONSSHEET_H__
