// KPUsePg1.h : header file
//
// $Id: KPDeletePg1.h 138 2005-02-10 02:58:28Z charlton $
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#include "EditGrid.h"
#include "gridctrl/ModGridCtrlEx.h"
#include "Tree.h"
#include "DefinedRanges.h"
#include "Keyword.h"

#if !defined(__DELETEPG1_H__)
#define __DELETEPG1_H__

// COMMENT: {11/13/2012 5:35:57 PM}#define OLD_GRID_CTRL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KPDeletePg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeletePg1 dialog

#include "KeywordPage.h"
#include "StorageBinList.h"
#include "afxwin.h"

#ifndef baseDeletePg1
#define baseDeletePg1 CKeywordPage
#endif

class CDeletePg1 : public baseDeletePg1
{
	DECLARE_DYNCREATE(CDeletePg1)
	DECLARE_LAYOUT();

// Construction
public:
	CDeletePg1(CTreeCtrlNode simNode = CTreeCtrlNode());
	~CDeletePg1();

	void InitGrid();
	void ExchangeGrid(CDataExchange* pDX);
	void ExchangeItem(StorageBinListItem &item, int row);
	void ValidateGrid(CDataExchange* pDX);
	void DDX_Grid(CDataExchange *pDX);

	static const CString s_arrStrKeys[];
	int FillKeywords(HWND hWndCombo);

	StorageBinList delete_info;
	bool bAll;


	static CString GetRanges(std::set<int>& nums);
	static CString GetRanges0(std::set<int> nums, CString acc);

	static std::set<int> GetCells(StorageBinList& bin);
	static bool GetAll(StorageBinList bin);

// Dialog Data
	//{{AFX_DATA(CDeletePg1)
#ifdef OLD_GRID_CTRL
	enum { IDD = IDD_KEY_DELETE_PG1 };
#else
	enum { IDD = IDD_KEY_DELETE_PG2 };
#endif /* OLD_GRID_CTRL */
	CEdit	m_eDesc;
#ifdef OLD_GRID_CTRL
	CEditGrid	m_egDelete;
#else
	CModGridCtrlEx   gridDelete;     // IDC_GRID_DELETE
#endif
	CButton btnAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDeletePg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDefinedRanges m_ranges;
	// std::set<CDBRange, std::less<CDBRange> > m_arrRangeSet[10];
	std::map<CKeyword::type, std::set<CDBRange> > m_setNums;


	// Generated message map functions
	//{{AFX_MSG(CDeletePg1)
#ifdef OLD_GRID_CTRL
	afx_msg void OnEnterCellMshfgDelete();
	afx_msg void OnKeyDownMshfgDelete(short FAR* KeyCode, short Shift);
#endif /* OLD_GRID_CTRL */
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// specialized grid notifications
#ifdef OLD_GRID_CTRL
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
#endif /* OLD_GRID_CTRL */
	// afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnKillfocusEG(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAllCheck();
	afx_msg void OnBnSetfocusAllCheck();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DELETEPG1_H__)
