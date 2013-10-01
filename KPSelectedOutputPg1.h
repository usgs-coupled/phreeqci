// KPSelectedOutputPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __KPSELECTEDOUTPUTPG1_H__
#define __KPSELECTEDOUTPUTPG1_H__

#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "GridListDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg1 dialog

#ifndef baseCKPSelectedOutputPg1
#define baseCKPSelectedOutputPg1 CKeywordPage
#endif

class CKPSelectedOutputPg1 : public baseCKPSelectedOutputPg1
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg1)

// Construction
public:
	CKPSelectedOutputPg1();
	~CKPSelectedOutputPg1();

// Dialog Data
	//{{AFX_DATA(CKPSelectedOutputPg1)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG1 };
	CEdit	m_eInputDesc;
	CString	m_strFileName;
	//}}AFX_DATA

	enum ValueType
	{
		AS_IS    = 0,
		AS_TRUE  = 1,
		AS_FALSE = 2
	} m_arrValue[19];

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg1)
	afx_msg void OnBAsIs();
	afx_msg void OnBTrue();
	afx_msg void OnBFalse();
	afx_msg void OnBrowse();
	afx_msg void OnSelChangeCbo(UINT nID);
	afx_msg void OnSetfocusFileName();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg void OnSetfocusCbo(UINT nID);
	afx_msg void OnSetfocusBtn(UINT nID);
	afx_msg void OnSetfocusBrowse();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg2 dialog

#ifndef baseCKPSelectedOutputPg2
#define baseCKPSelectedOutputPg2 CKeywordPage
#endif

class CKPSelectedOutputPg2 : public baseCKPSelectedOutputPg2
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg2)

// Construction
public:
	CKPSelectedOutputPg2();
	~CKPSelectedOutputPg2();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg2)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG2 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcTotals;
	CMSHFlexGrid	m_egTotals;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listTotals;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg2)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg2)
	afx_msg void OnItemchangedClTotals(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgTotals(short FAR* KeyCode, short Shift);
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusClTotals(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg3 dialog

#ifndef baseCKPSelectedOutputPg3
#define baseCKPSelectedOutputPg3 CKeywordPage
#endif

class CKPSelectedOutputPg3 : public baseCKPSelectedOutputPg3
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg3)

// Construction
public:
	CKPSelectedOutputPg3();
	~CKPSelectedOutputPg3();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg3)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG3 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcMolalities;
	CMSHFlexGrid	m_egMolalities;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listMolalities;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg3)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg3)
	afx_msg void OnItemchangedClMolalities(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgMolalities(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClMolalities(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnterCellMshfgMolalities();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg4 dialog

#ifndef baseCKPSelectedOutputPg4
#define baseCKPSelectedOutputPg4 CKeywordPage
#endif

class CKPSelectedOutputPg4 : public baseCKPSelectedOutputPg4
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg4)

// Construction
public:
	CKPSelectedOutputPg4();
	~CKPSelectedOutputPg4();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg4)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG4 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcActivities;
	CMSHFlexGrid	m_egActivities;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listActivities;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg4)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg4)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClActivities(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgActivities(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClActivities(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgActivities();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg5 dialog

#ifndef baseCKPSelectedOutputPg5
#define baseCKPSelectedOutputPg5 CKeywordPage
#endif

class CKPSelectedOutputPg5 : public baseCKPSelectedOutputPg5
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg5)

// Construction
public:
	CKPSelectedOutputPg5();
	~CKPSelectedOutputPg5();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg5)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG5 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcPhases;
	CMSHFlexGrid	m_egPhases;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listPhases;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg5)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg5)
	afx_msg void OnItemchangedClPhases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgPhases(short FAR* KeyCode, short Shift);
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusClPhases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgPhases();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg6 dialog

#ifndef baseCKPSelectedOutputPg6
#define baseCKPSelectedOutputPg6 CKeywordPage
#endif

class CKPSelectedOutputPg6 : public baseCKPSelectedOutputPg6
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg6)

// Construction
public:
	CKPSelectedOutputPg6();
	~CKPSelectedOutputPg6();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg6)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG6 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcPhases;
	CMSHFlexGrid	m_egPhases;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listPhases;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg6)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg6)
	afx_msg void OnSetfocusClPhases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedClPhases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgPhases(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgPhases();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg6 dialog

#ifndef baseCKPSelectedOutputPg7
#define baseCKPSelectedOutputPg7 CKeywordPage
#endif

class CKPSelectedOutputPg7 : public baseCKPSelectedOutputPg7
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg7)

// Construction
public:
	CKPSelectedOutputPg7();
	~CKPSelectedOutputPg7();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg7)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG7 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcGases;
	CMSHFlexGrid	m_egGases;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listGases;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg7)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKPSelectedOutputPg7)
	afx_msg void OnItemchangedClGases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusClGases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgGases(short FAR* KeyCode, short Shift);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgGases();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

#endif // __KPSELECTEDOUTPUTPG1_H__
