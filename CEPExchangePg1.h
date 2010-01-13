// CEPExchangePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CEPEXCHANGEPG1_H__
#define __CEPEXCHANGEPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCommonExchangePage dialog

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"
#include "DefinedRanges.h"

// forward declaration
class CCKSExchange;

#include "CommonKeywordPage.h"

#ifdef baseCommonExchangePage
#error
#endif

#ifndef baseCommonExchangePage
#define baseCommonExchangePage CCommonKeywordPage
#endif

class CCommonExchangePage : public baseCommonExchangePage
{
	DECLARE_DYNCREATE(CCommonExchangePage)
	DECLARE_LAYOUT();

// Construction
public:
	CCommonExchangePage();
	CCommonExchangePage(UINT nIDTemplate, UINT nIDCaption = 0);
	CCommonExchangePage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

	~CCommonExchangePage();

public:
	CCKSExchange* GetSheet();

public:
	std::list<CExchComp> m_listExchComp;


// Dialog Data
	//{{AFX_DATA(CCommonExchangePage)
	enum { IDD = IDD_KEY_EXCHANGE_COMMON };
	CCheckListCtrl	m_ctrlExSpecCheckList;
	CStatic	    m_sDefExSpec;
	CComboBox	m_cboSolutions;
	CButton	    m_btnEquilibrate;
	CButton	    m_btnPitzer;
	CEditGrid	m_ctrlExSpecEditGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonExchangePage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_Equilibrate(CDataExchange* pDX);
	void DDX_Pitzer(CDataExchange* pDX);	
	BOOL InitExSpecCheckList();
	void UpdateEquilState();

protected:
	CGridListDoc m_glDoc;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonExchangePage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnEquilibrate();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnSetfocusCbSolutions();
	afx_msg void OnSetfocusClExchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedClExchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom setfocus handlers
	afx_msg void OnSetfocusBtnEquil();
	afx_msg void OnSetfocusBtnPitzer();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CEPExchangePg1.cpp
inline CCKSExchange* CCommonExchangePage::GetSheet()
   { return (CCKSExchange*)GetParent(); }
#endif


/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg1 dialog

#ifndef baseCEPExchangePg1
#define baseCEPExchangePg1 CCommonExchangePage
#endif

class CCEPExchangePg1 : public baseCEPExchangePg1
{
	DECLARE_DYNCREATE(CCEPExchangePg1)

// Construction
public:
	CCEPExchangePg1();
	~CCEPExchangePg1();

// Dialog Data
	//{{AFX_DATA(CCEPExchangePg1)
	enum { IDD = IDD_KEY_EXCHANGE_PG1 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCEPExchangePg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnKillActive();

protected:
	void DDX_ExchCompList(CDataExchange* pDX, int nIDC);
	BOOL InitExSpecCheckList();
	BOOL InitExSpecEditGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCEPExchangePg1)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg void OnEquilibrate();
	afx_msg void OnSetfocusClExchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgExchange();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	// grid event handlers
	afx_msg void OnKeyDownMshfgExchange(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg2 dialog

#ifndef baseCEPExchangePg2
#define baseCEPExchangePg2 CCommonExchangePage
#endif

class CCEPExchangePg2 : public baseCEPExchangePg2
{
	DECLARE_DYNCREATE(CCEPExchangePg2)

// Construction
public:
	CCEPExchangePg2();
	~CCEPExchangePg2();

// Dialog Data
	//{{AFX_DATA(CCEPExchangePg2)
	enum { IDD = IDD_KEY_EXCHANGE_PG2 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCEPExchangePg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_ExchCompList(CDataExchange* pDX, int nIDC);
	BOOL InitExSpecEditGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCEPExchangePg2)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg void OnEnterCellMshfgExchange();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	// grid event handlers
	afx_msg void OnKeyDownMshfgExchange(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CCEPExchangePg3 dialog

#ifndef baseCEPExchangePg3
#define baseCEPExchangePg3 CCommonExchangePage
#endif

class CCEPExchangePg3 : public baseCEPExchangePg3
{
	DECLARE_DYNCREATE(CCEPExchangePg3)

// Construction
public:
	CCEPExchangePg3();
	~CCEPExchangePg3();

// Dialog Data
	//{{AFX_DATA(CCEPExchangePg3)
	enum { IDD = IDD_KEY_EXCHANGE_PG3 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCEPExchangePg3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_ExchCompList(CDataExchange* pDX, int nIDC);
	BOOL InitExSpecEditGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCEPExchangePg3)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg void OnEnterCellMshfgExchange();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	// grid event handlers
	afx_msg void OnKeyDownMshfgExchange(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()

};



#endif // __CEPEXCHANGEPG1_H__
