// RichViewIn.h : interface of the CRichViewIn class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHVIEWIN_H__9A57B7D5_849B_41D2_8742_EB013B774BAC__INCLUDED_)
#define AFX_RICHVIEWIN_H__9A57B7D5_849B_41D2_8742_EB013B774BAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRichInCntrItem;
class CWorkspaceBar;


class CRichViewIn : public CRichEditView
{
protected: // create from serialization only
	CRichViewIn();
	DECLARE_DYNCREATE(CRichViewIn)

// Attributes
public:
	CRichDocIn* GetDocument();

// Operations
public:

// Overrides
	virtual HMENU GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg);
	virtual HRESULT QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat, DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichViewIn)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichViewIn();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	friend class CTreeCtrlIn;

// Generated message map functions
protected:
	void JumpToError(CObject* pHint);
	bool m_bOnInitialUpdateCalled;
	CWorkspaceBar& m_workSpace;
	CHARFORMAT m_cf;
	CLIPFORMAT m_nTreeCtrlInClipFormat;
	//{{AFX_MSG(CRichViewIn)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnHelp();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	//{{
	afx_msg void OnUpdateKey(CCmdUI* pCmdUI);
	afx_msg void OnKey(UINT nID);
	//}}
	//}}AFX_MSG
	// user messages
	afx_msg void OnEditChange();
	DECLARE_MESSAGE_MAP()

// Interface Map
public:
	bool m_bDraggingText;
	bool IsDraggingText();
	BEGIN_INTERFACE_PART(MyRichEditOleCallback, IRichEditOleCallback)
		INIT_INTERFACE_PART(CRichViewIn, MyRichEditOleCallback)
		STDMETHOD(GetNewStorage) (LPSTORAGE*);
		STDMETHOD(GetInPlaceContext) (LPOLEINPLACEFRAME*,
									  LPOLEINPLACEUIWINDOW*,
									  LPOLEINPLACEFRAMEINFO);
		STDMETHOD(ShowContainerUI) (BOOL);
		STDMETHOD(QueryInsertObject) (LPCLSID, LPSTORAGE, LONG);
		STDMETHOD(DeleteObject) (LPOLEOBJECT);
		STDMETHOD(QueryAcceptData) (LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
		STDMETHOD(ContextSensitiveHelp) (BOOL);
		STDMETHOD(GetClipboardData) (CHARRANGE*, DWORD, LPDATAOBJECT*);
		STDMETHOD(GetDragDropEffect) (BOOL, DWORD, LPDWORD);
		STDMETHOD(GetContextMenu) (WORD, LPOLEOBJECT, CHARRANGE*, HMENU*);
	END_INTERFACE_PART(MyRichEditOleCallback)

	DECLARE_INTERFACE_MAP()
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // debug version in RichViewIn.cpp
inline CRichDocIn* CRichViewIn::GetDocument()
   { return (CRichDocIn*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHVIEWIN_H__9A57B7D5_849B_41D2_8742_EB013B774BAC__INCLUDED_)
