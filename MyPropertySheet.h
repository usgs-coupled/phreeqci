// MyPropertySheet.h : header file
//
// $Date: 2001/08/20 20:36:17 $
// $Revision: 1.1.1.8 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CMyPropertySheet.
 
#ifndef __MYPROPERTYSHEET_H__
#define __MYPROPERTYSHEET_H__

#include "MyPropertyPage1.h"

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

#include "OleCtlKeywordSheet.h"

#ifndef baseCMyPropertySheet
#define baseCMyPropertySheet COleCtlKeywordSheet
#endif

class CMyPropertySheet : public baseCMyPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)

// Construction
public:
	CMyPropertySheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CMyPropertyPage1 m_Page1;
	CMyPropertyPage2 m_Page2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertySheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPropertySheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CMyPropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __MYPROPERTYSHEET_H__
