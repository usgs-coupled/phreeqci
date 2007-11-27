// CKSMix.h : header file
//
// This class defines custom modal property sheet 
// CCKSGas_Phase.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSMIX5F_H__
#define __CKSMIX5F_H__

#include "CKPMixPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSMix
#include "CommonKeywordSheet.h"

#ifndef baseCKSMix
#define baseCKSMix CCommonKeywordSheet
#endif

class CCKSMix : public baseCKSMix
{
	DECLARE_DYNAMIC(CCKSMix)

// Construction
public:
	CCKSMix(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	CCKPMixPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSGas_Phase)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSMix();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

public:
	CString m_strNumFormat;
	std::set<CDBRange> m_setSolutions;

protected:
	CDefinedRanges m_ranges;

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSMix)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSMIX5F_H__
