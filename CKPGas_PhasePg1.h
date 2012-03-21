// CKPGas_PhasePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CKPGAS5F_PHASEPG1_H__
#define __CKPGAS5F_PHASEPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPGas_PhasePg1 dialog

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"
#include "DefinedRanges.h"
//{{
#include "phreeqc3/src/GasPhase.h"
//}}

#include "CommonKeywordPage.h"

#ifndef baseCKPGas_PhasePg1
#define baseCKPGas_PhasePg1 CCommonKeywordPage
#endif

class CCKPGas_PhasePg1 : public baseCKPGas_PhasePg1
{
	DECLARE_DYNCREATE(CCKPGas_PhasePg1)
	DECLARE_LAYOUT();

// Construction
public:
	CCKPGas_PhasePg1(CTreeCtrlNode simNode = CTreeCtrlNode());
	~CCKPGas_PhasePg1();

public:
// COMMENT: {3/14/2012 11:34:36 PM}	enum GasPhaseType
// COMMENT: {3/14/2012 11:34:36 PM}	{
// COMMENT: {3/14/2012 11:34:36 PM}		pressureType = 0,
// COMMENT: {3/14/2012 11:34:36 PM}		volumeType   = 1, cxxGasPhase::
// COMMENT: {3/14/2012 11:34:36 PM}	} m_nType;

	enum cxxGasPhase::GP_TYPE m_nType;

	enum nSolution
	{
		NONE = -1,
	};

	int m_nSolution;
	double m_dTempC;
	double m_dVolumeL;
	double m_dPressureAtm;
	CString m_strNumFormat;

	std::list<CGasComp> m_listGasComp;

// Dialog Data
	//{{AFX_DATA(CCKPGas_PhasePg1)
	enum { IDD = IDD_KEY_GAS_PHASE_PG1 };
	CComboBox	m_cboSolutions;
	CButton	m_btnEquilbrate;
	CCheckListCtrl	m_ctrlGasCheckList;
	CEditGrid	m_ctrlGasEditGrid;
	//}}AFX_DATA

protected:
	void DDX_GasCompList(CDataExchange *pDX, int nIDC);
	void DDX_Type(CDataExchange *pDX);

	BOOL InitNumDesc();
	BOOL InitGasCheckList();
	BOOL InitGasEditGrid();
	BOOL InitRadioCombo();


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPGas_PhasePg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CGridListDoc m_glDoc;
	CDefinedRanges m_ranges;

	void UpdateRadioState();

	// Generated message map functions
	//{{AFX_MSG(CCKPGas_PhasePg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusClGasPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioFixPress();
	afx_msg void OnRadioFixVol();
	afx_msg void OnBtnEquil();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgGasphase();
	afx_msg void OnItemchangedClGasphase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgGasphase(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusCboEquil();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom setfocus handlers
	afx_msg void OnSetfocusRadioFixPress();
	afx_msg void OnSetfocusRadioFixVol();
	afx_msg void OnSetfocusBtnEquil();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __CKPGAS5F_PHASEPG1_H__
