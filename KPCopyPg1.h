// KPUsePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#include "EditGrid.h"
#include "Tree.h"
#include "DefinedRanges.h"
#include "Keyword.h"

#if !defined(__KPCOPYPG1_H__)
#define __KPCOPYPG1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KPCopyPg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKPCopyPg1 dialog

#include "KeywordPage.h"

#ifndef baseKPCopyPg1
#define baseKPCopyPg1 CKeywordPage
#endif

class CKPCopyPg1 : public baseKPCopyPg1
{
	DECLARE_DYNCREATE(CKPCopyPg1)
	DECLARE_LAYOUT();

// Construction
public:
	CKPCopyPg1(CTreeCtrlNode simNode = CTreeCtrlNode());
	~CKPCopyPg1();

	void InitGrid();
	void ExchangeGrid(CDataExchange* pDX);
	void ValidateGrid(CDataExchange* pDX);
	void DDX_Grid(CDataExchange *pDX);

	static const CString s_arrStrKeys[];
	int FillKeywords(HWND hWndCombo);

	std::list< std::vector<int> > m_listCopies;


// Dialog Data
	//{{AFX_DATA(CKPCopyPg1)
	enum { IDD = IDD_KEY_COPY_PG1 };
	CEdit	m_eDesc;
	CEditGrid	m_egCopy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPCopyPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDefinedRanges m_ranges;
	// std::set<CDBRange, std::less<CDBRange> > m_arrRangeSet[10];
	std::map<CKeyword::type, std::set<CDBRange> > m_setNums;


	// Generated message map functions
	//{{AFX_MSG(CKPCopyPg1)
	afx_msg void OnEnterCellMshfgCopy();
	afx_msg void OnKeyDownMshfgCopy(short FAR* KeyCode, short Shift);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// specialized grid notifications
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnKillfocusEG(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__KPCOPYPG1_H__)
