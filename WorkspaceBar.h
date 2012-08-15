// WorkspaceBar.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__65FC535D_0942_4EC2_A835_4273C2C873AC__INCLUDED_)
#define AFX_WORKSPACEBAR_H__65FC535D_0942_4EC2_A835_4273C2C873AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar window

#include "Posea\sizecbar.h"
#include "FixedTabCtrl.h"
#include "TreeCtrlIn.h"
#include "TreeCtrlOut.h"
#include "TreeCtrlDB.h"
#include "TreeCtrlErr.h"
#include "TreeCtrlPfw.h"

#ifndef baseWorkspaceBar
#define baseWorkspaceBar CSizingControlBar
#endif

class CWorkspaceBar : public baseWorkspaceBar
{
	DECLARE_DYNAMIC(CWorkspaceBar);
// Construction
public:
	CWorkspaceBar();

// Attributes
public:
	enum TabIndex
	{
		InputTab,
		OutputTab,
		DatabaseTab,
		ErrorTab,
		PfwTab,
	};

	enum TabImage
	{
		InputImage,
		OutputImage,
		DatabaseImage,
		ErrorImage,
		PfwImage,
	};

// Operations
public:

// Overrides
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CTreeCtrlIn& GetTreeCtrlIn();
	int SyncInputTrees(CRichEditDoc* pDoc);
	void UpdateAllInViews(CDocument* pDoc, LPARAM lHint, CObject* pHint);

	// input
	void AddInputDoc(CRichEditDoc* pDoc);
	void RemoveInputDoc(CRichEditDoc* pDoc);
	void RemoveInputFileNode(CTreeCtrlNode node);

	// output
	CTreeCtrlNode AddOutputFileNode(LPCTSTR lpszLabel);

	// database
	void AddDatabaseDoc(CRichEditDoc* pDoc);
	void RemoveDatabaseDoc(CRichEditDoc* pDoc);
	void SelectDatabaseTreeItem(CRichEditDoc* pDoc);

	// error
	void RemoveErrorFileNode(CTreeCtrlNode node);

	// tabs
	int GetTabIndex(enum CWorkspaceBar::TabImage imageIndex);
	int GetInsertIndex(enum CWorkspaceBar::TabImage imageIndex);
	enum TabIndex GetCurSelTab()const;

	virtual ~CWorkspaceBar();

protected:  // control bar embedded members
	CFixedTabCtrl    m_wndTab;      // IDC_TC_WORKSPACE
	CFont            m_font;
	CTreeCtrlIn      m_wndTreeIn;   // IDC_TV_WS_IN
	CTreeCtrlOut     m_wndTreeOut;  // IDC_TV_WS_OUT
	CTreeCtrlDB      m_wndTreeDB;   // IDC_TV_WS_DB
	CTreeCtrlErr     m_wndTreeErr;  // IDC_TV_WS_ERR
	CTreeCtrlPfw     m_wndTreePfw;  // IDC_TV_WS_PFW

	friend class CRichViewIn;

	// Generated message map functions
protected:
	CImageList m_ctrlImage;

	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	// Tab control notifications
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);

	// Tree control notifications
	afx_msg void OnDeleteItemOut(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__65FC535D_0942_4EC2_A835_4273C2C873AC__INCLUDED_)
