// TreeCtrlErr.h : header file
//
// $Date: 2000/10/04 20:17:57 $
// $Revision: 1.1.1.29 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLERR_H__6B5BB88E_76BA_4A76_9BCA_8EA9F2E1F811__INCLUDED_)
#define AFX_TREECTRLERR_H__6B5BB88E_76BA_4A76_9BCA_8EA9F2E1F811__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlErr window
#include "Tree.h"

#ifndef baseCTreeCtrlErr
#define baseCTreeCtrlErr CTreeCtrlEx
#endif

// forward declaration
class CWorkspaceBar;

class CTreeCtrlErr : public baseCTreeCtrlErr
{
// Construction
public:
	CTreeCtrlErr();

// Attributes
public:

// Operations
public:
	enum ImageIndex
	{
		UNUSED00Image,
		UNUSED01Image,
		workImage,
		lineImage,
		fileImage,
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlErr)
	//}}AFX_VIRTUAL

// Implementation
public:
	int CheckErrors(CRichEditDoc* pDoc);
	CTreeCtrlNode GetFileNode(CRichEditDoc *pDoc);
	CTreeCtrlNode m_baseNode;
	CTreeCtrlNode ParseTree(CRichEditDoc* pDoc);
	CImageList m_ctrlImage;
	CWorkspaceBar& GetWorkspaceBar();
	void UpdateAllViews(CDocument *pDoc, LPARAM lHint, CObject *pHint);

	virtual ~CTreeCtrlErr();

	// Generated message map functions
protected:
	void LoadErrorFile(CDocument *pDoc, LPCTSTR szFile);

	//{{AFX_MSG(CTreeCtrlErr)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLERR_H__6B5BB88E_76BA_4A76_9BCA_8EA9F2E1F811__INCLUDED_)
