// KPSurfaceSpeciesPg1.h : header file
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

#ifndef __KPSURFACESPECIESPG1_H__
#define __KPSURFACESPECIESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSurfaceSpeciesPg1 dialog

#ifndef baseCKPSurfaceSpeciesPg1
#define baseCKPSurfaceSpeciesPg1 CKeywordPage
#endif

class CKPSurfaceSpeciesPg1 : public baseCKPSurfaceSpeciesPg1
{
	DECLARE_DYNCREATE(CKPSurfaceSpeciesPg1)

public:
	std::list<CSpecies> m_listSpecies;

// Construction
public:
	CKPSurfaceSpeciesPg1();
	~CKPSurfaceSpeciesPg1();

// Dialog Data
#define CMSHFlexGrid CEditGrid
#define CSRCDBPG CSRCDBPGCH
	//{{AFX_DATA(CKPSurfaceSpeciesPg1)
	enum { IDD = IDD_KEY_SURFACE_SPECIES_PG1_NEW };
	CEdit	m_ctrlAssocRxn;
	CButton	m_ctrlCheckEqn;
	CButton	m_ctrlCheckCD_MUSIC;
	CEdit	m_ctrlLogK;
	CEdit	m_ctrlDeltaH;
	CComboBox	m_ctrlDeltaHUnits;
	CEdit	m_ctrlMoleBal;
	CEdit	m_ctrlA1;
	CEdit	m_ctrlA2;
	CEdit	m_ctrlA3;
	CEdit	m_ctrlA4;
	CEdit	m_ctrlA5;
	CEdit	m_ctrlA6;
	CEdit	m_ctrlZ0;
	CEdit	m_ctrlZ1;
	CEdit	m_ctrlZ2;
	CEdit	m_ctrlF;
	CEdit	m_ctrlCharge;
	CSRCDBPG	m_pager;
	CMSHFlexGrid	m_ctrlGrid;
	CEdit	m_eInputDesc;
	//}}AFX_DATA
#undef CSRCDBPG
#undef CMSHFlexGrid


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSurfaceSpeciesPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	bool m_bIgnoreChanges;
	bool m_bEnableLLNL;
	CPictureHolder m_pictHolderRightArrow;
	CPictureHolder m_pictHolderStar;
	HWND m_hWndLastControl;    // last control used
	BOOL m_bEditLastControl;   // last control was an edit item


// Implementation
protected:
	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();
	// Generated message map functions
	//{{AFX_MSG(CKPSurfaceSpeciesPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditAssocRxn();
	afx_msg void OnChangeEditLogk();
	afx_msg void OnChangeEditDeltaH();
	afx_msg void OnChangeEditA1();
	afx_msg void OnChangeEditA2();
	afx_msg void OnChangeEditA3();
	afx_msg void OnChangeEditA4();
	afx_msg void OnChangeEditA5();
	afx_msg void OnChangeEditA6();
	afx_msg void OnChangeEditZ0();
	afx_msg void OnChangeEditZ1();
	afx_msg void OnChangeEditZ2();
	afx_msg void OnChangeEditF();
	afx_msg void OnChangeEditCharge();
	afx_msg void OnKillfocusEditAssocRxn();
	afx_msg void OnKillfocusEditLogk();
	afx_msg void OnKillfocusEditDeltaH();
	afx_msg void OnKillfocusEditA1();
	afx_msg void OnKillfocusEditA2();
	afx_msg void OnKillfocusEditA3();
	afx_msg void OnKillfocusEditA4();
	afx_msg void OnKillfocusEditA5();
	afx_msg void OnKillfocusEditA6();
	afx_msg void OnKillfocusEditZ0();
	afx_msg void OnKillfocusEditZ1();
	afx_msg void OnKillfocusEditZ2();
	afx_msg void OnKillfocusEditF();
	afx_msg void OnKillfocusEditCharge();
	afx_msg void OnSetfocusEditAssocRxn();
	afx_msg void OnSetfocusEditLogk();
	afx_msg void OnSetfocusEditDeltaH();
	afx_msg void OnSetfocusEditA1();
	afx_msg void OnSetfocusEditA2();
	afx_msg void OnSetfocusEditA3();
	afx_msg void OnSetfocusEditA4();
	afx_msg void OnSetfocusEditA5();
	afx_msg void OnSetfocusEditA6();
	afx_msg void OnSetfocusEditZ0();
	afx_msg void OnSetfocusEditZ1();
	afx_msg void OnSetfocusEditZ2();
	afx_msg void OnSetfocusEditF();
	afx_msg void OnSetfocusEditCharge();
	afx_msg void OnSetFocusCheckEquation();
	afx_msg void OnSetFocusCheckCD_MUSIC();
	afx_msg void OnChangeEditMoleBal();
	afx_msg void OnKillfocusEditMoleBal();
	afx_msg void OnSetfocusEditMoleBal();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnterCellGrid();
	afx_msg void OnSelchangeComboDeltaHUnits();
	afx_msg void OnSetfocusComboDeltaHUnits();
	afx_msg void OnDblClickGrid();
	afx_msg void OnLeaveCellGrid();
	afx_msg void OnRowColChangeGrid();
	afx_msg void OnCheckEquation();
	afx_msg void OnCheckCD_MUSIC();
	afx_msg void OnGotoFirstClicked();
	afx_msg void OnGotoLastClicked();
	afx_msg void OnGotoNewClicked();
	afx_msg void OnGotoNextClicked();
	afx_msg void OnGotoPrevClicked();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	
	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __KPSURFACESPECIESPG1_H__
