// OCKSUserPrint.h : header file
//
// This class defines custom modal property sheet
// COCKSUserPrint.
//
// $Date: 2000/10/06 00:57:13 $
// $Revision: 1.1.1.23 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#ifndef __OCKSUSERPRINT_H__
#define __OCKSUSERPRINT_H__

#include "KPUserPrintPg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSUserPrint

#include "OleCtlKeywordSheet.h"

#ifndef baseOCKSUserPrint
#define baseOCKSUserPrint COleCtlKeywordSheet
#endif

class COCKSUserPrint : public baseOCKSUserPrint
{
	DECLARE_DYNAMIC(COCKSUserPrint)

// Construction
public:
	COCKSUserPrint(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPUserPrintPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSUserPrint)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~COCKSUserPrint();

// Generated message map functions
protected:
	//{{AFX_MSG(COCKSUserPrint)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSUSERPRINT_H__
