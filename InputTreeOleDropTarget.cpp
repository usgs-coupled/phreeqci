// InputTreeOleDropTarget.cpp : implementation file
//
// $Date: 2000/10/04 20:17:42 $
// $Revision: 1.1.1.27 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "InputTreeOleDropTarget.h"

#include "TreeCtrlIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputTreeOleDropTarget

CInputTreeOleDropTarget::CInputTreeOleDropTarget()
{
}

CInputTreeOleDropTarget::~CInputTreeOleDropTarget()
{
}


BEGIN_MESSAGE_MAP(CInputTreeOleDropTarget, COleDropTarget)
	//{{AFX_MSG_MAP(CInputTreeOleDropTarget)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputTreeOleDropTarget message handlers
DROPEFFECT CInputTreeOleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
												DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CTreeCtrlIn, pWnd);

	return ((CTreeCtrlIn*)pWnd)->OnDragEnter(pDataObject, dwKeyState, point);
}

void CInputTreeOleDropTarget::OnDragLeave(CWnd* pWnd)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CTreeCtrlIn, pWnd);

	((CTreeCtrlIn*)pWnd)->OnDragLeave();

	// call base class
	COleDropTarget::OnDragLeave(pWnd);
}

DROPEFFECT CInputTreeOleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
											   DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CTreeCtrlIn, pWnd);

	return ((CTreeCtrlIn*)pWnd)->OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CInputTreeOleDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	// TRACE0(_T("CInputTreeOleDropTarget::OnDragScroll\n"));
	return COleDropTarget::OnDragScroll(pWnd, dwKeyState, point);
}

BOOL CInputTreeOleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect,
									 CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CTreeCtrlIn, pWnd);

	return ((CTreeCtrlIn*)pWnd)->OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT CInputTreeOleDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
											 DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
	// TRACE0(_T("CInputTreeOleDropTarget::OnDropEx\n"));
	return COleDropTarget::OnDropEx(pWnd, pDataObject, dropDefault, dropList, point);
}
