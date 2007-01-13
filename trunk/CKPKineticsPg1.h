// CKPKineticsPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#include "EditGrid.h"
//{{
#include "CheckListCtrl.h"
#include "KeywordPageListItems.h"
#include "GridListDoc.h"
//}}

#ifndef __CKPKINETICSPG1_H__
#define __CKPKINETICSPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg1 dialog

#include "CommonKeywordPage.h"

#ifndef baseCKPKineticsPg1
#define baseCKPKineticsPg1 CCommonKeywordPage
#endif

class CCKPKineticsPg1 : public baseCKPKineticsPg1
{
	// inner class
protected:
// COMMENT: {12/12/2000 4:05:31 PM}	class XGridTabItem
// COMMENT: {12/12/2000 4:05:31 PM}	{
// COMMENT: {12/12/2000 4:05:31 PM}	public:
// COMMENT: {12/12/2000 4:05:31 PM}		// CRate rate;
// COMMENT: {12/12/2000 4:05:31 PM}		CKineticComp kineticComp;
// COMMENT: {12/12/2000 4:05:31 PM}		long nRowIndex;
// COMMENT: {12/12/2000 4:05:31 PM}		int nTabIndex;
// COMMENT: {12/12/2000 4:05:31 PM}
// COMMENT: {12/12/2000 4:05:31 PM}		XGridTabItem()
// COMMENT: {12/12/2000 4:05:31 PM}		{
// COMMENT: {12/12/2000 4:05:31 PM}			nRowIndex = -1;
// COMMENT: {12/12/2000 4:05:31 PM}			nTabIndex = -1;
// COMMENT: {12/12/2000 4:05:31 PM}		}
// COMMENT: {12/12/2000 4:05:31 PM}
// COMMENT: {12/12/2000 4:05:31 PM}		virtual ~XGridTabItem(){}
// COMMENT: {12/12/2000 4:05:31 PM}	};

