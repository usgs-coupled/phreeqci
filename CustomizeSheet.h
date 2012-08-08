// CustomizeSheet.h : header file
//
// This class defines custom modal property sheet 
// CCustomizeSheet.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CUSTOMIZESHEET_H__
#define __CUSTOMIZESHEET_H__

#include "CustomizePage1.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

#include "Layout/ETSLayout.h"

#ifndef baseCustomizeSheet
#define baseCustomizeSheet ETSLayoutPropertySheet
#endif


class CCustomizeSheet : public baseCustomizeSheet
{
	DECLARE_DYNAMIC(CCustomizeSheet)

// Construction
public:
	CCustomizeSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CCustomizePage1 m_Page1;
	CCustomizePage2 m_Page2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomizeSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomizeSheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomizeSheet)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CUSTOMIZESHEET_H__
