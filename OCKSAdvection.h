// OCKSAdvection.h : header file
//
// This class defines custom modal property sheet 
// COCKSAdvection.
//
// $Date: 2001/09/14 20:12:03 $
// $Revision: 1.1.1.8 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __OCKSADVECTION_H__
#define __OCKSADVECTION_H__

#include "KPAdvectionPg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSAdvection

#include "OleCtlKeywordSheet.h"

#ifndef baseCOCKSAdvection
#define baseCOCKSAdvection COleCtlKeywordSheet
#endif

class COCKSAdvection : public baseCOCKSAdvection
{
	DECLARE_DYNAMIC(COCKSAdvection)

// Construction
public:
	COCKSAdvection(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPAdvectionPg1 m_Page1;
	CKPAdvectionPg2 m_Page2;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSAdvection)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COCKSAdvection();

// Generated message map functions
protected:
	//{{AFX_MSG(COCKSAdvection)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSADVECTION_H__
