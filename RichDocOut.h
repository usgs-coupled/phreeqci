// RichDocOut.h : interface of the CRichDocOut class
//
// $Date: 2000/10/04 20:17:52 $
// $Revision: 1.1.1.29 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHDOCOUT_H__7BC19A1F_C662_44EE_87F7_B0EF0857B1B3__INCLUDED_)
#define AFX_RICHDOCOUT_H__7BC19A1F_C662_44EE_87F7_B0EF0857B1B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeCtrlOut.h"	// for enums

// forward declarations
class CWorkspaceBar;
class CTreeCtrlNode;
class CRichEditLineParserSafe;

/*
NOTE: May want to use in future
static UINT NEAR WM_FIND = RegisterWindowMessage("COMMDLG_FIND");
*/
#define WM_USER_THREAD_ABORTED		(WM_USER+100)
#define WM_USER_THREAD_FINISHED		(WM_USER+101)


class CRichDocOut : public CRichEditDoc
{
protected: // create from serialization only
	CRichDocOut();
	DECLARE_DYNCREATE(CRichDocOut)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichDocOut)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual void OnChangedViewList();
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CRichDocOut();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:

	//{{AFX_MSG(CRichDocOut)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/**
class CFillTreeThreadProcParam  
{
public:
	CTreeCtrlNode* m_pBaseNode;
	const HWND m_hwndRichEdit;
	const CEvent* m_pAbortEvent;
	CFillTreeThreadProcParam(HWND hwndRichEdit);
	virtual ~CFillTreeThreadProcParam();

};
**/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHDOCOUT_H__7BC19A1F_C662_44EE_87F7_B0EF0857B1B3__INCLUDED_)
