// KPSelectedOutputPg8.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_KPSELECTEDOUTPUTPG8_H__B5DF12F0_7504_43DB_8CA7_C34D185234EF__INCLUDED_)
#define AFX_KPSELECTEDOUTPUTPG8_H__B5DF12F0_7504_43DB_8CA7_C34D185234EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KPSelectedOutputPg8.h : header file
//

#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "GridListDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg8 dialog

#ifndef baseCKPSelectedOutputPg8
#define baseCKPSelectedOutputPg8 CKeywordPage
#endif

class CKPSelectedOutputPg8 : public baseCKPSelectedOutputPg8
{
	DECLARE_DYNCREATE(CKPSelectedOutputPg8)

// Construction
public:
	CKPSelectedOutputPg8();
	~CKPSelectedOutputPg8();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	//{{AFX_DATA(CKPSelectedOutputPg8)
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG8 };
	CEdit	m_eInputDesc;
	CCheckListCtrl	m_clcKinetic;
	CMSHFlexGrid	m_egKinetic;
	//}}AFX_DATA
#undef CMSHFlexGrid
	std::list<CString> m_listKinetic;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSelectedOutputPg8)
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
	//{{AFX_MSG(CKPSelectedOutputPg8)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClKinetic(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusClKinetic(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgKinetic(short FAR* KeyCode, short Shift);
	afx_msg void OnEnterCellMshfgKinetic();
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

#endif // !defined(AFX_KPSELECTEDOUTPUTPG8_H__B5DF12F0_7504_43DB_8CA7_C34D185234EF__INCLUDED_)
