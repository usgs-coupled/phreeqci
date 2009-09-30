// KPPhasesPg1.h : header file
//
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
#include "srcdbpg.h"
//}}AFX_INCLUDES
#include "KeywordPage.h"
#include "EditGrid.h"
#include "KeywordPageListItems.h"
#include "SRCDBPGCH.h"

#include <afxctl.h> // for class CPictureHolder

#ifndef __KPPHASESPG1_H__
#define __KPPHASESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPPhasesPg1 dialog

#ifndef baseCKPPhasesPg1
#define baseCKPPhasesPg1 CKeywordPage
#endif

class CKPPhasesPg1 : public baseCKPPhasesPg1
{
	DECLARE_DYNCREATE(CKPPhasesPg1)

public:
	std::list<CPhase> m_listPhase;

public:

// Construction
public:
	CKPPhasesPg1();
	~CKPPhasesPg1();

// Dialog Data
#define CMSHFlexGrid CEditGrid
#define CSRCDBPG CSRCDBPGCH
	//{{AFX_DATA(CKPPhasesPg1)
	enum { IDD = IDD_KEY_PHASES_PG1 };
	CEdit	m_ctrlName;
	CEdit	m_eInputDesc;
	CButton	m_ctrlCheckEqn;
	CComboBox	m_ctrlDeltaHUnits;
	CEdit	m_ctrlDeltaH;
	CEdit	m_ctrlLLNLa;
	CEdit	m_ctrlDHb;
	CEdit	m_ctrlDHa;
	CEdit	m_ctrlRxn;
	CEdit	m_ctrlLogK;
	CEdit	m_ctrlA1;
	CEdit	m_ctrlA2;
	CEdit	m_ctrlA3;
	CEdit	m_ctrlA4;
	CEdit	m_ctrlA5;
	CEdit	m_ctrlA6;
	CMSHFlexGrid	m_ctrlGrid;
	CSRCDBPG	m_pager;
	//}}AFX_DATA
#undef CSRCDBPG
#undef CMSHFlexGrid

protected:
	bool m_bIgnoreChanges;
	bool m_bEnableLLNL;
	CPictureHolder m_pictHolderRightArrow;
	CPictureHolder m_pictHolderStar;
	HWND m_hWndLastControl;    // last control used
	BOOL m_bEditLastControl;   // last control was an edit item
	std::map<CString, CSpecies::ActType> m_mapStrToActType;
	std::map<CSpecies::ActType, CString> m_mapActTypeToStr;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPPhasesPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();
	// Generated message map functions
	//{{AFX_MSG(CKPPhasesPg1)
	afx_msg void OnRowColChangeGrid();
	afx_msg void OnGotoNextClicked();
	afx_msg void OnLeaveCellGrid();
	afx_msg void OnGotoFirstClicked();
	afx_msg void OnGotoLastClicked();
	afx_msg void OnGotoNewClicked();
	afx_msg void OnGotoPrevClicked();
	afx_msg void OnChangeEditRxn();
	afx_msg void OnChangeEditLogk();
	afx_msg void OnKillfocusEditRxn();
	afx_msg void OnChangeEditDeltaH();
	afx_msg void OnSelchangeComboDeltaHUnits();
	afx_msg void OnCheckEquation();
	afx_msg void OnChangeEditA1();
	afx_msg void OnChangeEditA2();
	afx_msg void OnChangeEditA3();
	afx_msg void OnChangeEditA4();
	afx_msg void OnChangeEditA5();
	afx_msg void OnChangeEditA6();
	afx_msg void OnKillfocusEditPhaseName();
	afx_msg void OnKillfocusEditLogk();
	afx_msg void OnKillfocusEditDeltaH();
	afx_msg void OnKillfocusEditA1();
	afx_msg void OnKillfocusEditA2();
	afx_msg void OnKillfocusEditA3();
	afx_msg void OnKillfocusEditA4();
	afx_msg void OnKillfocusEditA5();
	afx_msg void OnKillfocusEditA6();
	afx_msg void OnDblClickGrid();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditPhaseName();
	afx_msg void OnSetfocusEditRxn();
	afx_msg void OnSetfocusEditLogk();
	afx_msg void OnSetfocusEditDeltaH();
	afx_msg void OnSetfocusComboDeltaHUnits();
	afx_msg void OnSetfocusEditA1();
	afx_msg void OnSetfocusEditA2();
	afx_msg void OnSetfocusEditA3();
	afx_msg void OnSetfocusEditA4();
	afx_msg void OnSetfocusEditA5();
	afx_msg void OnSetfocusEditA6();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnterCellGrid();
	afx_msg void OnChangeEditPhaseName();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowInsertRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertRow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowDeleteRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteRow(WPARAM wParam, LPARAM lParam);

	// custom radio notifications
// COMMENT: {8/16/2001 6:34:25 PM}	afx_msg void OnSetfocusRadio(UINT nID);
	DECLARE_MESSAGE_MAP()
};



#endif // __KPPHASESPG1_H__
