// KPInversePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "GridListDoc.h"

// forward declaration
class CCKSInverse;

#ifndef __KPINVERSEPG1_H__
#define __KPINVERSEPG1_H__

#include "CommonKeywordPage.h"
/////////////////////////////////////////////////////////////////////////////
// CKPInversePg1A dialog

#ifndef baseKPInversePg1A
#define baseKPInversePg1A CCommonKeywordPage
#endif

class CKPInversePg1A : public baseKPInversePg1A
{
	DECLARE_DYNCREATE(CKPInversePg1A)

// Construction
public:
	CKPInversePg1A();
	~CKPInversePg1A();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPInversePg1A)
	enum { IDD = IDD_KEY_INVERSE_PG1A };
	CCheckListCtrl	m_clcFinalSoln;
	CCheckListCtrl	m_clcInitSolns;
	CMSHFlexGrid	m_egInitSolns;
	CMSHFlexGrid	m_egFinalSoln;
	BOOL	m_bMinimal;
	BOOL	m_bMineralWater;
	double	m_dTol;
	//}}AFX_DATA
	BOOL   m_bRange;
	double m_dRange;
#undef CMSHFlexGrid

public:
	CCKSInverse* GetSheet();

protected:
	CGridListDoc m_glDocInit;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPInversePg1A)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_Grids(CDataExchange* pDX);
	void DDX_Range(CDataExchange* pDX);
	void InitGrids();
	void EnableRange(BOOL bEnable);


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPInversePg1A)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClcInitSolns(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedClcFinalSoln(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBRange();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgInitsolns();
	afx_msg void OnEnterCellMshfgFinalsoln();
	afx_msg void OnSetfocusETol();
	afx_msg void OnSetfocusClcInitsolns(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusClcFinalsoln(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgInitSolns(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyDownMshfgFinalSoln(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusERange();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// checkbox setfocus handlers
	afx_msg void OnSetfocusBRange();
	afx_msg void OnSetfocusBMineralWater();
	afx_msg void OnSetfocusBMinimal();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in KPInversePg1A.cpp
inline CCKSInverse* CKPInversePg1A::GetSheet()
   { return (CCKSInverse*)GetParent(); }
#endif


/////////////////////////////////////////////////////////////////////////////
// CKPInversePg1B dialog

#ifndef baseKPInversePg1B
#define baseKPInversePg1B CKeywordPage
#endif

class CKPInversePg1B : public baseKPInversePg1B
{
	DECLARE_DYNCREATE(CKPInversePg1B)

// Construction
public:
	CKPInversePg1B();
	~CKPInversePg1B();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPInversePg1B)
	enum { IDD = IDD_KEY_INVERSE_PG1B };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcPhases;
	CMSHFlexGrid	m_egPhaseGrid;
	//}}AFX_DATA
	std::list<CInvPhase> m_listInvPhase;
#undef CMSHFlexGrid

public:
	CCKSInverse* GetSheet();


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPInversePg1B)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	static const long nNameCol;
	static const long nDissOrPrecCol;
	static const long nForceCol;

	CGridListDoc m_glDoc;
	void DDX_PhaseGrid(CDataExchange* pDX);
	void InitPhaseGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPInversePg1B)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClPhases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgPhasegrid(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgPhasegrid();
	afx_msg void OnSetfocusClPhases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in KPInversePg1B.cpp
inline CCKSInverse* CKPInversePg1B::GetSheet()
   { return (CCKSInverse*)GetParent(); }
#endif

/////////////////////////////////////////////////////////////////////////////
// CKPInversePg2 dialog

#ifndef baseKPInversePg2
#define baseKPInversePg2 CKeywordPage
#endif

class CKPInversePg2 : public baseKPInversePg2
{
	DECLARE_DYNCREATE(CKPInversePg2)

// Construction
public:
	CKPInversePg2();
	~CKPInversePg2();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPInversePg2)
	enum { IDD = IDD_KEY_INVERSE_PG2 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcElements;
	CMSHFlexGrid	m_egBalance;
	double	m_dWaterUncert;
	BOOL	m_bWaterUncert;
	BOOL	m_bFrozen;
	//}}AFX_DATA
#undef CMSHFlexGrid
	// std::list<CInvElem> m_listInvElem;
public:
	CCKSInverse* GetSheet();

protected:
	CGridListDoc m_glDoc;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPInversePg2)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() /* const */;
	// virtual void Dump(CDumpContext& dc) const;
#endif
	void EnableWater(BOOL bEnable);
	// Generated message map functions
	//{{AFX_MSG(CKPInversePg2)
	afx_msg void OnItemchangedClElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBFreeze();
	afx_msg void OnBWater();
	afx_msg void OnBReset();
	afx_msg void OnKeyDownMshfgBalance(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgBalance();
	afx_msg void OnSetfocusEWater();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// checkbox setfocus handlers
	afx_msg void OnSetfocusBWater();
	afx_msg void OnSetfocusBFreeze();
	afx_msg void OnSetfocusBReset();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in KPInversePg1.cpp
inline CCKSInverse* CKPInversePg2::GetSheet()
   { return (CCKSInverse*)GetParent(); }
#endif


/////////////////////////////////////////////////////////////////////////////
// CKPInversePg3 dialog

#ifndef baseKPInversePg3
#define baseKPInversePg3 CKeywordPage
#endif

class CKPInversePg3 : public baseKPInversePg3
{
	DECLARE_DYNCREATE(CKPInversePg3)

// Construction
public:
	CKPInversePg3();
	~CKPInversePg3();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPInversePg3)
	enum { IDD = IDD_KEY_INVERSE_PG3 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcIsoPhase;
	CCheckListCtrl	m_clcIsoSol;
	CMSHFlexGrid	m_egSol;
	CMSHFlexGrid	m_egPhase;
	BOOL	m_bFreezePhase;
	BOOL	m_bFreezeSol;
	//}}AFX_DATA
#undef CMSHFlexGrid
public:
	CCKSInverse* GetSheet();
	std::list<CInvIsotope> m_listInvIsotopes;
	std::map<CString, double> m_mapIso2DefaultUnc;


protected:
	CGridListDoc m_glDocSol;
	CGridListDoc m_glDocPhase;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPInversePg3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_Phases(CDataExchange* pDX);
	void DDX_Soln(CDataExchange* pDX);

protected:
	void InitSoln();
	void InitPhases();
	void EnablePhases(bool bEnable);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPInversePg3)
	afx_msg void OnItemchangedClIsoSol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedClIsoPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgSol(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyDownMshfgPhase(short FAR* KeyCode, short Shift);
	afx_msg void OnBFreezeSol();
	afx_msg void OnBFreezePhase();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusClIsoSol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusClIsoPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgSol();
	afx_msg void OnEnterCellMshfgPhase();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// checkbox setfocus handlers
	afx_msg void OnSetfocusBFreezePhase();
	afx_msg void OnSetfocusBFreezeSol();

	// custom grid messages
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in KPInversePg1.cpp
inline CCKSInverse* CKPInversePg3::GetSheet()
   { return (CCKSInverse*)GetParent(); }
#endif


#endif // __KPINVERSEPG1_H__
