// CKSEquilibrium_Phases.h : header file
//
// This class defines custom modal property sheet 
// CCKSEquilibrium_Phases.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSEQUILIBRIUM5F_PHASES_H__
#define __CKSEQUILIBRIUM5F_PHASES_H__

#include "CKPEquilibrium_PhasesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSEquilibrium_Phases

#include "CommonKeywordSheet.h"

#ifndef baseCKSEquilibrium_Phases
#define baseCKSEquilibrium_Phases CCommonKeywordSheet
#endif

class CCKSEquilibrium_Phases : public baseCKSEquilibrium_Phases
{
	DECLARE_DYNAMIC(CCKSEquilibrium_Phases)

// Construction
public:
	CCKSEquilibrium_Phases(CWnd* pWndParent = NULL);

// Attributes
public:
	CCKPEquilibrium_PhasesPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSEquilibrium_Phases)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSEquilibrium_Phases();

	virtual CString GetString();
	virtual void Edit(CString& rStr);


// Generated message map functions
protected:
	//{{AFX_MSG(CCKSEquilibrium_Phases)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSEQUILIBRIUM5F_PHASES_H__
