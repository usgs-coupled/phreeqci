// KeywordPage.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KeywordPage.h"

#include "RichDocIn.h"
#include "KeywordSheet.h"

//{{
#include "Util.h"
#include "MainFrame.h"
//}}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeywordPage property page

const double CKeywordPage::double_NaN = std::numeric_limits<double>::signaling_NaN();
const int CKeywordPage::int_NaN = INT_MIN;

IMPLEMENT_DYNCREATE(CKeywordPage, baseKeywordPage)

CKeywordPage::CKeywordPage()
: baseKeywordPage()
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN);
	ASSERT(!(double_NaN == 0.0));
	ASSERT(double_NaN != 0.0);
	ASSERT(!_finite(double_NaN));
	//{{AFX_DATA_INIT(CKeywordPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKeywordPage::CKeywordPage(UINT nIDTemplate, UINT nIDCaption /*= 0*/)
: baseKeywordPage(nIDTemplate, nIDCaption)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN);
	ASSERT(!(double_NaN == 0.0));
	ASSERT(double_NaN != 0.0);
	//{{AFX_DATA_INIT(CKeywordPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKeywordPage::CKeywordPage(LPCTSTR lpszTemplateName, UINT nIDCaption /*= 0*/)
: baseKeywordPage(lpszTemplateName, nIDCaption)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN);
	ASSERT(!(double_NaN == 0.0));
	ASSERT(double_NaN != 0.0);
	//{{AFX_DATA_INIT(CKeywordPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKeywordPage::~CKeywordPage()
{
}

void CKeywordPage::DoDataExchange(CDataExchange* pDX)
{
	baseKeywordPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeywordPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeywordPage, baseKeywordPage)
	//{{AFX_MSG_MAP(CKeywordPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeywordPage message handlers

CRichDocIn* CKeywordPage::GetActiveDocument() const
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMDIFrameWnd, pFrame);

	// Get the active MDI child window.
	CMDIChildWnd *pChild = pFrame->MDIGetActive();
	CDocument *pDoc = pChild->GetActiveDocument();

	// FIXED: Output window may be the active doc
	// GetActiveDocument is set in CTreeCtrlIn::OnKey CTreeCtrlIn::OnKeyA and CTreeCtrlIn::OnEditKeyword
	ASSERT_KINDOF(CRichDocIn, pDoc);

	return (CRichDocIn*)pDoc;
}

const CDatabase& CKeywordPage::GetDatabase() const
{
	return GetActiveDocument()->database;
}

BOOL CKeywordPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	return baseKeywordPage::OnHelpInfo(pHelpInfo);
}

bool CKeywordPage::IsContextHelp()
{
	CKeywordSheet* pSheet = (CKeywordSheet*)GetParent();
	ASSERT_KINDOF(CKeywordSheet, pSheet);
	return pSheet->bContextHelp;
}
