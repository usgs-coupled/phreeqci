#if !defined(AFX_CLIPTREECTRL_H__38C31022_6BBE_46B0_B8EC_1D538ADA6691__INCLUDED_)
#define AFX_CLIPTREECTRL_H__38C31022_6BBE_46B0_B8EC_1D538ADA6691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClipTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClipTreeCtrl window

class CClipTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CClipTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClipTreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClipTreeCtrl();

protected:
	HACCEL m_hAccelTable;       // accelerator table

	// Generated message map functions
protected:
	//{{AFX_MSG(CClipTreeCtrl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCopy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnRedrawTree(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIPTREECTRL_H__38C31022_6BBE_46B0_B8EC_1D538ADA6691__INCLUDED_)
