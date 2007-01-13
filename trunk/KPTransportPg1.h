// KPTransportPg1.h : header file
// KPSelectedOutputPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __KPTRANSPORTPG1_H__
#define __KPTRANSPORTPG1_H__

#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "DefinedRanges.h"

/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg1 dialog

#ifndef baseCKPTransportPg1
#define baseCKPTransportPg1 CKeywordPage
#endif

class CKPTransportPg1 : public baseCKPTransportPg1
{
	DECLARE_DYNCREATE(CKPTransportPg1)

// Construction
public:
	CKPTransportPg1();
	~CKPTransportPg1();

// Dialog Data
	//{{AFX_DATA(CKPTransportPg1)
	enum { IDD = IDD_KEY_TRANSPORT_PG1 };
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
	//{{AFX_VIRTUAL(CKPTransportPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPTransportPg1)
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
// CKPTransportPg2 dialog

#ifndef baseCKPTransportPg2
#define baseCKPTransportPg2 CKeywordPage
#endif

#include "DefinedRanges.h"	// Added by ClassView
class CKPTransportPg2 : public baseCKPTransportPg2
{
	DECLARE_DYNCREATE(CKPTransportPg2)

// Construction
public:
	bool SetCells(size_t stCells, size_t stStags);
	std::list<CRange> m_listPunchRange;
	std::list<CRange> m_listPrintRange;	
	CKPTransportPg2();
	~CKPTransportPg2();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPTransportPg2)
	enum { IDD = IDD_KEY_TRANSPORT_PG2 };
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
	//{{AFX_VIRTUAL(CKPTransportPg2)
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
	//{{AFX_MSG(CKPTransportPg2)
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


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg3 dialog

#ifndef baseCKPTransportPg3
#define baseCKPTransportPg3 CKeywordPage
#endif

class CKPTransportPg3 : public baseCKPTransportPg3
{
	DECLARE_DYNCREATE(CKPTransportPg3)

// Construction
public:
	CKPTransportPg3();
	~CKPTransportPg3();

// Dialog Data
	//{{AFX_DATA(CKPTransportPg3)
	enum { IDD = IDD_KEY_TRANSPORT_PG3 };
	CComboBox	m_cboFD;
	CComboBox	m_cboBCLast;
	CComboBox	m_cboBCFirst;
	CEdit	m_eInputDesc;
	BOOL	m_bCorrectDisp;
	double	m_dDiffCoef;
	double	m_dTDC;
	BOOL	m_bUseThermal;
	//}}AFX_DATA
	double	m_dTRF;

	enum BC
	{
		BC_CLOSED,
		BC_CONSTANT,
		BC_FLUX,
	} m_bcFirst, m_bcLast;

	enum FD
	{
		FD_BACK,
		FD_DIFF_ONLY,
		FD_FORWARD,
	} m_fdFD;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPTransportPg3)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bSynchDiffCoefs;
	bool m_bSynching;
	// Generated message map functions
	//{{AFX_MSG(CKPTransportPg3)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusCboFirstCell();
	afx_msg void OnSetfocusCboLastCell();
	afx_msg void OnSetfocusCboFlowDir();
	afx_msg void OnSetfocusEDiffCoef();
	afx_msg void OnSetfocusETrf();
	afx_msg void OnSetfocusETdc();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeEDiffCoef();
	afx_msg void OnChangeETdc();
	afx_msg void OnCbUseThermal();
	afx_msg void OnSelchangeCboFirstCell();
	afx_msg void OnSelchangeCboLastCell();
	//}}AFX_MSG
	afx_msg void OnSetfocusCBCorrDisp();
	afx_msg void OnSetfocusCBUseTherm();
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg4 dialog

#ifndef baseCKPTransportPg4
#define baseCKPTransportPg4 CKeywordPage
#endif

class CKPTransportPg4 : public baseCKPTransportPg4
{
	DECLARE_DYNCREATE(CKPTransportPg4)

// Construction
public:
	bool SetCells(size_t stCells);
	CKPTransportPg4();
	~CKPTransportPg4();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPTransportPg4)
	enum { IDD = IDD_KEY_TRANSPORT_PG4 };
	CEdit	m_eInputDesc;
	CMSHFlexGrid	m_egLengths;
	CMSHFlexGrid	m_egDisps;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CRepeat> m_lrLengths;
	std::list<CRepeat> m_lrDisps;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPTransportPg4)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void InitEditGrid(CEditGrid& editGrid);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPTransportPg4)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgLen();
	afx_msg void OnKeyDownMshfgLen(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgDisp();
	afx_msg void OnKeyDownMshfgDisp(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	// afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

	LRESULT OnEndCellEdit(NMEGINFO* pInfo, CEditGrid& editGrid);

};


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg5 dialog

#ifndef baseCKPTransportPg5
#define baseCKPTransportPg5 CKeywordPage
#endif

class CKPTransportPg5 : public baseCKPTransportPg5
{
	DECLARE_DYNCREATE(CKPTransportPg5)

// Construction
public:
	CKPTransportPg5();
	~CKPTransportPg5();

// Dialog Data
	//{{AFX_DATA(CKPTransportPg5)
	enum { IDD = IDD_KEY_TRANSPORT_PG5 };
	CSpinButtonCtrl	m_spinDumpRestart;
	CSpinButtonCtrl	m_spinDumpModulus;
	CSpinButtonCtrl	m_spinStagCells;
	CEdit	m_eInputDesc;
	UINT	m_nStagCells;
	double	m_dExchFactor;
	double	m_dThetaM;
	double	m_dThetaIM;
	CString	m_strDumpFileName;
	BOOL	m_bDumpToFile;
	BOOL	m_bUseStagnant;
	//}}AFX_DATA
	UINT	m_nDumpModulus;
	UINT	m_nDumpRestart;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPTransportPg5)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPTransportPg5)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEStagCells();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeEStagCells();
	afx_msg void OnSetfocusEExchFact();
	afx_msg void OnSetfocusEMobPor();
	afx_msg void OnSetfocusEImmobPor();
	afx_msg void OnBDumpFile();
	afx_msg void OnCbDumpToFile();
	afx_msg void OnSetfocusEDumpFile();
	afx_msg void OnSetfocusEDumpMod();
	afx_msg void OnSetfocusEDumpRestart();
	afx_msg void OnCbUseStagnant();
	//}}AFX_MSG
	afx_msg void OnSetfocusCBDumpToFile();
	afx_msg void OnSetfocusCBUseStagnant();
	DECLARE_MESSAGE_MAP()

};



#endif // __KPTRANSPORTPG1_H__
