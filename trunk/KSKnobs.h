// KSKnobs.h : header file
//
// This class defines custom modal property sheet 
// CKSKnobs.
 
#ifndef __KSKNOBS_H__
#define __KSKNOBS_H__

#include "KPKnobsPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSKnobs

#include "KeywordSheet.h"

#ifndef baseCKSKnobs
#define baseCKSKnobs CKeywordSheet
#endif

class CKSKnobs : public baseCKSKnobs
{
	DECLARE_DYNAMIC(CKSKnobs)

// Construction
public:
	CKSKnobs(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPKnobsPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSKnobs)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSKnobs();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSKnobs)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSKNOBS_H__
