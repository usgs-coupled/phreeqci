// RichViewOut.h : interface of the CRichViewOut class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHVIEWOUT_H__9A57B7D5_849B_41D2_8742_EB013B774BAC__INCLUDED_)
#define AFX_RICHVIEWOUT_H__9A57B7D5_849B_41D2_8742_EB013B774BAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRichOutCntrItem;
class CTreeCtrlNode;
class CWorkspaceBar;

class CRichViewOut : public CRichEditView
{
public:
// inner class
	class CThreadParam  
	{
	public:
		CTreeCtrlNode m_baseNode;
		const HWND m_hwndRichEdit;
		//const CEvent* m_pAbortEvent;
		//CEvent m_abortEvent;
		HANDLE m_hAbortEvent;
		CThreadParam(HWND hwndRichEdit, CTreeCtrlNode node, HANDLE hAbort);
		virtual ~CThreadParam();
	};
protected: // create from serialization only
	CRichViewOut();
	DECLARE_DYNCREATE(CRichViewOut)

// Attributes
public:
	static const long viewAddedOrRemoved;
	CRichDocOut* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichViewOut)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	static UINT FillTreeThreadProc(LPVOID pParam);
	virtual ~CRichViewOut();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	UINT m_uStatusTimer;
	static void Parse(CRichViewOut::CThreadParam* pParam);
	static void ParseBeginning(CRichEditLineParserSafe &parser, CTreeCtrlNode &baseNode, LPCTSTR lpsz/*CString rStr*/, enum CTreeCtrlOut::ImageIndex image, bool bCallParsePrintAll);
	static void ParseInverse(CRichEditLineParserSafe &parser, CTreeCtrlNode &baseNode);
	static void ParsePrintAll(CRichEditLineParserSafe& parser, CTreeCtrlNode &baseNode);
	CEvent m_eventAbort;
	bool m_bThreadStillActive;
	CWorkspaceBar& m_workSpace;
	CTreeCtrlNode m_baseTreeNode;
	CHARFORMAT m_cf;
	//{{AFX_MSG(CRichViewOut)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteSpecial(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOleEditProperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOleInsertNew(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnUserThreadFinished(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RichViewOut.cpp
inline CRichDocOut* CRichViewOut::GetDocument()
   { return (CRichDocOut*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHVIEWOUT_H__9A57B7D5_849B_41D2_8742_EB013B774BAC__INCLUDED_)
