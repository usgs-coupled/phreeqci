// AllVirtualPropertyPage.cpp : implementation file
//
// $Date: 2001/09/14 20:11:58 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "AllVirtualPropertyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualPropertyPage property page

IMPLEMENT_DYNCREATE(CAllVirtualPropertyPage, CPropertyPage)

CAllVirtualPropertyPage::CAllVirtualPropertyPage() : CPropertyPage(CAllVirtualPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CAllVirtualPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAllVirtualPropertyPage::~CAllVirtualPropertyPage()
{
}

void CAllVirtualPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllVirtualPropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllVirtualPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAllVirtualPropertyPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualPropertyPage message handlers

void CAllVirtualPropertyPage::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CAllVirtualPropertyPage::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CAllVirtualPropertyPage::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::DefWindowProc(message, wParam, lParam);
}

BOOL CAllVirtualPropertyPage::DestroyWindow() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::DestroyWindow();
}

int CAllVirtualPropertyPage::DoModal() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::DoModal();
}

CScrollBar* CAllVirtualPropertyPage::GetScrollBarCtrl(int nBar) const
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::GetScrollBarCtrl(nBar);
}

BOOL CAllVirtualPropertyPage::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnAmbientProperty(pSite, dispid, pvar);
}

BOOL CAllVirtualPropertyPage::OnApply() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnApply();
}

void CAllVirtualPropertyPage::OnCancel() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::OnCancel();
}

BOOL CAllVirtualPropertyPage::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CAllVirtualPropertyPage::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CAllVirtualPropertyPage::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnCommand(wParam, lParam);
}

void CAllVirtualPropertyPage::OnFinalRelease() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::OnFinalRelease();
}

BOOL CAllVirtualPropertyPage::OnKillActive() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

BOOL CAllVirtualPropertyPage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CAllVirtualPropertyPage::OnOK() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}

BOOL CAllVirtualPropertyPage::OnQueryCancel() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnQueryCancel();
}

void CAllVirtualPropertyPage::OnReset() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::OnReset();
}

BOOL CAllVirtualPropertyPage::OnSetActive() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

void CAllVirtualPropertyPage::OnSetFont(CFont* pFont) 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::OnSetFont(pFont);
}

LRESULT CAllVirtualPropertyPage::OnWizardBack() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnWizardBack();
}

BOOL CAllVirtualPropertyPage::OnWizardFinish() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnWizardFinish();
}

LRESULT CAllVirtualPropertyPage::OnWizardNext() 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnWizardNext();
}

void CAllVirtualPropertyPage::PostNcDestroy() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::PostNcDestroy();
}

BOOL CAllVirtualPropertyPage::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::PreCreateWindow(cs);
}

void CAllVirtualPropertyPage::PreSubclassWindow() 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::PreSubclassWindow();
}

BOOL CAllVirtualPropertyPage::PreTranslateMessage(MSG* pMsg) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CAllVirtualPropertyPage::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

LRESULT CAllVirtualPropertyPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Add your specialized code here and/or call the base class
	
	return CPropertyPage::WindowProc(message, wParam, lParam);
}

void CAllVirtualPropertyPage::WinHelp(DWORD dwData, UINT nCmd) 
{
	// Add your specialized code here and/or call the base class
	
	CPropertyPage::WinHelp(dwData, nCmd);
}
