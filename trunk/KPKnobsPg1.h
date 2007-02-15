// KPKnobsPg1.h : header file
//


#ifndef __KPKNOBSPG1_H__
#define __KPKNOBSPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPKnobsPg1 dialog
#include "KeywordPage.h"

#ifndef baseCKPKnobsPg1
#define baseCKPKnobsPg1 CKeywordPage
#endif

class CKPKnobsPg1 : public baseCKPKnobsPg1
{
	DECLARE_DYNCREATE(CKPKnobsPg1)

public:
	enum ValueType
	{
		AS_IS    = 0,
		AS_TRUE  = 1,
		AS_FALSE = 2
	} m_arrValue[8];

// Construction
public:
	CKPKnobsPg1();
	~CKPKnobsPg1();

// Dialog Data
	//{{AFX_DATA(CKPKnobsPg1)
	enum { IDD = IDD_KEY_KNOBS };
	CEdit	m_eInputDesc;
	CSpinButtonCtrl	m_spinIterations;
	UINT	m_nItmax;
	double	m_dConvergenceTolerance;
	double	m_dIneqTol;
	double	m_dStepSize;
	double	m_dPeStepSize;
	//}}AFX_DATA
#ifdef ENABLE_SCALE_PURE_PHASES
	double	m_dPPScale;
#endif


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPKnobsPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPKnobsPg1)
	afx_msg void OnBAsIs();
	afx_msg void OnBTrue();
	afx_msg void OnBFalse();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSetfocusCbo(UINT nID);
	afx_msg void OnSetFocusEdit(UINT nID);
	DECLARE_MESSAGE_MAP()

};



#endif // __KPKNOBSPG1_H__
