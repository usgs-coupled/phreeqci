// AllVirtualTreeCtrl.cpp : implementation file
//
// $Date: 2001/09/14 20:11:58 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "AllVirtualTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualTreeCtrl

CAllVirtualTreeCtrl::CAllVirtualTreeCtrl()
{
}

CAllVirtualTreeCtrl::~CAllVirtualTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CAllVirtualTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CAllVirtualTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_NOTIFY_REFLECT(NM_OUTOFMEMORY, OnOutofmemory)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_RDBLCLK, OnRdblclk)
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginrdrag)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TVN_SETDISPINFO, OnSetdispinfo)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PARENTNOTIFY_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualTreeCtrl message handlers

void CAllVirtualTreeCtrl::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// Add your specialized code here and/or call the base class
	
	CTreeCtrl::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CAllVirtualTreeCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CAllVirtualTreeCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::DefWindowProc(message, wParam, lParam);
}

BOOL CAllVirtualTreeCtrl::DestroyWindow() 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::DestroyWindow();
}

void CAllVirtualTreeCtrl::DoDataExchange(CDataExchange* pDX) 
{
	// Add your specialized code here and/or call the base class
	
	CTreeCtrl::DoDataExchange(pDX);
}

CScrollBar* CAllVirtualTreeCtrl::GetScrollBarCtrl(int nBar) const
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::GetScrollBarCtrl(nBar);
}

BOOL CAllVirtualTreeCtrl::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::OnAmbientProperty(pSite, dispid, pvar);
}

BOOL CAllVirtualTreeCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CAllVirtualTreeCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CAllVirtualTreeCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::OnCommand(wParam, lParam);
}

void CAllVirtualTreeCtrl::OnFinalRelease() 
{
	// Add your specialized code here and/or call the base class
	
	CTreeCtrl::OnFinalRelease();
}

BOOL CAllVirtualTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::OnNotify(wParam, lParam, pResult);
}

void CAllVirtualTreeCtrl::PostNcDestroy() 
{
	// Add your specialized code here and/or call the base class
	
	CTreeCtrl::PostNcDestroy();
}

BOOL CAllVirtualTreeCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::PreCreateWindow(cs);
}

void CAllVirtualTreeCtrl::PreSubclassWindow() 
{
	// Add your specialized code here and/or call the base class
	
	CTreeCtrl::PreSubclassWindow();
}

BOOL CAllVirtualTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CAllVirtualTreeCtrl::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

LRESULT CAllVirtualTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

void CAllVirtualTreeCtrl::WinHelp(DWORD dwData, UINT nCmd) 
{
	// Add your specialized code here and/or call the base class
	
	CTreeCtrl::WinHelp(dwData, nCmd);
}

void CAllVirtualTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnOutofmemory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnRdblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVDISPINFO* pTVDispInfo = (NMTVDISPINFO*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVDISPINFO* pTVDispInfo = (NMTVDISPINFO*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVDISPINFO* pTVDispInfo = (NMTVDISPINFO*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVKEYDOWN* pTVKeyDown = (NMTVKEYDOWN*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

void CAllVirtualTreeCtrl::OnSetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVDISPINFO* pTVDispInfo = (NMTVDISPINFO*)pNMHDR;
	//  Add your control notification handler code here
	
	*pResult = 0;
}

HBRUSH CAllVirtualTreeCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// Change any attributes of the DC here
	
	// Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CAllVirtualTreeCtrl::ParentNotify(UINT message, LPARAM lParam) 
{
	// Add your message handler code here
	
}
