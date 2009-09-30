// KPSolutionSpeciesPg1.h : header file
// KPSelectedOutputPg1.h : header file
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

#ifndef __KPSOLUTIONSPECIESPG1_H__
#define __KPSOLUTIONSPECIESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSolutionSpeciesPg1 dialog

#ifndef baseCKPSolutionSpeciesPg1
#define baseCKPSolutionSpeciesPg1 CKeywordPage
#endif

class CKPSolutionSpeciesPg1 : public baseCKPSolutionSpeciesPg1
{
	DECLARE_DYNCREATE(CKPSolutionSpeciesPg1)

public:
	std::list<CSpecies> m_listSpecies;

// Construction
public:
	CKPSolutionSpeciesPg1();
	~CKPSolutionSpeciesPg1();

public:
	void EnableLLNL(bool bEnableLLNL);

// Dialog Data
#define CMSHFlexGrid CEditGrid
#define CSRCDBPG CSRCDBPGCH
	//{{AFX_DATA(CKPSolutionSpeciesPg1)
	enum { IDD = IDD_KEY_SOLUTION_SPECIES_PG1 };
	CEdit	m_eInputDesc;
	CEdit	m_ctrlMoleBal;
	CButton	m_ctrlCheckEqn;
	CComboBox	m_ctrlDeltaHUnits;
	CEdit	m_ctrlDeltaH;
	CEdit	m_ctrlLLNLa;
	CEdit	m_ctrlDHb;
	CEdit	m_ctrlDHa;
	CEdit	m_ctrlAssocRxn;
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
	//{{AFX_VIRTUAL(CKPSolutionSpeciesPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();
	// Generated message map functions
	//{{AFX_MSG(CKPSolutionSpeciesPg1)
	afx_msg void OnRowColChangeGrid();
	afx_msg void OnGotoNextClicked();
	afx_msg void OnLeaveCellGrid();
	afx_msg void OnGotoFirstClicked();
	afx_msg void OnGotoLastClicked();
	afx_msg void OnGotoNewClicked();
	afx_msg void OnGotoPrevClicked();
	afx_msg void OnChangeEditAssocRxn();
	afx_msg void OnChangeEditLogk();
	afx_msg void OnKillfocusEditAssocRxn();
	afx_msg void OnChangeEditDha();
	afx_msg void OnChangeEditDhb();
	afx_msg void OnChangeEditLlnlDha();
	afx_msg void OnChangeEditDeltaH();
	afx_msg void OnSelchangeComboDeltaHUnits();
	afx_msg void OnCheckEquation();
	afx_msg void OnChangeEditMoleBal();
	afx_msg void OnChangeEditA1();
	afx_msg void OnChangeEditA2();
	afx_msg void OnChangeEditA3();
	afx_msg void OnChangeEditA4();
	afx_msg void OnChangeEditA5();
	afx_msg void OnChangeEditA6();
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
	afx_msg void OnSetfocusEditAssocRxn();
	afx_msg void OnSetfocusEditLogk();
	afx_msg void OnSetfocusEditDeltaH();
	afx_msg void OnSetfocusComboDeltaHUnits();
	afx_msg void OnSetfocusEditA1();
	afx_msg void OnSetfocusEditA2();
	afx_msg void OnSetfocusEditA3();
	afx_msg void OnSetfocusEditA4();
	afx_msg void OnSetfocusEditA5();
	afx_msg void OnSetfocusEditA6();
	afx_msg void OnSetfocusEditDhb();
	afx_msg void OnSetfocusEditLlnlDha();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnterCellGrid();
	afx_msg void OnSetfocusEditMoleBal();
	afx_msg void OnSetfocusEditDha();
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
	afx_msg void OnActCoefRadioClicked(UINT nID);
	afx_msg void OnSetfocusRadio(UINT nID);
	DECLARE_MESSAGE_MAP()

};



#endif // __KPSOLUTIONSPECIESPG1_H__
