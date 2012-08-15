#pragma once


// CTreeCtrlDB
#include "Tree.h"
#include "TreeCtrlKeyword.h"


#ifndef baseCTreeCtrlDB
#define baseCTreeCtrlDB CTreeCtrlKeyword
#endif

class CTreeCtrlDB : public baseCTreeCtrlDB
{
	DECLARE_DYNAMIC(CTreeCtrlDB)

public:
	CTreeCtrlDB();
	virtual ~CTreeCtrlDB();

// COMMENT: {8/13/2012 5:30:23 PM}	CTreeCtrlNode GetFileNode(CRichEditDoc *pDoc);
	CRichEditDoc* GetRichEditDoc(const CTreeCtrlNode& node)const;
	CRichEditView* GetRichEditView(const CTreeCtrlNode& node)const;
	CHARRANGE GetRange(CTreeCtrlNode node)const;

protected:
	virtual void RemoveDatabaseKeyword(CRichEditDoc *pDoc);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDbClose();
	afx_msg void OnDbCloseAll();
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDbGoto();
};


