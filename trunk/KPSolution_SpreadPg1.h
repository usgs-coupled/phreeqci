// KPSolution_SpreadPg1.h : header file
// KPSelectedOutputPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#include "EditGrid.h"

#ifndef __KPSOLUTION5F_SPREADPG1_H__
#define __KPSOLUTION5F_SPREADPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg1 dialog
#include "KeywordPage.h"
#include "KPSolutionPg1.h"

#ifndef baseKPSolution_SpreadPg1
#define baseKPSolution_SpreadPg1 CKeywordPage
#endif

class CKPSolution_SpreadPg1 : public baseKPSolution_SpreadPg1
{
	DECLARE_DYNCREATE(CKPSolution_SpreadPg1)

// Construction
public:
	CKPSolution_SpreadPg1();
	~CKPSolution_SpreadPg1();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSolution_SpreadPg1)
	enum { IDD = IDD_KEY_SOLUTION_S_PG1 };
	CEdit	m_ctrlDescription;
	CCheckListCtrl	m_ctrlCheckList;
	CMSHFlexGrid	m_ctrlGrid;
	//}}AFX_DATA
#undef CMSHFlexGrid
	BOOL m_bFreeze;
	std::vector< std::string > m_vsHeading;
	std::vector< std::vector<std::string> > m_vvsCells;
	//std::set< std::string > m_setExtraHeadings;
	std::map< CString, UINT > m_mapExtraHeadings;

private:
	// std::set<CString> m_setSubItems;
	std::map<CString, CString> m_mapSubItem2Item;
	std::map<CString, CString> m_mapItem2SubItem;
	bool m_bIgnoreItemchanged;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolution_SpreadPg1)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateElementList(int nItem);
	bool IsMoreThanOneHeader(CString strHeader, long nCol);
	// Generated message map functions
	//{{AFX_MSG(CKPSolution_SpreadPg1)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBFreeze();
	afx_msg void OnDoubleclickedBFreeze();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnChkUseInverse();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClTotals(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgTotals();
	afx_msg void OnDblClickMshfgTotals();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom setfocus messages
	afx_msg void OnSetfocusBFreeze();
	afx_msg void OnSetfocusBUseInverse();

	// custom grid messages
	// afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowDeleteRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAllowDeleteCol(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteCol(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowInsertCol(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertCol(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowInsertRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertRow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnResizeRows(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResizeCols(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg2 dialog
#ifndef baseKPSolution_SpreadPg2
#define baseKPSolution_SpreadPg2 CKeywordPage
#endif

class CKPSolution_SpreadPg2 : public baseKPSolution_SpreadPg2
{
	DECLARE_DYNCREATE(CKPSolution_SpreadPg2)

// Construction
public:
	std::map<std::string, CConc> m_mapElement2Conc;
	CKPSolution_SpreadPg2();
	~CKPSolution_SpreadPg2();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSolution_SpreadPg2)
	enum { IDD = IDD_KEY_SOLUTION_S_PG2 };
	CEdit	m_ctrlDescription;
	CComboBox	m_cboChargeBalance;
	CMSHFlexGrid	m_ctrlGrid;
	//}}AFX_DATA
#undef CMSHFlexGrid


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolution_SpreadPg2)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSolution_SpreadPg2)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgTotals();
	afx_msg void OnSetfocusCboCharge();
	afx_msg void OnKeyDownMshfgTotals(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid messages
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg3 dialog
#ifndef baseKPSolution_SpreadPg3
#define baseKPSolution_SpreadPg3 CKeywordPage
#endif

class CKPSolution_SpreadPg3 : public baseKPSolution_SpreadPg3
{
	DECLARE_DYNCREATE(CKPSolution_SpreadPg3)

public:
	double m_dWaterMass;
	double m_dDensity;
	double m_dTemp;
	double m_dPE;
	double m_dPH;
	CString m_strDefaultUnits;
	CString m_strRedox;
// Construction
public:
	CKPSolution_SpreadPg3();
	~CKPSolution_SpreadPg3();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSolution_SpreadPg3)
	enum { IDD = IDD_KEY_SOLUTION_S_PG3 };
	CEdit	m_ctrlDescription;
	CMSHFlexGrid	m_ctrlGrid;
	//}}AFX_DATA
#undef CMSHFlexGrid


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolution_SpreadPg3)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DefaultValues();
	void DDX_Grid(CDataExchange *pDX, int nIDC);
	void InitGrid(CDataExchange* pDX, int nIDC);
	// void InitValues();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKPSolution_SpreadPg3)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnterCellGrid();
	afx_msg void OnReset();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
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


/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg4 dialog
#ifndef baseKPSolution_SpreadPg4
#define baseKPSolution_SpreadPg4 CKPSolutionPg3
#endif

class CKPSolution_SpreadPg4 : public baseKPSolution_SpreadPg4
{
	DECLARE_DYNCREATE(CKPSolution_SpreadPg4)

// Construction
public:
	CKPSolution_SpreadPg4();
	~CKPSolution_SpreadPg4();

// Dialog Data
	//{{AFX_DATA(CKPSolution_SpreadPg4)
	enum { IDD = IDD_KEY_SOLUTION_S_PG4 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolution_SpreadPg4)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_IsotopeList(CDataExchange* pDX, int nIDC);


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSolution_SpreadPg4)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnterCellIsotopeGrid();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid messages
	// afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};



#endif // __KPSOLUTION5F_SPREADPG1_H__
