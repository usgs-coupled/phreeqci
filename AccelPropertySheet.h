// AccelPropertySheet.h : header file
//
// $Date: 2000/10/04 20:17:28 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCELPROPERTYSHEET_H__B2B5C610_87D4_4BB0_B0EE_4E2004162066__INCLUDED_)
#define AFX_ACCELPROPERTYSHEET_H__B2B5C610_87D4_4BB0_B0EE_4E2004162066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAccelPropertySheet

#include "Layout/ETSLayout.h"

#ifndef baseAccelPropertySheet
#define baseAccelPropertySheet ETSLayoutPropertySheet
#endif

class CAccelPropertySheet : public baseAccelPropertySheet
{
	DECLARE_DYNAMIC(CAccelPropertySheet)

// Construction
public:
	CAccelPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAccelPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccelPropertySheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAccelPropertySheet();

	// Generated message map functions
protected:
	CMap<int, int, int, int&> m_accel;

	//{{AFX_MSG(CAccelPropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCELPROPERTYSHEET_H__B2B5C610_87D4_4BB0_B0EE_4E2004162066__INCLUDED_)
