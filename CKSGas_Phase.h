// CKSGas_Phase.h : header file
//
// This class defines custom modal property sheet 
// CCKSGas_Phase.
//
// $Date: 2000/10/04 20:17:34 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSGAS5F_PHASE_H__
#define __CKSGAS5F_PHASE_H__

#include "CKPGas_PhasePg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSGas_Phase
#include "CommonKeywordSheet.h"

#ifndef baseCKSGas_Phase
#define baseCKSGas_Phase CCommonKeywordSheet
#endif

class CCKSGas_Phase : public baseCKSGas_Phase
{
	DECLARE_DYNAMIC(CCKSGas_Phase)

// Construction
public:
	CCKSGas_Phase(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	CCKPGas_PhasePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSGas_Phase)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSGas_Phase();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSGas_Phase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSGAS5F_PHASE_H__
