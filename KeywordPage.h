// KeywordPage.h : header file
//
// $Date: 2001/09/14 20:12:03 $
// $Revision: 1.1.1.28 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDPAGE_H__017B18D0_FAEF_4AE1_A94F_B9916368AC39__INCLUDED_)
#define AFX_KEYWORDPAGE_H__017B18D0_FAEF_4AE1_A94F_B9916368AC39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeywordPage dialog
#include "Layout/ETSLayout.h"
#include "DDX_DDV.h"
#include <Htmlhelp.h>


// forward declaration
class CRichDocIn;
class CDatabase;

#ifndef baseKeywordPage
#define baseKeywordPage ETSLayoutPropertyPage
#endif

class CKeywordPage : public baseKeywordPage
{
	DECLARE_DYNCREATE(CKeywordPage)

// Construction
public:
	CKeywordPage();
	CKeywordPage(UINT nIDTemplate, UINT nIDCaption = 0);
	CKeywordPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

	~CKeywordPage();

public:
	bool IsContextHelp();
	static const double double_NaN;
	static const int int_NaN;

// Dialog Data
	//{{AFX_DATA(CKeywordPage)
	enum { IDD = 0 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKeywordPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRichDocIn* GetActiveDocument()const;
	const CDatabase& GetDatabase()const;
	// Generated message map functions
	//{{AFX_MSG(CKeywordPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYWORDPAGE_H__017B18D0_FAEF_4AE1_A94F_B9916368AC39__INCLUDED_)
