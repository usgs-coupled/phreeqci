// MainFrame.h : interface of the CMainFrame class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRAME_H__A0B93958_3CA7_42E6_8596_08ED4B858CB8__INCLUDED_)
#define AFX_MAINFRAME_H__A0B93958_3CA7_42E6_8596_08ED4B858CB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkspaceBar.h"
#include "Util.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	CWorkspaceBar& GetWorkspaceBar();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	friend class CRichViewIn;
	friend class CRichDocIn;
	friend class CRichViewOut;
	friend class CChildFrame;

	// friend methods
	friend CStatusBar& CUtil::GetStatusBar();

	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndToolInitCond;
	CToolBar    m_wndToolForward;
	CToolBar    m_wndToolBarRun;
	CToolBar    m_wndToolPrint;
	CToolBar    m_wndToolStoichTherm;
	CReBar      m_wndReBar;
	CWorkspaceBar m_wndWorkspaceBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnHelpUser();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateOptionsWarnoverwriteinput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsWarnoverwriteoutput(CCmdUI* pCmdUI);
	afx_msg void OnOptionsWarnoverwriteinput();
	afx_msg void OnOptionsWarnoverwriteoutput();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAll();
	afx_msg void OnHelpFactsheet();
	//}}AFX_MSG
	void OnUpdateKeywordDetail(CCmdUI* pCmdUI);
	void OnKeywordDetail(UINT id);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__A0B93958_3CA7_42E6_8596_08ED4B858CB8__INCLUDED_)
