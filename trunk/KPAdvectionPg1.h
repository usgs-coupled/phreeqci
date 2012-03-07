// KPAdvectionPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __KPADVECTIONPG1_H__
#define __KPADVECTIONPG1_H__

#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CKPAdvectionPg1 dialog

#ifndef baseCKPAdvectionPg1
#define baseCKPAdvectionPg1 CKeywordPage
#endif

class CKPAdvectionPg1 : public baseCKPAdvectionPg1
{
	DECLARE_DYNCREATE(CKPAdvectionPg1)

// Construction
public:
	CKPAdvectionPg1();
	~CKPAdvectionPg1();

// Dialog Data
	//{{AFX_DATA(CKPAdvectionPg1)
	enum { IDD = IDD_KEY_ADVECTION_PG1 };
	CEdit	m_eInputDesc;
	CComboBox	m_cboInitTimeUnits;
	CComboBox	m_cboTimeStepUnits;
	CSpinButtonCtrl	m_spinShifts;
	CSpinButtonCtrl	m_spinCells;
	UINT	m_nCells;
	UINT	m_nShifts;
	BOOL	m_bPrintWarnings;
	double	m_dTimeStep;
	double	m_dInitTime;
	//}}AFX_DATA

	enum TimeUnit 
	{
		TU_SECONDS,
		TU_MINUTES,
		TU_HOURS,
		TU_DAYS,
		TU_YEARS
	} m_tuTimeStep, m_tuInitTime;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPAdvectionPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPAdvectionPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusECells();
	afx_msg void OnSetfocusEShifts();
	afx_msg void OnSetfocusETimeStep();
	afx_msg void OnSetfocusCboTimeStepUnits();
	afx_msg void OnSetfocusEInitTime();
	afx_msg void OnSetfocusCboInitTimeUnits();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg void OnSetfocusBWarn();
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CKPAdvectionPg2 dialog

#ifndef baseCKPAdvectionPg2
#define baseCKPAdvectionPg2 CKeywordPage
#endif

#include "DefinedRanges.h"	// Added by ClassView
class CKPAdvectionPg2 : public baseCKPAdvectionPg2
{
	DECLARE_DYNCREATE(CKPAdvectionPg2)

// Construction
public:
	bool SetCells(size_t stCells);
	std::list<CRange> m_listPunchRange;
	std::list<CRange> m_listPrintRange;
	CKPAdvectionPg2();
	~CKPAdvectionPg2();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPAdvectionPg2)
	enum { IDD = IDD_KEY_ADVECTION_PG2 };
	CEdit	m_eInputDesc;
	CSpinButtonCtrl	m_spinPunch;
	CSpinButtonCtrl	m_spinPrint;
	CCheckListCtrl	m_clcPunch;
	CCheckListCtrl	m_clcPrint;
	CMSHFlexGrid	m_egPrint;
	CMSHFlexGrid	m_egPunch;
	UINT	m_nPrintModulus;
	UINT	m_nPunchModulus;
	//}}AFX_DATA
#undef CMSHFlexGrid


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPAdvectionPg2)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void DDX_Ranges(CDataExchange* pDX, std::list<CRange>& listRange, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid);
	void InitEditGrid(CEditGrid& editGrid);
	void InitSpinner(CSpinButtonCtrl& spinner);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPAdvectionPg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClPrint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgPrint(short FAR* KeyCode, short Shift);
	afx_msg void OnItemchangedClPunch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgPunch(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClPrint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusEPrintMod();
	afx_msg void OnSetfocusEPunchMod();
	afx_msg void OnSetfocusClPunch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgPrint();
	afx_msg void OnEnterCellMshfgPunch();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	// afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

	void OnItemchanged(NM_LISTVIEW* pNMListView, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid);
	LRESULT OnEndCellEdit(NMEGINFO* pInfo, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid);
	void OnKeyDownMshfg(short FAR* KeyCode, short Shift, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid);

};

#endif // __KPADVECTIONPG1_H__
