// DumpPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

// COMMENT: {11/15/2012 10:35:12 PM}//{{AFX_INCLUDES()
// COMMENT: {11/15/2012 10:35:12 PM}#include "mshflexgrid.h"
// COMMENT: {11/15/2012 10:35:12 PM}//}}AFX_INCLUDES

#include "EditGrid.h"
#include "gridctrl/ModGridCtrlEx.h"
#include "Tree.h"
#include "DefinedRanges.h"
#include "Keyword.h"
#include "dumper.h"

#if !defined(__DUMPPG1_H__)
#define __DUMPPG1_H__

// COMMENT: {11/13/2012 5:35:57 PM}#define OLD_GRID_CTRL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DumpPg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDumpPg1 dialog

#include "KeywordPage.h"
#include "StorageBinList.h"
#include "afxwin.h"

#ifndef baseDumpPg1
#define baseDumpPg1 CKeywordPage
#endif

class CDumpPg1 : public baseDumpPg1
{
	DECLARE_DYNCREATE(CDumpPg1)
	DECLARE_LAYOUT();

// Construction
public:
	CDumpPg1(CTreeCtrlNode simNode = CTreeCtrlNode());
	~CDumpPg1();

	void InitGrid();
	void ExchangeGrid(CDataExchange* pDX);
	void ExchangeItem(StorageBinListItem& item, int row);
	void ValidateGrid(CDataExchange* pDX);
	void DDX_Grid(CDataExchange *pDX);

	static const CString s_arrStrKeys[];
	int FillKeywords(HWND hWndCombo);

	bool bAll;
	dumper dump_info;

	static CString GetRanges(std::set<int>& nums);
	static CString GetRanges0(std::set<int> nums, CString acc);

	static std::set<int> GetCells(StorageBinList& bin);
	static bool GetAll(const StorageBinList& bin);
	static bool Add(StorageBinListItem& item);

	CString GetHelpString(int row, int col);

// Dialog Data
	//{{AFX_DATA(CDumpPg1)
	enum { IDD = IDD_KEY_DUMP_PG1 };
	CEdit	m_eDesc;
	CModGridCtrlEx   gridDelete;     // IDC_GRID_DELETE
	CButton btnAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDumpPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDefinedRanges m_ranges;
	std::map<CKeyword::type, std::set<CDBRange> > m_setNums;

	// Generated message map functions
	//{{AFX_MSG(CDumpPg1)
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// specialized grid notifications
	// afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnKillfocusEG(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedAllCheck();
	afx_msg void OnBnSetfocusAllCheck();
	afx_msg void OnSelChangedDelete(NMHDR *pNotifyStruct, LRESULT *result);
	afx_msg void OnBnClickedBrowseFname();
	afx_msg void OnEnSetfocusEditFname();
	afx_msg void OnBnSetfocusBrowseFname();
	afx_msg void OnBnSetfocusCheckAppend();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DUMPPG1_H__)
