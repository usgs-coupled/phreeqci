// ClipListBox.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPLISTBOX_H__8809BB61_F054_48D4_9CCF_8246C52E6A1E__INCLUDED_)
#define AFX_CLIPLISTBOX_H__8809BB61_F054_48D4_9CCF_8246C52E6A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClipListBox window

class CClipListBox : public CListBox
{
// Construction
public:
	CClipListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClipListBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClipListBox();

protected:
	HACCEL m_hAccelTable;       // accelerator table

	// Generated message map functions
protected:
	//{{AFX_MSG(CClipListBox)
	afx_msg void OnCopy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg BOOL OnDblclk();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIPLISTBOX_H__8809BB61_F054_48D4_9CCF_8246C52E6A1E__INCLUDED_)
