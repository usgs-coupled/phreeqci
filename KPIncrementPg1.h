// KPIncrementPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __KPINCREMENTPG1_H__
#define __KPINCREMENTPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPIncrementPg1 dialog
#include "KeywordPage.h"

#ifndef baseCKPIncrementPg1
#define baseCKPIncrementPg1 CKeywordPage
#endif

class CKPIncrementPg1 : public baseCKPIncrementPg1
{
	DECLARE_DYNCREATE(CKPIncrementPg1)

// Construction
public:
	CKPIncrementPg1();
	~CKPIncrementPg1();

// Dialog Data
	//{{AFX_DATA(CKPIncrementPg1)
	enum { IDD = IDD_KEY_INCREMENT };
	CEdit	m_eDesc;
	BOOL	m_bTrueOrFalse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPIncrementPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPIncrementPg1)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



#endif // __KPINCREMENTPG1_H__
