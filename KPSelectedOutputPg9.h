// KPSelectedOutputPg9.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_KPSELECTEDOUTPUTPG9_H__E727CF82_EAC3_47C6_B22E_D4D9229B442D__INCLUDED_)
#define AFX_KPSELECTEDOUTPUTPG9_H__E727CF82_EAC3_47C6_B22E_D4D9229B442D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KPSelectedOutputPg9.h : header file
//

#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "GridListDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg9 dialog

#ifndef baseCKPSelectedOutputPg9
#define baseCKPSelectedOutputPg9 CKeywordPage
#endif

class CKPSelectedOutputPg9 : public baseCKPSelectedOutputPg9
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg9)

// Construction
public:
	CKPSelectedOutputPg9();
	~CKPSelectedOutputPg9();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg9)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG9 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcSolid;
	CMSHFlexGrid	m_egSolid;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listSolid;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg9)
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
	//{{AFX_MSG(CKPSelectedOutputPg9)
	afx_msg void OnItemchangedClSolid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusClSolid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgSolid(short FAR* KeyCode, short Shift);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgSolid();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KPSELECTEDOUTPUTPG9_H__E727CF82_EAC3_47C6_B22E_D4D9229B442D__INCLUDED_)
