// CKSInverse.h : header file
//
// This class defines custom modal property sheet 
// CCKSInverse.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSINVERSE_H__
#define __CKSINVERSE_H__

#include "KPInversePg1.h"
#include "DefinedRanges.h"


/////////////////////////////////////////////////////////////////////////////
// CCKSInverse
#include "CommonKeywordSheet.h"

#ifndef baseCKSInverse
#define baseCKSInverse CCommonKeywordSheet
#endif

class CCKSInverse : public baseCKSInverse
{
	DECLARE_DYNAMIC(CCKSInverse)

// Construction
public:
	CCKSInverse(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	CKPInversePg1A m_Page1A;
	CKPInversePg1B m_Page1B;
	CKPInversePg2 m_Page2;
	CKPInversePg3 m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSInverse)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSInverse();
	
	virtual CString GetString();
	virtual void Edit(CString& rStr);

public:
	CString m_strNumFormat;
	std::set<CDBRange> m_setSolutions;
	std::list<InvSol> m_listInvSol;
	std::list<CInvElem> m_listInvElem;
	InvSol m_invSolFinal;

protected:
	CDefinedRanges m_ranges;

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSInverse)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSINVERSE_H__
