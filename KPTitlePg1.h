// KPTitlePg1.h : header file
//
// $Date: 2001/05/12 02:25:25 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#ifndef __KPTITLEPG1_H__
#define __KPTITLEPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPTitlePg1 dialog
#include "KeywordPage.h"

#ifndef baseKPTitlePg1
#define baseKPTitlePg1 CKeywordPage
#endif

class CKPTitlePg1 : public baseKPTitlePg1
{
	DECLARE_DYNCREATE(CKPTitlePg1)
	DECLARE_LAYOUT();

// Construction
public:
	CKPTitlePg1();
	~CKPTitlePg1();

// Dialog Data
	//{{AFX_DATA(CKPTitlePg1)
	enum { IDD = IDD_KEY_TITLE_PG1 };
	CEdit	m_eTitle;
	CString	m_strTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPTitlePg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DDX_Title(CDataExchange *pDX);
	// Generated message map functions
	//{{AFX_MSG(CKPTitlePg1)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



#endif // __KPTITLEPG1_H__
