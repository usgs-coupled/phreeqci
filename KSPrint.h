// KSPrint.h : header file
//
// $Date: 2001/08/20 20:36:16 $
// $Revision: 1.1.1.9 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSPrint.
 
#ifndef __KSPRINT_H__
#define __KSPRINT_H__

#include "KPPrintPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSPrint

#include "KeywordSheet.h"

#ifndef baseCKSPrint
#define baseCKSPrint CKeywordSheet
#endif

class CKSPrint : public baseCKSPrint
{
	DECLARE_DYNAMIC(CKSPrint)

// Construction
public:
	CKSPrint(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPPrintPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSPrint)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSPrint();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSPrint)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSPRINT_H__
