// KSPhases.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSPhases.
 
#ifndef __KSPHASES_H__
#define __KSPHASES_H__

#include "KPPhasesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSPhases
#include "OleCtlKeywordSheet.h"

#ifndef baseCKSPhases
#define baseCKSPhases COleCtlKeywordSheet
#endif

class CKSPhases : public baseCKSPhases
{
	DECLARE_DYNAMIC(CKSPhases)

// Construction
public:
	CKSPhases(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPPhasesPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSPhases)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSPhases();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSPhases)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSPHASES_H__
