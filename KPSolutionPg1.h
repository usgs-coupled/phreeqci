// KPSolutionPg1.h : header file
//
// $Date: 2001/09/14 20:12:01 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"


#ifndef __KPSOLUTIONPG1_H__
#define __KPSOLUTIONPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSolutionPg1 dialog

#include "KeywordPage.h"

#ifndef baseKPSolutionPg1
#define baseKPSolutionPg1 CKeywordPage
#endif

class CCommonKeywordSheet;

class CKPSolutionPg1 : public baseKPSolutionPg1
{
	DECLARE_DYNCREATE(CKPSolutionPg1)

public:
	double m_dWaterMass;
	double m_dDensity;
	double m_dTemp;
	double m_dPE_SI;
	double m_dPE;
	double m_dPH_SI;
	double m_dPH;
	CString m_strPH_Equil;
	CString m_strPE_Equil;
	CString m_strRedox;

// Construction
public:
	CKPSolutionPg1();
	~CKPSolutionPg1();

public:
	CCommonKeywordSheet* GetSheet();

// Dialog Data
	//{{AFX_DATA(CKPSolutionPg1)
	enum { IDD = IDD_KEY_SOLUTION_PG1 };
	CEdit	m_ctrlDescription;
	CEditGrid	m_ctrlGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolutionPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_NumDesc(CDataExchange *pDX, int nIDC);
	void DDX_Grid(CDataExchange *pDX, int nIDC);
	void InitGrid(CDataExchange* pDX, int nIDC);
	void InitValues();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSolutionPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellGrid();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	afx_msg void OnResetPressed();
	afx_msg void OnDblClickGrid();
	afx_msg void OnMouseDownGrid(short Button, short Shift, long x, long y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid messages
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	// custom setfocus handlers
	afx_msg void OnSetfocusReset();

	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in KPSolutionPg1.cpp
inline CCommonKeywordSheet* CKPSolutionPg1::GetSheet()
   { return (CCommonKeywordSheet*)GetParent(); }
#endif



/////////////////////////////////////////////////////////////////////////////
// CKPSolutionPg2 dialog

#ifndef baseKPSolutionPg2
#define baseKPSolutionPg2 CKeywordPage
#endif

class CKPSolutionPg2 : public baseKPSolutionPg2
{
	DECLARE_DYNCREATE(CKPSolutionPg2)

public:
	CString m_strChargeBalance;
	CString m_strDefaultUnits;
	std::list<CConc> m_listConc;

// Construction
public:
	CKPSolutionPg2();
	~CKPSolutionPg2();

// Dialog Data
	//{{AFX_DATA(CKPSolutionPg2)
	enum { IDD = IDD_KEY_SOLUTION_PG2 };
	CComboBox	m_ctrlChargeBalance;
	CComboBox	m_ctrlDefaultUnits;
	CCheckListCtrl	m_ctrlElements;
	CEdit	m_ctrlDescription;
	CEditGrid	m_ctrlGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolutionPg2)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitGrid(CDataExchange* pDX, int nIDC);
	void UpdateElementList(int nItem);
protected:
	CGridListDoc m_glDoc;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKPSolutionPg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemchangedElementList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	afx_msg void OnMajors();
	afx_msg void OnTrace();
	afx_msg void OnCustom();
	afx_msg void OnSelchangeCboDefUnits();
	afx_msg void OnEnterCellGrid();
	afx_msg void OnSetfocusElementList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusCboDefUnits();
	afx_msg void OnSetfocusCboCharge();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBtnModCust();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	//{{TESTING
#ifdef _DEBUG
	// afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
#endif // _DEBUG
	//}}TESTING

	// custom setfocus messages
	afx_msg void OnSetfocusMajors();
	afx_msg void OnSetfocusTrace();
	afx_msg void OnSetfocusCustom();
	afx_msg void OnSetfocusBtnModCust();

	// custom grid messages
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	// context menu check list commands
	afx_msg void OnViewChange(UINT uID);

	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CKPSolutionPg3 dialog

#ifndef baseKPSolutionPg3
#define baseKPSolutionPg3 CKeywordPage
#endif

class CKPSolutionPg3 : public baseKPSolutionPg3
{
	DECLARE_DYNCREATE(CKPSolutionPg3)

public:
	std::list<CIsotope> m_listIsotopes;

// Construction
public:
	CKPSolutionPg3();
	~CKPSolutionPg3();

// Dialog Data
	//{{AFX_DATA(CKPSolutionPg3)
	enum { IDD = IDD_KEY_SOLUTION_PG3 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_ctrlIsotopes;
	CEditGrid	m_ctrlGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolutionPg3)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_IsotopeList(CDataExchange* pDX, int nIDC);
	void InitGrid(CDataExchange* pDX, int nIDC);

// Implementation
protected:
	CGridListDoc m_glDoc;
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSolutionPg3)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedIsotopeList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellIsotopeGrid();
	afx_msg void OnKeyDownIsotopeGrid(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusIsotopeList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid messages
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};



#endif // __KPSOLUTIONPG1_H__
