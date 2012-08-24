// TreeCtrlIn.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLIN_H__34D2C593_13C9_412E_963B_6DC571043441__INCLUDED_)
#define AFX_TREECTRLIN_H__34D2C593_13C9_412E_963B_6DC571043441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlIn window

#include "TreeCtrlKeyword.h"

#ifndef baseCTreeCtrlIn
#define baseCTreeCtrlIn CTreeCtrlKeyword
#endif

#include "Keyword.h"
#include "InputTreeOleDropTarget.h"	// Added by ClassView

// forward declaration
class CWorkspaceBar;

class CTreeCtrlIn : public baseCTreeCtrlIn
{
	DECLARE_DYNAMIC(CTreeCtrlIn)
// Construction
public:
	CTreeCtrlIn();

// Attributes
public:

// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlIn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetNextNum(const CTreeCtrlNode& node, enum CTreeCtrlKeyword::ImageIndex nImageIndex);
	CTreeCtrlNode GetNextKeywordNode(const CTreeCtrlNode& rNode)const;
	CTreeCtrlNode GetPrevKeywordNode(const CTreeCtrlNode& rNode)const;
	bool GetClipBoardData(CString& rStr);
	long GetInsertPosBeforeKeyword(const CTreeCtrlNode& node);
	long GetInsertPosAfterKeyword(const CTreeCtrlNode& node);
	CTreeCtrlNode PasteString(CTreeCtrlNode nodeToPasteTo, CString& rStr);
	CWorkspaceBar& GetWorkspaceBar();
	void UpdateAllViews(CDocument *pDoc, LPARAM lHint, CObject *pHint);
	void RenumberSimulations(CTreeCtrlNode fileNode, CTreeCtrlNode firstSimNode = CTreeCtrlNode());
	CRichEditDoc* GetRichEditDoc(const CTreeCtrlNode& node)const;
	CRichEditView* GetRichEditView(const CTreeCtrlNode& node)const;
	CHARRANGE GetRange(CTreeCtrlNode node)const;

	afx_msg void OnUpdateKey(CCmdUI* pCmdUI);
	afx_msg void OnKey(UINT nID);
	afx_msg void OnKeyA(UINT nID);

	virtual ~CTreeCtrlIn();

	// Generated message map functions
protected:
	CTreeCtrlNode ReplaceNode(CTreeCtrlNode nodeToReplace, CString& rStr);
	void ClearNode(CTreeCtrlNode node);
	void CopyNode(CTreeCtrlNode node);
	CString GetNodeString(const CTreeCtrlNode& rNode)const;
	CString GetTransportNodeString(const CTreeCtrlNode& rNode)const;
	CString GetPreviousString(const CTreeCtrlNode& rNode)const;

	void DoKeyword(UINT nID, CTreeCtrlNode nodeToPasteTo);

	HTREEITEM m_hDragTreeItem;
	CLIPFORMAT m_nIDClipFormat;
	CInputTreeOleDropTarget m_OleDropTarget;
	CImageList* m_pDragImageList;
	friend class CWorkspaceBar;

	//{{AFX_MSG(CTreeCtrlIn)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnDebugSelect();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditKeyword(CCmdUI* pCmdUI);
	afx_msg void OnEditKeyword();
	afx_msg void OnUpdateViewProperties(CCmdUI* pCmdUI);
	afx_msg void OnViewProperties();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnFileClose();
	afx_msg void OnDebugInsertBef();
	afx_msg void OnDebugInsertAft();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnPopupClose();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRunInput();
	afx_msg void OnUpdateRunInput(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateFileCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunAll(CCmdUI* pCmdUI);
	afx_msg void OnRunAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAll();
	afx_msg void OnGoto();
	//}}AFX_MSG

	afx_msg void OnUpdateNeedDocSel(CCmdUI* pCmdUI);

	// drag and drop operations
	DROPEFFECT GetDropEffect(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	friend class CInputTreeOleDropTarget;
	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	void OnDragLeave();
	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	void SelectDropNode(CPoint point);

	// tree notifications
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLIN_H__34D2C593_13C9_412E_963B_6DC571043441__INCLUDED_)
