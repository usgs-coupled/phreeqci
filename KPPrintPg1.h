// KPPrintPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __KPPRINTPG1_H__
#define __KPPRINTPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPPrintPg1 dialog

#include "KeywordPage.h"

#ifndef baseCKPPrintPg1
#define baseCKPPrintPg1 CKeywordPage
#endif

class CKPPrintPg1 : public baseCKPPrintPg1
{
	DECLARE_DYNCREATE(CKPPrintPg1)

// Construction
public:
	CKPPrintPg1();
	~CKPPrintPg1();

// Dialog Data
	//{{AFX_DATA(CKPPrintPg1)
	enum { IDD = IDD_KEY_PRINT };
	CEdit	m_eInputDesc;
	int	m_nLimit;
	CSpinButtonCtrl	m_spinLimit;
	//}}AFX_DATA


	enum ValueType
	{
		AS_IS    = 0,
		AS_TRUE  = 1,
		AS_FALSE = 2
	} m_arrValue[17];


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPPrintPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPPrintPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnBAsIs();
	afx_msg void OnBTrue();
	afx_msg void OnBFalse();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusELimit();
	//}}AFX_MSG
	afx_msg void OnSetfocusCbo(UINT nID);
	afx_msg void OnSetfocusBtn(UINT nID);
	afx_msg void OnSetfocusRadioBtn(UINT nID);
	afx_msg void OnClickedRadio(UINT nID);
	DECLARE_MESSAGE_MAP()

};



#endif // __KPPRINTPG1_H__
