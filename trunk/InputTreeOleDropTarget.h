// InputTreeOleDropTarget.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTTREEOLEDROPTARGET_H__E71E51EB_D742_4F2D_93F6_1155110EBACB__INCLUDED_)
#define AFX_INPUTTREEOLEDROPTARGET_H__E71E51EB_D742_4F2D_93F6_1155110EBACB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInputTreeOleDropTarget command target

class CInputTreeOleDropTarget : public COleDropTarget
{
public:
	CInputTreeOleDropTarget();
	virtual ~CInputTreeOleDropTarget();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputTreeOleDropTarget)
	//}}AFX_VIRTUAL
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputTreeOleDropTarget)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTTREEOLEDROPTARGET_H__E71E51EB_D742_4F2D_93F6_1155110EBACB__INCLUDED_)
