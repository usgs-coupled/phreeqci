// KSKinetics.h : header file
//
// $Date: 2004/09/20 20:37:10 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

// This class defines custom modal property sheet 
// CCKSKinetics.
 
#ifndef __KSKINETICS_H__
#define __KSKINETICS_H__

#include "CKPKineticsPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSKinetics

#include "CommonKeywordSheet.h"

#ifndef baseCKSKinetics
#define baseCKSKinetics CCommonKeywordSheet
#endif

class CCKSKinetics : public baseCKSKinetics
{
	DECLARE_DYNAMIC(CCKSKinetics)

// Construction
public:
	CCKSKinetics(CWnd* pWndParent = NULL);

// Attributes
public:
	CCKPKineticsPg1  m_Page1;
	CCKPKineticsPg1A m_Page1A;
	CCKPKineticsPg2  m_Page2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSKinetics)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~CCKSKinetics();

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSKinetics)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSKINETICS_H__
