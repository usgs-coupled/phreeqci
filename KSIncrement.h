// KSIncrement.h : header file
//
// $Date: 2001/08/20 20:36:16 $
// $Revision: 1.1.1.9 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSIncrement.
 
#ifndef __KSINCREMENT_H__
#define __KSINCREMENT_H__

#include "KPIncrementPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSIncrement

#include "KeywordSheet.h"

#ifndef baseCKSIncrement
#define baseCKSIncrement CKeywordSheet
#endif

class CKSIncrement : public baseCKSIncrement
{
	DECLARE_DYNAMIC(CKSIncrement)

// Construction
public:
	CKSIncrement(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPIncrementPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSIncrement)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSIncrement();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSIncrement)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSINCREMENT_H__
