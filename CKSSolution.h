// CKSSolution.h : header file
//
// This class defines custom modal property sheet 
// CCKSSolution.
//
// $Date: 2000/10/04 20:17:36 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSSOLUTION_H__
#define __CKSSOLUTION_H__

#include "KPSolutionPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSSolution

#include "CommonKeywordSheet.h"

#ifndef baseCKSSolution
#define baseCKSSolution CCommonKeywordSheet
#endif

class CCKSSolution : public baseCKSSolution
{
	DECLARE_DYNAMIC(CCKSSolution)

// Construction
public:
	CCKSSolution(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSolutionPg1 m_Page1;
	CKPSolutionPg2 m_Page2;
	CKPSolutionPg3 m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSSolution)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSSolution();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSSolution)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSSOLUTION_H__
