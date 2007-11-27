// AllVirtualPropertySheet.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "AllVirtualPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualPropertySheet

IMPLEMENT_DYNAMIC(CAllVirtualPropertySheet, CPropertySheet)

CAllVirtualPropertySheet::CAllVirtualPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CAllVirtualPropertySheet::CAllVirtualPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CAllVirtualPropertySheet::~CAllVirtualPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CAllVirtualPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAllVirtualPropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualPropertySheet message handlers

void CAllVirtualPropertySheet::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// Add your specialized code here and/or call the base class
	
	CPropertySheet::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CAllVirtualPropertySheet::ContinueModal() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::ContinueModal();
}

BOOL CAllVirtualPropertySheet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CAllVirtualPropertySheet::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::DefWindowProc(message, wParam, lParam);
}

BOOL CAllVirtualPropertySheet::DestroyWindow() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::DestroyWindow();
}

void CAllVirtualPropertySheet::DoDataExchange(CDataExchange* pDX) 
{
	// Add your specialized code here and/or call the base class
	
	CPropertySheet::DoDataExchange(pDX);
}

int CAllVirtualPropertySheet::DoModal() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::DoModal();
}

CScrollBar* CAllVirtualPropertySheet::GetScrollBarCtrl(int nBar) const
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::GetScrollBarCtrl(nBar);
}

BOOL CAllVirtualPropertySheet::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::OnAmbientProperty(pSite, dispid, pvar);
}

BOOL CAllVirtualPropertySheet::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CAllVirtualPropertySheet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CAllVirtualPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::OnCommand(wParam, lParam);
}

void CAllVirtualPropertySheet::OnFinalRelease() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertySheet::OnFinalRelease();
}

BOOL CAllVirtualPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// Add your specialized code here
	
	return bResult;
}

BOOL CAllVirtualPropertySheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}

void CAllVirtualPropertySheet::PostNcDestroy() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
}

BOOL CAllVirtualPropertySheet::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::PreCreateWindow(cs);
}

void CAllVirtualPropertySheet::PreSubclassWindow() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertySheet::PreSubclassWindow();
}

BOOL CAllVirtualPropertySheet::PreTranslateMessage(MSG* pMsg) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}

void CAllVirtualPropertySheet::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

LRESULT CAllVirtualPropertySheet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertySheet::WindowProc(message, wParam, lParam);
}

void CAllVirtualPropertySheet::WinHelp(DWORD dwData, UINT nCmd) 
{
	// Add your specialized code here and/or call the base class
	
	CPropertySheet::WinHelp(dwData, nCmd);
}
