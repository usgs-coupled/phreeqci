// CKPReactionPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CKPREACTIONPG1_H__
#define __CKPREACTIONPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg1 dialog

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"

#include "CommonKeywordPage.h"

#ifndef baseCKPReactionPg1
#define baseCKPReactionPg1 CCommonKeywordPage
#endif

class CCKPReactionPg1 : public baseCKPReactionPg1
{
	DECLARE_DYNCREATE(CCKPReactionPg1)
	DECLARE_LAYOUT();

public:
	std::list<CNameCoef> m_listNameCoef;


// Construction
public:
	CCKPReactionPg1();
	~CCKPReactionPg1();

// Dialog Data
	//{{AFX_DATA(CCKPReactionPg1)
	enum { IDD = IDD_KEY_REACTION_PG1 };
	CCheckListCtrl	m_ctrlPhaseCheckList;
	CEditGrid	m_ctrlReactionEditGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPReactionPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_NameCoefList(CDataExchange* pDX, int nIDC);
	void InitGrid();

protected:
	CGridListDoc m_glDoc;
	bool m_bComboFilledOnce;


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPReactionPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgReaction(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgReaction();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnSetfocusClPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CCKPReactionPg2 dialog

#ifndef baseCKPReactionPg2
#define baseCKPReactionPg2 CCommonKeywordPage
#endif

class CCKPReactionPg2 : public baseCKPReactionPg2
{
	DECLARE_DYNCREATE(CCKPReactionPg2)

public:
	enum amountType 
	{
		TYPE_LINEAR,
		TYPE_LIST
	} m_nType;

	enum unitType
	{
		UNITS_MICROMOLES = 0,
		UNITS_MILLIMOLES = 1,
		UNITS_MOLES = 2
	} m_nUnits;

	int m_nLinearSteps;
	double m_dLinearAmt;
	std::list<double> m_listSteps;

// Construction
public:
	CCKPReactionPg2();
	~CCKPReactionPg2();

// Dialog Data
	//{{AFX_DATA(CCKPReactionPg2)
	enum { IDD = IDD_KEY_REACTION_PG2 };
	CEdit	m_eRxnAmt;
	CSpinButtonCtrl	m_spinSteps;
	CComboBox	m_cboUnitsSteps;
	CComboBox	m_cboUnitsLinear;
	CEdit	m_eSteps;
	CEditGrid	m_ctrlStepEditGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPReactionPg2)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_Steps(CDataExchange* pDX, int nIDC);

protected:
	void UpdateRadioState();
	void EnableLinear(BOOL bEnable);
	void EnableList(BOOL bEnable);
	void InitCombos();
	void InitSpin();
	void InitStepEditGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPReactionPg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgList();
	afx_msg void OnKeyDownMshfgList(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusERxnAmt();
	afx_msg void OnSetfocusCboLinearUnits();
	afx_msg void OnSetfocusESteps();
	afx_msg void OnSetfocusCboListUnits();
	afx_msg void OnRadioLinear();
	afx_msg void OnRadioList();
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



#endif // __CKPREACTIONPG1_H__
