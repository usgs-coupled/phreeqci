// FixedTabCtrl.cpp : implementation file
//
// $Date: 2000/10/04 20:17:41 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "FixedTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFixedTabCtrl

CFixedTabCtrl::CFixedTabCtrl()
{
}

CFixedTabCtrl::~CFixedTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CFixedTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CFixedTabCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFixedTabCtrl message handlers

LRESULT CFixedTabCtrl::OnNotifyFormat(WPARAM wParam,LPARAM lParam)
{
#ifdef _DEBUG
	HWND hwndFrom = (HWND) wParam;
	ASSERT(::GetParent(hwndFrom) == m_hWnd);
	ASSERT(lParam == NF_QUERY);
#endif

	// default
	LRESULT lResult = 0;
	lResult = DefWindowProc(WM_NOTIFYFORMAT, wParam, lParam);

	// this may be fixed in a later version
	// and may assert in Win95 and Win98
	ASSERT(lResult == NFR_UNICODE);
	ASSERT(lResult != NFR_ANSI);
	// return lResult;

#ifdef _UNICODE
	ASSERT(::IsWindowUnicode(m_hWnd));
	return NFR_UNICODE;
#else
	ASSERT(!::IsWindowUnicode(m_hWnd));
	return NFR_ANSI;
#endif
} 
