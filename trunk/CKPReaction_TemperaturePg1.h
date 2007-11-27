// CKPReaction_TemperaturePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CKPREACTION5F_TEMPERATUREPG1_H__
#define __CKPREACTION5F_TEMPERATUREPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPReaction_TemperaturePg1 dialog

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "KeywordPageListItems.h"

#include "CommonKeywordPage.h"

#ifndef baseCKPReaction_TemperaturePg1
#define baseCKPReaction_TemperaturePg1 CCommonKeywordPage
#endif

class CCKPReaction_TemperaturePg1 : public baseCKPReaction_TemperaturePg1
{
	DECLARE_DYNCREATE(CCKPReaction_TemperaturePg1)

// Construction
public:
	std::list<double> m_listSteps;
	double m_dTemp2;
	double m_dTemp1;
	int m_nLinearSteps;
	CCKPReaction_TemperaturePg1();
	~CCKPReaction_TemperaturePg1();

	enum stepType 
	{
		TYPE_LINEAR,
		TYPE_LIST
	} m_nType;


// Dialog Data
	//{{AFX_DATA(CCKPReaction_TemperaturePg1)
	enum { IDD = IDD_KEY_RXN_TEMP };
	CSpinButtonCtrl	m_spinSteps;
	CEdit	m_eSteps;
	CEdit	m_eTemp2;
	CEdit	m_eTemp1;
	CEditGrid	m_ctrlStepEditGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPReaction_TemperaturePg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_Steps(CDataExchange* pDX, int nIDC);
	void UpdateRadioState();
	void EnableLinear(BOOL bEnable);
	void EnableList(BOOL bEnable);
	void InitSpin(CDataExchange* pDX);
	void InitStepEditGrid(CDataExchange* pDX);



// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPReaction_TemperaturePg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioLinear();
	afx_msg void OnRadioList();
	afx_msg void OnSetfocusETemp1();
	afx_msg void OnSetfocusETemp2();
	afx_msg void OnSetfocusESteps();
	afx_msg void OnKeyDownMshfgList(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom radio button setfocus handlers
	afx_msg void OnSetfocusRadioLinear();
	afx_msg void OnSetfocusRadioList();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __CKPREACTION5F_TEMPERATUREPG1_H__
