// RichDocInSheet.h : header file
//
// This class defines custom modal property sheet 
// CRichDocInSheet.
//
// $Date: 2000/12/19 22:55:44 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __RICHDOCINSHEET_H__
#define __RICHDOCINSHEET_H__

#include "RichDocInPages.h"

/////////////////////////////////////////////////////////////////////////////
// CRichDocInSheet

#include "Layout/ETSLayout.h"

#ifndef baseRichDocInSheet
#define baseRichDocInSheet ETSLayoutPropertySheet
#endif

class CRichDocInSheet : public baseRichDocInSheet
{
	DECLARE_DYNAMIC(CRichDocInSheet)

// Construction
public:
	CRichDocInSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CRichDocInPage1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichDocInSheet)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichDocInSheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CRichDocInSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __RICHDOCINSHEET_H__