	DECLARE_DYNCREATE(CCKPKineticsPg1)

// Construction
public:
	CCKPKineticsPg1();
	~CCKPKineticsPg1();
#define CMSHFlexGrid CEditGrid
// Dialog Data
	//{{AFX_DATA(CCKPKineticsPg1)
	enum { IDD = IDD_KEY_KINETICS_PG1 };
	CCheckListCtrl	m_clRates;
	CTabCtrl	m_tcNames;
	CMSHFlexGrid	m_egNames;
	CMSHFlexGrid	m_egGeneral;
	CMSHFlexGrid	m_egFormula;
	CMSHFlexGrid	m_egUser;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CKineticComp> m_listComps;
	std::map<int, CKineticComp> m_mapComps;
	int m_nextMapInt;

protected:
	CGridListDoc m_gltDoc;
// COMMENT: {12/12/2000 4:05:48 PM}	std::list<XGridTabItem> m_listItems;

protected:
	void DDX_CompList(CDataExchange *pDX);
// COMMENT: {12/12/2000 4:04:42 PM}	BOOL GetComp(long nRowIndex, CKineticComp* pComp)const;
// COMMENT: {12/12/2000 4:05:05 PM}	CString GetRateName(long nRowIndex)const;
// COMMENT: {12/11/2000 5:31:38 PM}	BOOL DeleteComp(long nRowIndex);
// COMMENT: {12/11/2000 5:30:44 PM}	void InsertComp(long nRowIndex, CKineticComp& rComp);
	void DisplayTab(int nTabIndex);
// COMMENT: {12/11/2000 5:32:39 PM}	BOOL SetCompName(long nRowIndex, LPCTSTR lpszName);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPKineticsPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bIgnoreCheckChange;
	void InitGrids();
	void UpdateTabs();
	// Generated message map functions
	//{{AFX_MSG(CCKPKineticsPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTcNames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTcNames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedClRates(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgNames(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyDownMshfgGeneral(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgGeneral();
	afx_msg void OnEnterCellMshfgUser();
	afx_msg void OnKeyDownMshfgUser(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyDownMshfgFormula(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgFormula();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgNames();
	afx_msg void OnSetfocusClRates(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	//{{ Not Used??
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg1A dialog

#ifndef baseCKPKineticsPg1A
#define baseCKPKineticsPg1A CCommonKeywordPage
#endif

class CCKPKineticsPg1A : public baseCKPKineticsPg1A
{
	DECLARE_DYNCREATE(CCKPKineticsPg1A)

// Construction
public:
	CCKPKineticsPg1A();
	~CCKPKineticsPg1A();

	enum ODEMethodType 
	{
		ODE_RUNGA_KUTTA = 0,
		ODE_CVODE       = 1,
	} m_nODEMethodType;


	enum RKType 
	{
		RK_1 = 0,
		RK_2 = 1,
		RK_3 = 2,
		RK_6 = 3,
	} m_nRKType;


// Dialog Data
	//{{AFX_DATA(CCKPKineticsPg1A)
	enum { IDD = IDD_KEY_KINETICS_PG1A };
	CComboBox	m_cboRKOpt;
	double	m_dStepDivide;
	//}}AFX_DATA
// COMMENT: {9/17/2004 4:35:56 PM}	bool m_bCVODE;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPKineticsPg1A)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void InitCombos();
	void UpdateRadioState();


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPKineticsPg1A)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusCbRk();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnRungeRadio();
	afx_msg void OnCvodeRadio();
	afx_msg void OnSetfocusEStepDivide();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSetfocusRungeRadio();
	afx_msg void OnSetfocusCvodeRadio();

	DECLARE_MESSAGE_MAP()

};



/////////////////////////////////////////////////////////////////////////////
// CCKPKineticsPg2 dialog

#ifndef baseCKPKineticsPg2
#define baseCKPKineticsPg2 CCommonKeywordPage
#endif

class CCKPKineticsPg2 : public baseCKPKineticsPg2
{
	DECLARE_DYNCREATE(CCKPKineticsPg2)

// Construction
public:
	CCKPKineticsPg2();
	~CCKPKineticsPg2();

	enum amountType 
	{
		TYPE_LINEAR,
		TYPE_LIST
	} m_nAmountType;

// COMMENT: {4/29/2004 7:25:48 PM}	enum RKType 
// COMMENT: {4/29/2004 7:25:48 PM}	{
// COMMENT: {4/29/2004 7:25:48 PM}		RK_1 = 0,
// COMMENT: {4/29/2004 7:25:48 PM}		RK_2 = 1,
// COMMENT: {4/29/2004 7:25:48 PM}		RK_3 = 2,
// COMMENT: {4/29/2004 7:25:48 PM}		RK_6 = 3,
// COMMENT: {4/29/2004 7:25:48 PM}	} m_nRKType;
// COMMENT: {4/29/2004 7:25:48 PM}
	int m_nLinearSteps;
	std::list<double> m_listSteps;
	double m_dLinearAmt;


// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CCKPKineticsPg2)
	enum { IDD = IDD_KEY_KINETICS_PG2 };
// COMMENT: {4/29/2004 7:25:36 PM}	CComboBox	m_cboRKOpt;
	CSpinButtonCtrl	m_spinSteps;
	CEdit	m_eSteps;
	CEdit	m_eTotalTime;
	CMSHFlexGrid	m_ctrlStepEditGrid;
	//}}AFX_DATA
#undef CMSHFlexGrid


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPKineticsPg2)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_Steps(CDataExchange* pDX, int nIDC);

	void UpdateRadioState();
	void EnableLinear(BOOL bEnable);
	void EnableList(BOOL bEnable);
	void InitSpin();
	void InitStepEditGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPKineticsPg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioLinear();
	afx_msg void OnRadioList();
	afx_msg void OnSetfocusETotalTime();
	afx_msg void OnSetfocusESteps();
	afx_msg void OnEnterCellMshfgList();
	afx_msg void OnSetfocusCbRk();
	afx_msg void OnSetfocusEStepDivide();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnKeyDownMshfgList(short FAR* KeyCode, short Shift);
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


#endif // __CKPKINETICSPG1_H__
