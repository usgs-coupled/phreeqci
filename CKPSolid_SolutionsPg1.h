// CKPSolid_SolutionsPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#include "EditGrid.h"
#include "KeywordPageListItems.h"

#ifndef __SOLIDSOLUTIONSPAGES_H__
#define __SOLIDSOLUTIONSPAGES_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPSolid_SolutionsPg1 dialog

#include "CommonKeywordPage.h"

#ifndef baseCKPSolid_SolutionsPg1
#define baseCKPSolid_SolutionsPg1 CCommonKeywordPage
#endif

class CCKPSolid_SolutionsPg1 : public baseCKPSolid_SolutionsPg1
{
	DECLARE_DYNCREATE(CCKPSolid_SolutionsPg1)

// Construction
public:
	std::list<CS_S> m_listS_S;

	long GetLastCompRow(long nRow = -1);
	CCKPSolid_SolutionsPg1();
	~CCKPSolid_SolutionsPg1();

// Dialog Data
	//{{AFX_DATA(CCKPSolid_SolutionsPg1)
	enum { IDD = IDD_KEY_SOLID_SOLUTIONS_PG1 };
	CEditGrid	m_egSolSols;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPSolid_SolutionsPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	void DDX_S_S_List(CDataExchange* pDX, int nIDC);
	void InitGrid();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPSolid_SolutionsPg1)
	afx_msg void OnBtnAddComp();
	afx_msg void OnKeyDownMshfgSolSols(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgSolSols();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnBtnRemComp();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom setfocus handlers
	afx_msg void OnSetfocusBtnAddComp();
	afx_msg void OnSetfocusBtnRemComp();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CCKPSolid_SolutionsPg2 dialog

#ifndef baseCKPSolid_SolutionsPg2
#define baseCKPSolid_SolutionsPg2 CCommonKeywordPage
#endif

class CCKPSolid_SolutionsPg2 : public baseCKPSolid_SolutionsPg2
{
	DECLARE_DYNCREATE(CCKPSolid_SolutionsPg2)

// Construction
public:
	long GetLastCompRow(long nRow = -1);
	CCKPSolid_SolutionsPg2();
	~CCKPSolid_SolutionsPg2();

	std::list<CS_S> m_listS_S;
	
	std::map<enum CS_S::InputCase, CString> m_mapIC2String;
	std::map<CString, enum CS_S::InputCase> m_mapString2IC;

// Dialog Data
	//{{AFX_DATA(CCKPSolid_SolutionsPg2)
	enum { IDD = IDD_KEY_SOLID_SOLUTIONS_PG2 };
	CEditGrid	m_egSolSols;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPSolid_SolutionsPg2)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void DDX_S_S_List(CDataExchange* pDX, int nIDC);
	void InitGrid();


// Implementation
protected:

	std::list<CDBObject> m_listOptions;


	// Generated message map functions
	//{{AFX_MSG(CCKPSolid_SolutionsPg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDownMshfgSolSols(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgSolSols();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#endif // __SOLIDSOLUTIONSPAGES_H__
