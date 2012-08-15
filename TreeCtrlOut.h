// TreeCtrlOut.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLOUT_H__AD7C2918_E37A_4659_B4F8_727942EB5380__INCLUDED_)
#define AFX_TREECTRLOUT_H__AD7C2918_E37A_4659_B4F8_727942EB5380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlOut window
#include "Tree.h"

#ifndef baseCTreeCtrlOut
#define baseCTreeCtrlOut CTreeCtrlEx
#endif

class CTreeCtrlOut : public baseCTreeCtrlOut
{
// Construction
public:
	CTreeCtrlOut();

// Attributes
public:

// Operations
public:
	enum ImageIndex
	{
		UNUSED00Node,
		UNUSED01Node,
		workNode,
		lineNode,
		fileNode,
		solutionNode,
		reactionNode,
		solidSolutionsNode,
		errorNode,
		inverseNode,
		surfaceNode,
		exchangeNode,
		transportNode,
		advectionNode,
		userPrintNode,
		gasPhaseNode
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlOut)
	//}}AFX_VIRTUAL

// Implementation
public:
	CImageList m_ctrlImage;
	virtual ~CTreeCtrlOut();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlOut)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupClose();
	afx_msg void OnSubfileGoto();
	afx_msg void OnFileCloseAllOutput();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLOUT_H__AD7C2918_E37A_4659_B4F8_727942EB5380__INCLUDED_)
